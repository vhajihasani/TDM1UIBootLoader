#include "FRAM.h"

static SPI_HandleTypeDef FRAM_SPI;
#define FRAM_BUFFER_SIZE   256
#define SETTING_PAGE       1
#define MAX_PAGE           256
extern SPI_HandleTypeDef hspi4;
uint8_t FRAM_tx[5];
uint8_t FRAM_rx[5];

uint8_t FRAM_txbuffer[FRAM_BUFFER_SIZE];
uint8_t FRAM_rxbuffer[FRAM_BUFFER_SIZE];

void FRAM_SPI_init(SPI_HandleTypeDef *hspi)
{
	FRAM_SPI = *hspi;
	hspi->Instance = FRAM_SPI_INS;
	hspi->Init.Mode = SPI_MODE_MASTER;
	hspi->Init.Direction = SPI_DIRECTION_2LINES;
	hspi->Init.DataSize = SPI_DATASIZE_8BIT;
	hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi->Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi->Init.NSS = SPI_NSS_SOFT;
	hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi->Init.TIMode = SPI_TIMODE_DISABLE;
	hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi->Init.CRCPolynomial = 10;
	if(HAL_SPI_Init(hspi) != HAL_OK)
	{
		while (HAL_SPI_Init(hspi) != HAL_OK);
	}
}

void FRAM_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	/* FRAM CS: PE4 */
	GPIO_InitStruct.Pin =GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP ;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	/* SOOTI : PE3 */ ////pppp
	GPIO_InitStruct.Pin =GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	FRAM_CS_OFF;
}

void FRAM_Init(SPI_HandleTypeDef *hspi)
{
	FRAM_GPIO_Init();
	FRAM_SPI_init(hspi);
	for (int i=0;i<9000000;i++);
	BSP_FM25CL64_WriteState(FM25CL64_UNPROTECT); 
}

void FRAM_Write(uint16_t page_num, uint8_t* pBuff)
{
	BSP_FM25CL64_MultipleWrite(page_num,FRAM_BUFFER_SIZE,pBuff);
	BSP_FM25CL64_MultipleRead(page_num,FRAM_BUFFER_SIZE,FRAM_rxbuffer);
}

void FRAM_Read(uint16_t page_num, uint8_t* pBuff,uint16_t buffSize)
{
	BSP_FM25CL64_MultipleRead(page_num,buffSize,pBuff);
	BSP_FM25CL64_MultipleRead(page_num,FRAM_BUFFER_SIZE,FRAM_rxbuffer);
}



void SaveParameter(sysVar* systemVariables,uint16_t offset,uint16_t size)
{
}

uint8_t BSP_FM25CL64_ReadState(void)
{
	int output;
	
	FRAM_tx[0]=FM25CL64_RDSR;
	FRAM_tx[1]=0x00;
	
	FRAM_CS_ON;
	HAL_SPI_TransmitReceive(&hspi4,FRAM_tx,FRAM_rx,2,100);
	FRAM_CS_OFF;
	
	return FRAM_rx[0];
}

void BSP_FM25CL64_WriteEnable(void)
{
	FRAM_tx[0]=FM25CL64_WREN;
	
	FRAM_CS_ON;
	HAL_SPI_Transmit(&hspi4,FRAM_tx,1,100);
	FRAM_CS_OFF;
	BSP_FM25CL64_ReadState();
}

void BSP_FM25CL64_WriteState(uint8_t data)
{
	BSP_FM25CL64_WriteEnable();
	FRAM_tx[0]=FM25CL64_WRSR;
	FRAM_tx[1]=data;
	
	FRAM_CS_ON;
	HAL_SPI_Transmit(&hspi4,FRAM_tx,2,100);
	FRAM_CS_OFF;
}

void BSP_FM25CL64_ProAll(void)
{
	BSP_FM25CL64_WriteState(FM25CL64_PROTECT);
}

void BSP_FM25CL64_UProAll(void)
{
	BSP_FM25CL64_WriteState(FM25CL64_UNPROTECT);
}

uint8_t BSP_FM25CL64_SingleRead(uint16_t address)
{
	uint8_t output;
	
	FRAM_tx[0]=FM25CL64_READ;
	FRAM_tx[1]=((address & 0xff00) >> 8);
	FRAM_tx[2]=(address & 0x00ff);
	FRAM_tx[3]=0x00;
	
	FRAM_CS_ON;
	HAL_SPI_TransmitReceive(&hspi4,FRAM_tx,FRAM_rx,4,100);
	FRAM_CS_OFF;
	
	return FRAM_rx[3];
}

void BSP_FM25CL64_MultipleRead(uint16_t address, uint16_t num, uint8_t *pointer)
{
	FRAM_tx[0]=FM25CL64_READ;
	FRAM_tx[1]=((address & 0xff00) >> 8);
	FRAM_tx[2]=(address & 0x00ff);
	
	FRAM_CS_ON;
	HAL_SPI_Transmit(&hspi4,FRAM_tx,3,100);
	HAL_SPI_Receive(&hspi4,pointer,num,100);
	FRAM_CS_OFF;
}

uint8_t BSP_FM25CL64_SingleWrite(uint16_t address, uint8_t value)
{
	BSP_FM25CL64_WriteEnable();
	
	FRAM_tx[0]=FM25CL64_WRITE;
	FRAM_tx[1]=((address & 0xff00) >> 8);
	FRAM_tx[2]=(address & 0x00ff);
	FRAM_tx[3]=value;
	
	FRAM_CS_ON;
	HAL_SPI_Transmit(&hspi4,FRAM_tx,4,100);
	FRAM_CS_OFF;
}

void BSP_FM25CL64_MultipleWrite(uint16_t address, uint16_t num, uint8_t *pointer)
{
	BSP_FM25CL64_WriteEnable();
	
	FRAM_tx[0]=FM25CL64_WRITE;
	FRAM_tx[1]=((address & 0xff00) >> 8);
	FRAM_tx[2]=(address & 0x00ff);
	
	FRAM_CS_ON;
	HAL_SPI_Transmit(&hspi4,FRAM_tx,3,100);
	HAL_SPI_Transmit(&hspi4,pointer,num,100);
	FRAM_CS_OFF;
}

void ReadFirmware(sysVar* systemVariables)
{
	uint8_t a = 0;
	while((SPI4->SR & (SPI_FLAG_TXE|SPI_FLAG_RXNE)) == 0);
	BSP_FM25CL64_MultipleRead(4000,1,(uint8_t*)&a);
	while((SPI4->SR & (SPI_FLAG_TXE|SPI_FLAG_RXNE)) == 0);
	
	if(a == 1)
		systemVariables->StayInBootloader = 1;
}



void WriteFirmware(sysVar* systemVariables)
{
	uint8_t a = 0;
	a = 0;
	while((SPI4->SR & (SPI_FLAG_TXE|SPI_FLAG_RXNE)) == 0);
	BSP_FM25CL64_MultipleWrite(4000,1,(uint8_t*)&a);
	while((SPI4->SR & (SPI_FLAG_TXE|SPI_FLAG_RXNE)) == 0);
}



void FirmwareReady(sysVar* systemVariables)
{
	uint8_t a = 0;
	a = 0;
	while((SPI4->SR & (SPI_FLAG_TXE|SPI_FLAG_RXNE)) == 0);
	BSP_FM25CL64_MultipleWrite(4000,1,(uint8_t*)&a);
	while((SPI4->SR & (SPI_FLAG_TXE|SPI_FLAG_RXNE)) == 0);
}

