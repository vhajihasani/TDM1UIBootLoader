#include "systemVariables.h"
#include "FRAM.h"
CRC_HandleTypeDef hcrc;
IWDG_HandleTypeDef hiwdg1;
static uint8_t tryCounter = 0;
/* IWDG1 init function */
void MX_IWDG1_Init(void)
{
	hiwdg1.Instance = IWDG1;
	hiwdg1.Init.Prescaler = IWDG_PRESCALER_32;
	hiwdg1.Init.Window = 4095;
	hiwdg1.Init.Reload = 2000;
	if (HAL_IWDG_Init(&hiwdg1) != HAL_OK)
	{
		Error_Handler();
	}
	__HAL_IWDG_START(&hiwdg1);
}


void InitSystem(sysVar* systemVariables)
{
	
	systemVariables->CalculationSeq = CALC_IDLE;
	systemVariables->DFTCycle = 60;
	systemVariables->Local.EarthVoltageLimit = 50;
	
	systemVariables->systemInitialOK = 1;
}



float RMSCalc(uint32_t* window, uint16_t windowLen,float Coef)
{
	float sumRMS=0.0f;
	float SignalRMS = 0;
	for(uint16_t i=0; i<windowLen ; i++)
	{
		SignalRMS = (float)window[i] * Coef;
		sumRMS += (SignalRMS * SignalRMS);
	}
	sumRMS=sqrtf(sumRMS/windowLen);
	return sumRMS;
}




static void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct;
	HAL_MPU_Disable();
	
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x24048000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x30040000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER1;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x30044000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER2;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

static void MX_CRC_Init(void)
{
	hcrc.Instance = CRC;
	hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
	hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
	hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
	hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
	hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_WORDS;
	if(HAL_CRC_Init(&hcrc) != HAL_OK)
	{
		Error_Handler();
	}
}


void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	
	while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) 
	{
		
	}
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 400;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLFRACN = 0;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
	
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

void HardwareInit(sysVar* systemVariables)
{
	MPU_Config();
	SCB_EnableICache();
	SCB_EnableDCache();
	HAL_Init();
	SystemClock_Config();
	MX_CRC_Init();
	////pppp__HAL_RCC_D2SRAM3_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();
	GPIO_Init();
	HAL_GPIO_WritePin(ETH_RST,0);
	HAL_Delay(10);
	HAL_GPIO_WritePin(ETH_RST,1);
	WRITEPIN(UI_IN_BOOTLOADER,1);
	
}


uint8_t AES_KEY[] = { 0x4D, 0x61, 0x73, 0x74, 0x65, 0x72, 0x69, 0x6E, 0x67, 0x20, 0x20, 0x53, 0x54, 0x4D, 0x33, 0x32 };
#define APP_START_SECTOR 1
#define SECTORS_NUMBER 7


void cmd_erase() 
{
	HAL_StatusTypeDef status = HAL_ERROR;
	uint8_t state = 0;
	FLASH_EraseInitTypeDef eraseInfo;
	uint32_t ulBadBlocks = 0;
	eraseInfo.VoltageRange = FLASH_VOLTAGE_RANGE_4;
	eraseInfo.Sector = APP_START_SECTOR;
	eraseInfo.NbSectors = SECTORS_NUMBER;
	eraseInfo.TypeErase = FLASH_TYPEERASE_SECTORS;
	eraseInfo.Banks = 1;
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
	FLASH_WaitForLastOperation((uint32_t)50000, 1);
	HAL_FLASH_Unlock();
	for(int i = APP_START_SECTOR ; i < SECTORS_NUMBER; i++)
	{
		FLASH_Erase_Sector(i,1,FLASH_VOLTAGE_RANGE_4);
		FLASH_WaitForLastOperation((uint32_t)50000, 1);
		CLEAR_BIT(FLASH->CR1 , FLASH_CR_SER);
	}
	status = HAL_OK;
	if(status == HAL_OK && ulBadBlocks == 0xffffffff)
		state = 1;
	HAL_Delay(3000);
	HAL_FLASH_Lock();
	
}

//__________________________________________________________________________________________________________________________________________
//
//                                                						cmd_write
//__________________________________________________________________________________________________________________________________________

