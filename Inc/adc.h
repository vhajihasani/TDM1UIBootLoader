#ifndef __adc_H
#define __adc_H

#include "main.h"
#include "stm32h7xx_hal.h"
#include "GPIODefine.h"
#include "systemVariables.h"
#include "math.h"


#include "lwip.h"
#include "ethernet_handler.h"

extern ADC_HandleTypeDef hadc3;
void ADC_Init(void);
void TIM6_Init(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
bool ADCCalibration(sysVar* systemVariables);
void OLProtection(sysVar* systemVariables);
void OVProtection(sysVar* systemVariables);
void UVProtection(sysVar* systemVariables);
void OCProtection(sysVar* systemVariables);
void EarthProtection(sysVar* systemVariables);
#endif
