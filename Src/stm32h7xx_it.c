#include "main.h"
#include "stm32h7xx_it.h"
#include "systemVariables.h"
#include "GPIODefine.h"
#include "lwip.h"
#include "ethernet_handler.h"
extern CRC_HandleTypeDef hcrc;
extern uint8_t RxData[COM_BUFFER_SIZE] __attribute__((at(0x24048000)));
extern uint8_t TxData[COM_BUFFER_SIZE] __attribute__((at(0x2404A000)));
extern double Data[2][DATASIZE] __attribute__((at(0xD0000000)));
extern TIM_HandleTypeDef htim6;
extern ETH_HandleTypeDef heth;
extern SPI_HandleTypeDef hspi2;
extern ADC_HandleTypeDef hadc3;
extern DMA_HandleTypeDef hdma_adc3;
extern sysVar systemVariables;
double globaldouble1[10];
double globaldouble2[10];
double globaldouble3[10];

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
	WRITEPIN(RELAY_MAIN,0);
	while (1)
	{
	}
}

void MemManage_Handler(void)
{
	while (1)
	{
	}
}

void BusFault_Handler(void)
{
	while (1)
	{
	}
}

void UsageFault_Handler(void)
{
	while (1)
	{
	}
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
	HAL_IncTick();
	if(systemVariables.Eth_Enable == 1)
	{
		MX_LWIP_Process();
		EthernetHandler();
	}
	
	systemVariables.MiliSecond++;
	if(systemVariables.MiliSecond > 1000)
		systemVariables.MiliSecond = 0;
}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
}

void ETH_IRQHandler(void)
{
	HAL_ETH_IRQHandler(&heth);
}

void DMA1_Stream2_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_adc3);
}

void TIM6_DAC_IRQHandler(void)
{
	
	HAL_TIM_IRQHandler(&htim6);
}

void ADC_IRQHandler(void)
{
	HAL_ADC_IRQHandler(&hadc3);
}

void DMA1_Stream1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(hspi2.hdmatx);
}

void DMA1_Stream0_IRQHandler(void)
{
	HAL_DMA_IRQHandler(hspi2.hdmarx);
}

void SPI2_IRQHandler(void)
{
	uint32_t CRCRec;
	uint32_t CRCCalc;
	HAL_SPI_IRQHandler(&hspi2);
}


void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	uint32_t CRCRec;
	uint32_t CRCCalc;
	uint32_t CRCSend;
	if(hspi->Instance == SPI2)
	{
		CRCCalc = HAL_CRC_Calculate(&hcrc,(uint32_t*)&RxData,(COM_BUFFER_SIZE - 4)/4);
		CRCRec = (uint32_t)((uint32_t)RxData[COM_BUFFER_SIZE - 4] & 0x000000ff) | (uint32_t)(((uint32_t)RxData[COM_BUFFER_SIZE - 3] << 8) & 0x0000ff00) | (uint32_t)(((uint32_t)RxData[COM_BUFFER_SIZE - 2] << 16) & 0x00ff0000) | (uint32_t)((RxData[COM_BUFFER_SIZE - 1] << 24) & 0xff000000);
		
		if(CRCCalc == CRCRec)
		{
			WRITEPIN(UI_LED2,1);
			////ppppfor(int i = 0; i < sizeof(DataStr_t); i++)
			////pppp{
			////pppp	*(uint8_t*)(&systemVariables.DataStr + i) = RxData[i+1];
			////pppp}
			//SCB_InvalidateDCache();
			memcpy(&systemVariables.DataStr,&RxData[1],sizeof(DataStr_t));
			
			
			systemVariables.DataReady = 1;
			////ppppif(systemVariables.CommandStr.TestCondition.TestStart == 1)
			////pppp{
			////pppp	memcpy(&Data[0][systemVariables.DataBufferIndex],&systemVariables.DataStr.DataRef[0],8*COM_DATA_SIZE);
			////pppp	memcpy(&Data[1][systemVariables.DataBufferIndex],&systemVariables.DataStr.DataMeas[0],8*COM_DATA_SIZE);
			////pppp	
			////pppp	
			////pppp	if(systemVariables.CommandStr.alaki == 1)
			////pppp		systemVariables.DataBufferIndex = (systemVariables.DataBufferIndex + COM_DATA_SIZE);////pppp % DATASIZE;
			////pppp	else
			////pppp	systemVariables.DataBufferIndex = (systemVariables.DataBufferIndex + COM_DATA_SIZE) % DATASIZE;
			////pppp	if(systemVariables.DataBufferIndex >= DATASIZE)
			////pppp		systemVariables.CommandStr.TestCondition.TestStart = 0;
			////pppp	else if(systemVariables.CalculationSeq == CALC_IDLE && systemVariables.DataBufferIndex > systemVariables.CalculationSamples)////pppp inja bayad sharte test running ezafe she
			////pppp	{
			////pppp		systemVariables.EndCalcIndex = systemVariables.DataBufferIndex;
			////pppp		systemVariables.CalculationSeq = CALC_DATAREADY;
			////pppp	}
			////pppp}
			////ppppelse
			////pppp{
			////pppp	systemVariables.DataBufferIndex = 0;
			////pppp}
		}
		
		else
		{
			WRITEPIN(UI_LED2,0);
			systemVariables.RefMAXRANGE++;
		}
		
		memcpy(&TxData[1],&systemVariables.CommandStr,sizeof(CommandStr_t));
		CRCSend = HAL_CRC_Calculate(&hcrc,(uint32_t*)&TxData[0],(COM_BUFFER_SIZE - 4)/4);
		TxData[COM_BUFFER_SIZE - 4] = (CRCSend      ) & 0xff;
		TxData[COM_BUFFER_SIZE - 3] = (CRCSend >>  8) & 0xff;
		TxData[COM_BUFFER_SIZE - 2] = (CRCSend >> 16) & 0xff;
		TxData[COM_BUFFER_SIZE - 1] = (CRCSend >> 24) & 0xff;
	}
}
