#include "adc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

extern ADC_HandleTypeDef hadc3;
extern DMA_HandleTypeDef hdma_adc3;
extern TIM_HandleTypeDef htim6;
extern sysVar systemVariables;
extern IWDG_HandleTypeDef hiwdg1;


void ADC_Init(void)
{
	HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
	ADC_MultiModeTypeDef multimode = {0};
	ADC_ChannelConfTypeDef sConfig = {0};
	
	hadc3.Instance = ADC3;
	hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc3.Init.Resolution = ADC_RESOLUTION_16B;
	hadc3.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc3.Init.LowPowerAutoWait = DISABLE;
	hadc3.Init.ContinuousConvMode = DISABLE;
	hadc3.Init.NbrOfConversion = 5;
	hadc3.Init.DiscontinuousConvMode = DISABLE;
	hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T6_TRGO;
	hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc3.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
	hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc3.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
	
	hadc3.Init.OversamplingMode = DISABLE;
	hadc3.Init.BoostMode = DISABLE;
	if (HAL_ADC_Init(&hadc3) != HAL_OK)
	{
		Error_Handler();
	}
	
	multimode.Mode = ADC_MODE_INDEPENDENT;
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc3, &multimode) != HAL_OK)
	{
		Error_Handler();
	}
	
	sConfig.Channel = ADC_CHANNEL_3;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_16CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	
	sConfig.Channel = ADC_CHANNEL_7;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = ADC_REGULAR_RANK_3;
	if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	
	sConfig.Channel = ADC_CHANNEL_6;
	sConfig.Rank = ADC_REGULAR_RANK_4;
	if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_REGULAR_RANK_5;
	if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	
	TIM6_Init();
	HAL_TIM_Base_Start(&htim6);
	HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
	////ppppHAL_ADC_Start_DMA(&hadc3, (uint32_t*)ADCBuff, ADC_BUFF_UPDATE);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	if(adcHandle->Instance==ADC3)
	{
		__HAL_RCC_ADC3_CLK_ENABLE();
		__HAL_RCC_GPIOF_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		/**ADC1 GPIO Configuration
		PF7     ------> ADC3_INP3
		PF8     ------> ADC3_INP7
		PF9     ------> ADC3_INP2
		PF10    ------> ADC3_INP6
		PC2     ------> ADC3_INP0
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		/* ADC3 DMA Init */
		/* ADC3 Init */
		hdma_adc3.Instance = DMA1_Stream2;
		hdma_adc3.Init.Request = DMA_REQUEST_ADC3;
		hdma_adc3.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc3.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc3.Init.MemInc = DMA_MINC_ENABLE;
		hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_adc3.Init.Mode = DMA_CIRCULAR;
		hdma_adc3.Init.Priority = DMA_PRIORITY_MEDIUM;
		hdma_adc3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		if (HAL_DMA_Init(&hdma_adc3) != HAL_OK)
		{
			Error_Handler();
		}
		__HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc3);
	}
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

	if(adcHandle->Instance==ADC3)
	{
		__HAL_RCC_ADC3_CLK_DISABLE();
		HAL_GPIO_DeInit(GPIOF, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);
		HAL_GPIO_DeInit(GPIOC, GPIO_PIN_2);
		HAL_DMA_DeInit(adcHandle->DMA_Handle);
	}
}

