#include "RTD.h"

static SPI_HandleTypeDef RTD_SPI;
extern SPI_HandleTypeDef hspi5;

void RTD_Init(void)
{
	
	RTD_SPI = hspi5;
	RTD_SPI.Instance = RTD_SPI_INS;
	RTD_SPI.Init.Mode = SPI_MODE_MASTER;
	RTD_SPI.Init.Direction = SPI_DIRECTION_2LINES;
	RTD_SPI.Init.DataSize = SPI_DATASIZE_8BIT;
	RTD_SPI.Init.NSS = SPI_NSS_SOFT;
	RTD_SPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	RTD_SPI.Init.CLKPhase = SPI_PHASE_2EDGE;
	RTD_SPI.Init.CLKPolarity = SPI_POLARITY_LOW;
	RTD_SPI.Init.TIMode = SPI_TIMODE_DISABLE;
	RTD_SPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	RTD_SPI.Init.CRCPolynomial = 10;
	RTD_SPI.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
	if(HAL_SPI_Init(&RTD_SPI) != HAL_OK)
	{
		while(1);
	}
	
	RTD_CS_OFF;
	HAL_Delay(100);
	RTDWireSelect(3);
	RTDEnableBias(0);
	RTDAutoConvert(1);
	RTDClearFault();
	RTDFilterSelect(50);
	
}

void WriteRTDReg(uint8_t Addr,uint8_t Data)
{
	RTD_CS_ON;
	Addr |= 0x80;
	HAL_SPI_Transmit(&RTD_SPI,&Addr,1,1);
	HAL_SPI_Transmit(&RTD_SPI,&Data,1,1);
	RTD_CS_OFF;
}

uint8_t ReadRTDReg(uint8_t Addr)
{
	uint8_t Ret = 0;
	uint8_t Tmp = 0xff;
	RTD_CS_ON;
	Addr &= 0x7F;
	HAL_SPI_Transmit(&RTD_SPI,&Addr,1,1);
	HAL_SPI_TransmitReceive(&RTD_SPI,(uint8_t*)&Tmp,&Ret,1,1);
	RTD_CS_OFF;
	return Ret;
}

uint16_t Read2RTDReg(uint8_t Addr)
{
	uint8_t Ret[2] = {0,0};
	uint16_t Ret16 = 0;
	uint8_t Tmp = 0xff;
	RTD_CS_ON;
	Addr &= 0x7F;
	HAL_SPI_Transmit(&RTD_SPI,&Addr,1,10);
	HAL_SPI_TransmitReceive(&RTD_SPI,(uint8_t*)&Tmp,&Ret[1],1,1);
	HAL_SPI_TransmitReceive(&RTD_SPI,(uint8_t*)&Tmp,&Ret[0],1,1);
	Ret16 = Ret[1] << 8 | Ret[0];
	RTD_CS_OFF;
	return Ret16;
}

uint8_t RTDReadFault(void)
{
	return ReadRTDReg(RTD_FAULTSTAT_REG);
}

void RTDClearFault(void)
{
	uint8_t t = ReadRTDReg(RTD_CONFIG_REG);
	t &= ~0x2C;
	t |= RTD_CONFIG_FAULTSTAT;
	WriteRTDReg(RTD_CONFIG_REG,t);
}

void RTDEnableBias(bool Enable)
{
	uint8_t t = ReadRTDReg(RTD_CONFIG_REG);
	if (Enable)
		t |= RTD_CONFIG_BIAS;
	else
		t &= ~RTD_CONFIG_BIAS;
	WriteRTDReg(RTD_CONFIG_REG, t);
}

void RTDAutoConvert(bool Enable)
{
	uint8_t t = ReadRTDReg(RTD_CONFIG_REG);
	if (Enable)
		t |= RTD_CONFIG_MODEAUTO;
	else
		t &= ~RTD_CONFIG_MODEAUTO;
	WriteRTDReg(RTD_CONFIG_REG, t);
}

void RTDWireSelect(uint8_t WireNum)
{
	uint8_t t = ReadRTDReg(RTD_CONFIG_REG);
	if (WireNum == 3)
		t |= RTD_CONFIG_3WIRE;
	else
		t &= ~RTD_CONFIG_3WIRE;
	WriteRTDReg(RTD_CONFIG_REG, t);
}

void RTDFilterSelect(uint8_t filterHz)
{
	uint8_t t = ReadRTDReg(RTD_CONFIG_REG);
	if (filterHz == 50)
		t |= RTD_CONFIG_FILT50HZ;
	else
		t &= ~RTD_CONFIG_FILT50HZ;
	WriteRTDReg(RTD_CONFIG_REG, t);
}

uint16_t RTDReadData(void)
{
	RTDClearFault();
	RTDEnableBias(1);
	HAL_Delay(10);//10
	uint8_t t = ReadRTDReg(RTD_CONFIG_REG);
	t |= RTD_CONFIG_1SHOT;
	WriteRTDReg(RTD_CONFIG_REG, t);
	HAL_Delay(70);//65
	uint16_t rtd = Read2RTDReg(RTD_RTDMSB_REG);
	if (rtd == 1)
		RTDReadFault();
	rtd >>= 1;
	return rtd;
}

bool RTDTempC(float* ReadTemp)
{
	bool isOk = false;
	float Z1, Z2, Z3, Z4, Rt, temp;
	Rt = RTDReadData();
	Rt /= 32768;
	Rt *= RTD_RREF;
	Z1 = -RTD_A;
	Z2 = RTD_A * RTD_A - (4 * RTD_B);
	Z3 = (4 * RTD_B) / RTD_RNOMINAL;
	Z4 = 2 * RTD_B;
	temp = Z2 + (Z3 * Rt);
	temp = (sqrtf(temp) + Z1) / Z4;
	
	if (temp >= 0)
	{
		*ReadTemp = temp;
		if(RTDReadFault() == 0)
			isOk = true;
		return isOk;
	}
	Rt /= RTD_RNOMINAL;
	Rt *= 100;
	float rpoly = Rt;
	temp = -242.02f;
	temp += 2.2228f * rpoly;
	rpoly *= Rt;  // square
	temp += 2.5859f * rpoly /1000.0f;
	rpoly *= Rt;  // ^3
	temp -= 4.8260f * rpoly/1000000.0f;
	rpoly *= Rt;  // ^4
	temp -= 2.8183f * rpoly/100000000.0f;
	rpoly *= Rt;  // ^5
	temp += 1.5243f * rpoly/10000000000.0f;
	
	*ReadTemp = temp;
	if(RTDReadFault() == 0)
		isOk = true;
	return isOk;
}

void RTDTransTemprature(sysVar* systemVariables)
{
	float Temprature;
	bool CheckRTD;
}