/* 
   cmd format : cmdID(data_addr(4bytes),data(16bytes)
*/
 

uint8_t cmd_write(sysVar* systemVariables)
{
	HAL_StatusTypeDef status = HAL_ERROR;
	uint32_t data_addr = 0;
	uint8_t datatemp[16];
	uint8_t data[32];
	uint8_t tryCounter = 0;
	data_addr = systemVariables->FlashAddr;
	for (uint8_t i = 0; i < 16; i++)
		datatemp[i] = systemVariables->FlashData[i];
	aes_enc_dec((uint8_t*)datatemp, AES_KEY, 1);
	for (uint8_t i = 0; i < 16; i++)
		systemVariables->FlashData[i] = datatemp[i];
	
	for (uint8_t i = 0; i < 16; i++)
		datatemp[i] = systemVariables->FlashData[i + 16];
	aes_enc_dec((uint8_t*)datatemp, AES_KEY, 1);
	if(systemVariables->LastData == 1)
	{
		for (uint8_t i = 0; i < 16; i++)
			systemVariables->FlashData[i + 16] = 0xff;
	}
	else
	{
		for (uint8_t i = 0; i < 16; i++)
			systemVariables->FlashData[i + 16] = datatemp[i];
	}
	
	if (systemVariables->FlashAddr >= APP_START_ADDRESS) 
	{
		
		if(systemVariables->FlashAddr >= 0x803bb80) 
			__NOP();
		
		
		for (uint8_t i = 0; i < 32; i++)
			data[i] = systemVariables->FlashData[i];
		
		
		data_addr = systemVariables->FlashAddr;
		while(tryCounter < 5)
		{
			__disable_irq();
			HAL_FLASH_Unlock();
			__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
			FLASH_WaitForLastOperation((uint32_t)50000, 1);
			status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, systemVariables->FlashAddr, ((uint32_t)systemVariables->FlashData));
			HAL_FLASH_Lock();
			if(status != HAL_OK)
			{
				HAL_FLASH_Unlock();
				__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR);
				HAL_FLASH_Lock();
				if(tryCounter > 2)
					HAL_Delay(10);
			}
			__enable_irq();
			if(status == HAL_OK)
				break;
			HAL_Delay(10);
			
			tryCounter++;
		}
		if(tryCounter >= 5)
			return 0;
		else
			return 1;
	}
	else
	{
		//khata
		return 0;
	}
}


typedef void (application_t) (void);

typedef struct
{
  uint32_t stackAddr;
  application_t *func;
} __vector;

typedef __vector* vector_t;



void jump_to_app()
{
	
	const vector_t vector = (vector_t)APP_START_ADDRESS;
	SCB_DisableICache();
	SCB_DisableDCache();
	HAL_MPU_Disable();
	__disable_irq();
	HAL_DeInit();
	
	
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
	
	HAL_RCC_DeInit();
	
	for (uint8_t i = 0 ; i < 7 ; i++)
	{
		NVIC->ICER[i] = 0xFFFFFFFF;
		NVIC->ICPR[i] = 0xFFFFFFFF;
	}
	
	__set_MSP(vector->stackAddr);
	SCB->VTOR = (uint32_t) APP_START_ADDRESS;
	
	__enable_irq();
	
	vector->func();
	
}


void jump_to_boot()
{
	#define BOOT_START_ADDRESS 0x08000000
	
	const vector_t vector = (vector_t)BOOT_START_ADDRESS;
	SCB_DisableICache();
	SCB_DisableDCache();
	HAL_MPU_Disable();
	__disable_irq();
	HAL_DeInit();
	
	
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
	
	HAL_RCC_DeInit();
	
	for (uint8_t i = 0 ; i < 7 ; i++)
	{
		NVIC->ICER[i] = 0xFFFFFFFF;
		NVIC->ICPR[i] = 0xFFFFFFFF;
	}
	
	__set_MSP(vector->stackAddr);
	SCB->VTOR = (uint32_t) BOOT_START_ADDRESS;
	
	__enable_irq();
	
	vector->func();
	
}