void TIM6_Init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 2 - 1;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 250 - 1;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
	{
		while(1);
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
	{
		while(1);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static bool led = 0;
	if(htim->Instance == TIM6)
	{
	}
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	////ppppHAL_ADC_Start_DMA(&hadc3,(uint32_t*)ADCBuff,ADC_BUFF_UPDATE);
	
	if(systemVariables.systemInitialOK)
	{
		UIADCChannel_t PhasorSelection;

		for(int i = 1; i <= ADC_CHANNEL_UPDATE; i++)
		{
			////ppppsystemVariables.InputBuffPE1[ADC_CHANNEL_UPDATE - i]               = ADCBuff[(5*(i - 1))    ];
			////ppppsystemVariables.InputBuffPE2[ADC_CHANNEL_UPDATE - i]               = ADCBuff[(5*(i - 1)) + 1];
			////ppppsystemVariables.InputBuffAmbiantTemprature[ADC_CHANNEL_UPDATE - i] = ADCBuff[(5*(i - 1)) + 2];
			////ppppsystemVariables.InputBuffCurrent[ADC_CHANNEL_UPDATE - i]           = ADCBuff[(5*(i - 1)) + 3];
			////ppppsystemVariables.InputBuffVoltage[ADC_CHANNEL_UPDATE - i]           = ADCBuff[(5*(i - 1)) + 4];
			////ppppsystemVariables.LM35MeanBuff += (float)systemVariables.InputBuffAmbiantTemprature[i] / ADC_BUFF;
			systemVariables.LM35DataCounter++;
			if(systemVariables.LM35DataCounter >= ADC_BUFF)
			{
				systemVariables.AmbiantTemprature = (0.9 * systemVariables.AmbiantTemprature) + (0.1 * (3300.0f * (float)(systemVariables.LM35MeanBuff) / 65536.0f) / 10.0f);
				systemVariables.LM35MeanBuff = 0;
				systemVariables.LM35DataCounter = 0;
			}
		}
		
		WRITEPIN(UI_LED1,1);
		OLProtection(&systemVariables);
		OVProtection(&systemVariables);
		UVProtection(&systemVariables);
		OCProtection(&systemVariables);
		EarthProtection(&systemVariables);
		
		MX_LWIP_Process();
		EthernetHandler();
		
		WRITEPIN(UI_LED1,0);
	}
	
	if(!READPIN(SAFETY_STATUS))
	{
		WRITEPIN(RELAY_MAIN,0);
		systemVariables.DataBufferIndex = 0;
	}
	
	if(systemVariables.DataReady == 1)
	{
		systemVariables.DataReady = 0;
		systemVariables.MeanIndex++;
		if(systemVariables.MeanIndex >= 300)
		{
			systemVariables.MeanIndex = 0;
			systemVariables.MeanRef = systemVariables.SumRef / (300 * COM_DATA_SIZE);
			systemVariables.MeanMeas = systemVariables.SumMeas / (300 * COM_DATA_SIZE);
			systemVariables.SumRef = 0;
			systemVariables.SumMeas = 0;
		}
	}
	
	
	systemVariables.MiliSecond++;
	if(systemVariables.MiliSecond == 500)
	{
		WRITEPIN(PANEL_LED_HEALTHY,1);
		WRITEPIN(UI_LED3,1);
	}
	////ppppif(systemVariables.MiliSecond == 800)
	////pppp	WRITEPIN(UI_LED1,1);
	else if(systemVariables.MiliSecond > 1000)
	{
		systemVariables.ReadRTD = 1;
		WRITEPIN(PANEL_LED_HEALTHY,0);
		WRITEPIN(UI_LED3,0);
		////ppppWRITEPIN(UI_LED1,0);
		systemVariables.MiliSecond = 0;
	}
	HAL_IWDG_Refresh(&hiwdg1);
	
}

void OLProtection(sysVar* systemVariables)
{
	#define TIME_STEP 0.001f
	float M;
	float I_amp_pu;
	
	I_amp_pu = systemVariables->Local.TransCurrent.Value / ((float)systemVariables->Setting.OLPickup); //%Normalized based on the rated current
	M = (I_amp_pu) / ((float)systemVariables->Setting.OLServiceFactor);
	systemVariables->Local.OLTripCounter = (M*M * TIME_STEP)/(systemVariables->Setting.OLTauHot + TIME_STEP) + systemVariables->Setting.OLTauHot/(systemVariables->Setting.OLTauHot+ TIME_STEP) * systemVariables->Local.OLTripCounter;
	
	if (systemVariables->Local.OLTripCounter >= systemVariables->Setting.OLTripTemp)
		systemVariables->ErrorFlag.OLTrip = 0; ////pppp 1 beshe
	else if (systemVariables->Local.OLTripCounter <= systemVariables->Setting.OLTripTemp * RESETFACTOR)
		systemVariables->ErrorFlag.OLTrip = 0;
	if (systemVariables->Local.OLTripCounter >= systemVariables->Setting.OLStartTemp)
		systemVariables->ErrorFlag.OLStart = 1;
	else if (systemVariables->Local.OLTripCounter <= systemVariables->Setting.OLStartTemp * RESETFACTOR)
		systemVariables->ErrorFlag.OLStart = 0;
	
	if(systemVariables->Local.OLTripCounter >= 5.0f)
		systemVariables->Local.OLTripCounter = 5.0f;
}

void OVProtection(sysVar* systemVariables)
{
	static float ResetCounter = 0.0f;
	static float TripCounter = 0.0f;
	float m_fResetTime = RESETTIMEDEFINITE;
	float M;
	M = (systemVariables->Local.TransVoltage.Value) / (systemVariables->Setting.OVPickup * (float)systemVariables->Setting.TransPrimaryVoltage);
	
	if(M >= PICKUPFACTOR)
	{
		systemVariables->ErrorFlag.OverVoltageStart = 1;
		ResetCounter = 0.0f;
		
		if(TripCounter < 1.0f) 
			TripCounter = TripCounter + (1.0f / systemVariables->Setting.OVDelay);
		if(TripCounter >= 1.0f)
		{
			TripCounter = 1.0f;
			systemVariables->ErrorFlag.OverVoltageTrip = 1;
		}
	}
	
	else if((M < RESETFACTOR) && (TripCounter > 0))
	{
		systemVariables->ErrorFlag.OverVoltageStart = 0;
		ResetCounter += 1.0f;
		
		if (ResetCounter >= (float)(m_fResetTime))
		{
			TripCounter = 0;
			ResetCounter = 0.0f;
			systemVariables->ErrorFlag.OverVoltageTrip = 0;
		}
	}
}

void UVProtection(sysVar* systemVariables)
{
	float m_fResetValue = 0;
	static float  TripCounter = 0.0f;
	static float ResetCounter = 0.0f;
	static float ResetCounterBlock = 0.0f;
	float M;
	
	float m_fResetTime = RESETTIMEDEFINITE;
	float Vblock = 1;
	Vblock = (float)systemVariables->Setting.UVBlock * (float)systemVariables->Setting.TransPrimaryVoltage;
	
	if(systemVariables->Local.TransVoltage.Value > Vblock)
	{
		M = (systemVariables->Local.TransVoltage.Value) / ((float)systemVariables->Setting.UVPickup * (float)systemVariables->Setting.TransPrimaryVoltage);
		
		if(M <= PICKUPFACTOR_UV)
		{
			if(TripCounter > UNDER_INITIAL_BLOCK)
				systemVariables->ErrorFlag.UnderVoltageStart = 1;
			
			ResetCounter = 0.0f;
			if(TripCounter < 1.0f)
				TripCounter = TripCounter + (1.0f / (float)systemVariables->Setting.UVDelay);
			if(TripCounter >= 1.0f)
			{
				TripCounter = 1.0f;
				systemVariables->ErrorFlag.UnderVoltageTrip = 1;
			}
		}
		else if((M > RESETFACTOR_UV) && (TripCounter > 0))
		{
			systemVariables->ErrorFlag.UnderVoltageStart = 0;
			ResetCounter = ResetCounter + 1.0f;
			if(ResetCounter >= (float)(m_fResetTime))
			{
				TripCounter = 0;
				ResetCounter = 0.0f;
				systemVariables->ErrorFlag.UnderVoltageTrip = 0;
			}
		}
		ResetCounterBlock = 0;
	}
	else
	{
		ResetCounterBlock++;
		if(ResetCounterBlock == 30.0f)
		{
			systemVariables->ErrorFlag.UnderVoltageTrip  = 0;
			systemVariables->ErrorFlag.UnderVoltageStart = 0;
			ResetCounterBlock = 0.0f;
			TripCounter = 0;
			ResetCounter = 0.0f;
		}
	}
}

void OCProtection(sysVar* systemVariables)
{
	static float ResetCounter = 0.0f;
	static float TripCounter = 0.0f;
	float m_fResetTime = RESETTIMEDEFINITE;
	float M;
	M = (systemVariables->Local.TransCurrent.Value) / (systemVariables->Setting.OCPickup * (float)systemVariables->Setting.TransPrimaryCurrent);
	
	if(M >= PICKUPFACTOR)
	{
		systemVariables->ErrorFlag.OverCurrentStart = 1;
		ResetCounter = 0.0f;
		
		if(TripCounter < 1.0f) 
			TripCounter = TripCounter + (1.0f / systemVariables->Setting.OCDelay);
		if(TripCounter >= 1.0f)
		{
			TripCounter = 1.0f;
			systemVariables->ErrorFlag.OverCurrentTrip = 1;
		}
	}
	
	else if((M < RESETFACTOR) && (TripCounter > 0))
	{
		systemVariables->ErrorFlag.OverCurrentStart = 0;
		ResetCounter += 1.0f;
		
		if(ResetCounter >= (float)(m_fResetTime))
		{
			TripCounter = 0;
			ResetCounter = 0.0f;
			systemVariables->ErrorFlag.OverCurrentTrip = 0;
		}
	}
}

void EarthProtection(sysVar* systemVariables)
{
	static float ResetCounter = 0.0f;
	static float TripCounter = 0.0f;
	float m_fResetTime = RESETTIMEDEFINITE;
	float M;
	float Vsupply;
	Vsupply = systemVariables->Local.VPE1.Value + systemVariables->Local.VPE2.Value;
	if(systemVariables->Local.VPE1.Value > (Vsupply / 2))
		M = Vsupply - systemVariables->Local.VPE1.Value;
	else
		M = systemVariables->Local.VPE1.Value;
	if(M >= systemVariables->Local.EarthVoltageLimit)
	{
		systemVariables->ErrorFlag.NoEarth = 0; ////pppp 1 bayad beshe
		ResetCounter = RESETTIMEDEFINITE;
	}
	else if((M < systemVariables->Local.EarthVoltageLimit) && (ResetCounter <= 0))
	{
		systemVariables->ErrorFlag.NoEarth = 0;
		ResetCounter = 0;
	}
	else
		ResetCounter--;
	
	
	if(systemVariables->Local.VPE2.Value > (Vsupply / 2))
		M = Vsupply - systemVariables->Local.VPE2.Value;
	else
		M = systemVariables->Local.VPE2.Value;
	if(M >= systemVariables->Local.EarthVoltageLimit)
	{
		systemVariables->ErrorFlag.NoEarth = 0; ////pppp 1 bayad beshe
		ResetCounter = RESETTIMEDEFINITE;
	}
	else if((M < systemVariables->Local.EarthVoltageLimit) && (ResetCounter <= 0))
	{
		systemVariables->ErrorFlag.NoEarth = 0;
		ResetCounter = 0;
	}
	else
		ResetCounter--;
}

bool ADCCalibration(sysVar* systemVariables)
{
	static double TotalAmpl;
	static double x1;
	double Ampl = 0,A = 0;
	static uint16_t CalibrationNumber = 0;
	
	
	TotalAmpl += Ampl/CALIBRATION_NUM;
	CalibrationNumber++;
	if(CalibrationNumber >= CALIBRATION_NUM)
	{
		switch(systemVariables->UIADCChannel)
		{
			case ADC_I_TR:
			{
				if(systemVariables->StartCalibration == 1)
					x1 = TotalAmpl;
				else
				{
					systemVariables->UIADCCoef.InputCurrentCoef = (10.0 - 1) / (TotalAmpl - x1);
					systemVariables->UIADCCoef.InputCurrentOffset = (((10.0 + 1) / systemVariables->UIADCCoef.InputCurrentCoef) - TotalAmpl - x1) / 2;
				}
				break;
			}
			
			case ADC_V_TR:
			{
				if(systemVariables->StartCalibration == 1)
					x1 = TotalAmpl;
				else
				{
					systemVariables->UIADCCoef.InputVoltageCoef = (100.0 - 10) / (TotalAmpl - x1);
					systemVariables->UIADCCoef.InputVoltageOffset = (((100.0 + 10) / systemVariables->UIADCCoef.InputVoltageCoef) - TotalAmpl - x1) / 2;
				}
				break;
			}
			
			case ADC_VPE1:
			{
				if(systemVariables->StartCalibration == 1)
					x1 = TotalAmpl;
				else
				{
					systemVariables->UIADCCoef.InputVPE1Coef = (100.0 - 10) / (TotalAmpl - x1);
					systemVariables->UIADCCoef.InputVPE1Offset = (((100.0 + 10) / systemVariables->UIADCCoef.InputVPE1Coef) - TotalAmpl - x1) / 2;
				}
				break;
			}
			
			case ADC_VPE2:
			{
				if(systemVariables->StartCalibration == 1)
					x1 = TotalAmpl;
				else
				{
					systemVariables->UIADCCoef.InputVPE2Coef = (100.0 - 10) / (TotalAmpl - x1);
					systemVariables->UIADCCoef.InputVPE2Offset = (((100.0 + 10) / systemVariables->UIADCCoef.InputVPE2Coef) - TotalAmpl - x1) / 2;
				}
				break;
			}
		}
		systemVariables->StartCalibration = 0;
		TotalAmpl = 0;
		CalibrationNumber = 0;
	}
	return 1;
}
