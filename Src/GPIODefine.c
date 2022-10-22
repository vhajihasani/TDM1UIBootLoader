#include "GPIODefine.h"

void GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
	__HAL_RCC_GPIOJ_CLK_ENABLE();
	__HAL_RCC_GPIOK_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = ETH_RST_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(ETH_RST_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_ONBOARD_LED1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(UI_ONBOARD_LED1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_ONBOARD_LED2_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(UI_ONBOARD_LED2_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_LED1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(UI_LED1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_LED2_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(UI_LED2_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_LED3_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(UI_LED3_PORT, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.Pin = SAFETY_STATUS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(SAFETY_STATUS_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = RTD_CS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RTD_CS_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = RTD_DRDY_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(RTD_DRDY_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = RELAY_MAIN_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RELAY_MAIN_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = IN_B_STATUS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(IN_B_STATUS_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = IN_R_STATUS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(IN_R_STATUS_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(UI_COMM_IO1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO2_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(UI_COMM_IO2_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO3_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(UI_COMM_IO3_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO4_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(UI_COMM_IO4_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO5_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(UI_COMM_IO5_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO6_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(UI_COMM_IO6_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO7_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(UI_COMM_IO7_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO8_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(UI_COMM_IO8_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO9_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(UI_COMM_IO9_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO10_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(UI_COMM_IO10_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO11_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(UI_COMM_IO11_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO12_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(UI_COMM_IO12_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO13_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(UI_COMM_IO13_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO14_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(UI_COMM_IO14_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO15_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(UI_COMM_IO15_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO16_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(UI_COMM_IO16_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO17_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(UI_COMM_IO17_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO18_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(UI_COMM_IO18_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO19_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(UI_COMM_IO19_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = UI_COMM_IO20_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(UI_COMM_IO20_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = PANEL_LED_HEALTHY_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(PANEL_LED_HEALTHY_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = PANEL_LED_HV_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(PANEL_LED_HV_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = PANEL_LED_ERR_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(PANEL_LED_ERR_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = PANEL_LED_FAULT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(PANEL_LED_FAULT_PORT, &GPIO_InitStruct);
}