void LEDBlinker(sysVar* systemVariables)
{
	WRITEPIN(PANEL_LED_ERR,1);
	WRITEPIN(PANEL_LED_FAULT,1);
	WRITEPIN(PANEL_LED_HEALTHY,1);
	WRITEPIN(PANEL_LED_HV,1);
	HAL_Delay(500);
	WRITEPIN(PANEL_LED_ERR,0);
	WRITEPIN(PANEL_LED_FAULT,0);
	WRITEPIN(PANEL_LED_HEALTHY,0);
	WRITEPIN(PANEL_LED_HV,0);
	HAL_Delay(500);
	WRITEPIN(PANEL_LED_ERR,1);
	WRITEPIN(PANEL_LED_FAULT,1);
	WRITEPIN(PANEL_LED_HEALTHY,1);
	WRITEPIN(PANEL_LED_HV,1);
	HAL_Delay(500);
	WRITEPIN(PANEL_LED_ERR,0);
	WRITEPIN(PANEL_LED_FAULT,0);
	WRITEPIN(PANEL_LED_HEALTHY,0);
	WRITEPIN(PANEL_LED_HV,0);
}

void BootloaderHandler(sysVar* systemVariables)
{
	switch(systemVariables->BootloaderState)
	{
		case BL_IDLE:
		{
			if(systemVariables->MiliSecond % 250 < 125)
				WRITEPIN(PANEL_LED_HEALTHY,1);
			else
				WRITEPIN(PANEL_LED_HEALTHY,0);
			break;
		}
		case BL_ERASE_ADC:
		{
			WRITEPIN(PANEL_LED_HEALTHY,0);
			
			
			if(systemVariables->MiliSecond < 200)
				WRITEPIN(PANEL_LED_FAULT,1);
			else
				WRITEPIN(PANEL_LED_FAULT,0);
			
			systemVariables->CommandStr.EraseADC = 1;
			if(systemVariables->DataStr.ErasingADC)
				systemVariables->BootloaderState = BL_WAIT_FOR_ERASE_ADC;
			else if(systemVariables->DataStr.EraseADCCompleted)
			{
				systemVariables->CommandStr.EraseADC = 0;
				systemVariables->ETHConf.ETHState = ETH_ERASE_OK;
				systemVariables->BootloaderState = BL_IDLE;
			}
			break;
		}
		
		
		
		case BL_WAIT_FOR_ERASE_ADC:
		{
			WRITEPIN(PANEL_LED_HEALTHY,0);
			systemVariables->CommandStr.EraseADC = 0;
			if(systemVariables->MiliSecond < 200)
				WRITEPIN(PANEL_LED_FAULT,1);
			else
				WRITEPIN(PANEL_LED_FAULT,0);
			
			if(systemVariables->DataStr.EraseADCCompleted)
			{
				systemVariables->ETHConf.ETHState = ETH_ERASE_OK;
				systemVariables->BootloaderState = BL_IDLE;
			}
			break;
		}
		
		
		
		case BL_WRITE_ADC:
		{
			uint8_t datatemp[16];
			
			for (uint8_t i = 0; i < 16; i++)
				datatemp[i] = systemVariables->CommandStr.FlashData[i];
			aes_enc_dec((uint8_t*)datatemp, AES_KEY, 1);
			for (uint8_t i = 0; i < 16; i++)
				systemVariables->CommandStr.FlashData[i] = datatemp[i];
			
			for (uint8_t i = 0; i < 16; i++)
				datatemp[i] = systemVariables->CommandStr.FlashData[i + 16];
			aes_enc_dec((uint8_t*)datatemp, AES_KEY, 1);
			
			
			
			
			
			
			if(systemVariables->LastData == 1)
			{
				for (uint8_t i = 0; i < 16; i++)
					systemVariables->CommandStr.FlashData[i + 16] = 0xff;
			}
			else
			{
				for (uint8_t i = 0; i < 16; i++)
					systemVariables->CommandStr.FlashData[i + 16] = datatemp[i];
			}
			
			
			
			
			
			
			
			WRITEPIN(PANEL_LED_HEALTHY,0);
			if(systemVariables->MiliSecond < 200)
				WRITEPIN(PANEL_LED_HV,1);
			else
				WRITEPIN(PANEL_LED_HV,0);
			systemVariables->CommandStr.WriteFlash = 1;
			systemVariables->BootloaderState = BL_WAIT_FOR_WRITE_ADC;
			break;
		}
		
		
		
		case BL_WAIT_FOR_WRITE_ADC:
		{
			WRITEPIN(PANEL_LED_HEALTHY,0);
			if(systemVariables->MiliSecond < 200)
				WRITEPIN(PANEL_LED_HV,1);
			else
				WRITEPIN(PANEL_LED_HV,0);
			
			if(systemVariables->DataStr.WriteADCFlashCompleted)
			{
				systemVariables->CommandStr.WriteFlash = 0;
				systemVariables->BootloaderState = BL_WRITE_ADC_DONE;
			}
			break;
		}
		
		
		
		case BL_WRITE_ADC_DONE:
		{
			WRITEPIN(PANEL_LED_HEALTHY,0);
			WRITEPIN(PANEL_LED_HV,1);
			if(systemVariables->DataStr.WriteADCFlashCompleted == 0)
			{
				systemVariables->ETHConf.ETHState = ETH_WRITE_OK;
				systemVariables->BootloaderState = BL_IDLE;
			}
			break;
		}
		
		
		
		case BL_ERASE_UI:
		{
			WRITEPIN(PANEL_LED_HEALTHY,1);
			WRITEPIN(PANEL_LED_FAULT,1);
			
			
			cmd_erase();
			systemVariables->BootloaderState = BL_WAIT_FOR_ERASE_UI;
			break;
		}
		
		
		
		case BL_WAIT_FOR_ERASE_UI:
		{
			WRITEPIN(PANEL_LED_HEALTHY,0);
			WRITEPIN(PANEL_LED_FAULT,0);
			HAL_Delay(1000);
			
			uint8_t a = 0;
			a = *(uint8_t*)(APP_START_ADDRESS);
			if(a == 0xFF)
			{
				systemVariables->tryCounter = 0;
				systemVariables->ETHConf.ETHState = ETH_ERASE_OK;
				systemVariables->BootloaderState = BL_IDLE;
				
			}
			else if(systemVariables->tryCounter < 5)
			{
				systemVariables->tryCounter++;
				systemVariables->BootloaderState = BL_ERASE_UI;
			}
			else
			{
				systemVariables->tryCounter = 0;
				systemVariables->BLError = BL_ERR_UIERASE;
				systemVariables->BootloaderState = BL_IDLE;
			}
			break;
		}
		
		
		
		
		case BL_WRITE_UI:
		{
			WRITEPIN(PANEL_LED_HEALTHY,1);
			WRITEPIN(PANEL_LED_HV,1);
			
			
			
			if(cmd_write(systemVariables))
			{
				WRITEPIN(PANEL_LED_HEALTHY,0);
				WRITEPIN(PANEL_LED_HV,0);
				systemVariables->ETHConf.ETHState = ETH_WRITE_OK;
				systemVariables->BootloaderState = BL_IDLE;
			}
			else
			{
				systemVariables->BLError = BL_ERR_UIWRITE;
				systemVariables->ErrorAddress = 0;////pppp
				systemVariables->tryCounter = 0;
				systemVariables->BootloaderState = BL_IDLE;
			}
			break;
		}
		
		
		
		case BL_JUMP_TO_APP:
		{
			WriteFirmware(systemVariables);
			ReadFirmware(systemVariables);
			if(systemVariables->StayInBootloader == 1)
			{
				WRITEPIN(PANEL_LED_ERR,1);
				WRITEPIN(PANEL_LED_FAULT,1);
				WRITEPIN(PANEL_LED_HEALTHY,1);
				WRITEPIN(PANEL_LED_HV,1);
				HAL_Delay(1500);
			}
			WRITEPIN(ADC_NEED_RESET,1);
			WRITEPIN(STAY_IN_BOOTLOADER,0);
			while(READPIN(ADC_READY));
			HAL_NVIC_SystemReset();
			////ppppjump_to_boot();
			break;
		}
		
	}
}

