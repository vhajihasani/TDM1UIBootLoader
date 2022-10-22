#ifndef __GPIODEFINE_H__
#define __GPIODEFINE_H__

#include "stm32h7xx_hal.h"

#define UI_LED1_PORT            GPIOI
#define UI_LED1_PIN             GPIO_PIN_13
#define UI_LED1                 UI_LED1_PORT,UI_LED1_PIN

#define UI_LED2_PORT            GPIOI
#define UI_LED2_PIN             GPIO_PIN_12
#define UI_LED2                 UI_LED2_PORT,UI_LED2_PIN

#define UI_LED3_PORT            GPIOI
#define UI_LED3_PIN             GPIO_PIN_11
#define UI_LED3                 UI_LED3_PORT,UI_LED3_PIN

#define SAFETY_STATUS_PORT      GPIOJ
#define SAFETY_STATUS_PIN       GPIO_PIN_4
#define SAFETY_STATUS           SAFETY_STATUS_PORT,SAFETY_STATUS_PIN

#define RTD_CS_PORT             GPIOK
#define RTD_CS_PIN              GPIO_PIN_1
#define RTD_CS                  RTD_CS_PORT,RTD_CS_PIN

#define RTD_DRDY_PORT           GPIOH
#define RTD_DRDY_PIN            GPIO_PIN_10
#define RTD_DRDY                RTD_DRDY_PORT,RTD_DRDY_PIN

#define RELAY_MAIN_PORT         GPIOC
#define RELAY_MAIN_PIN          GPIO_PIN_3
#define RELAY_MAIN              RELAY_MAIN_PORT,RELAY_MAIN_PIN

#define IN_B_STATUS_PORT        GPIOJ
#define IN_B_STATUS_PIN         GPIO_PIN_0
#define IN_B_STATUS             IN_B_STATUS_PORT,IN_B_STATUS_PIN

#define IN_R_STATUS_PORT        GPIOJ
#define IN_R_STATUS_PIN         GPIO_PIN_2
#define IN_R_STATUS             IN_R_STATUS_PORT,IN_R_STATUS_PIN

#define UI_COMM_IO1_PORT        GPIOG
#define UI_COMM_IO1_PIN         GPIO_PIN_9
#define UI_COMM_IO1             UI_COMM_IO1_PORT,UI_COMM_IO1_PIN

#define UI_COMM_IO2_PORT        GPIOG
#define UI_COMM_IO2_PIN         GPIO_PIN_11
#define UI_COMM_IO2             UI_COMM_IO2_PORT,UI_COMM_IO2_PIN

#define UI_COMM_IO3_PORT        GPIOK
#define UI_COMM_IO3_PIN         GPIO_PIN_5
#define UI_COMM_IO3             UI_COMM_IO3_PORT,UI_COMM_IO3_PIN

#define UI_COMM_IO4_PORT        GPIOK
#define UI_COMM_IO4_PIN         GPIO_PIN_4
#define UI_COMM_IO4             UI_COMM_IO4_PORT,UI_COMM_IO4_PIN

#define UI_COMM_IO5_PORT        GPIOK
#define UI_COMM_IO5_PIN         GPIO_PIN_7
#define UI_COMM_IO5             UI_COMM_IO5_PORT,UI_COMM_IO5_PIN

#define UI_COMM_IO6_PORT        GPIOK
#define UI_COMM_IO6_PIN         GPIO_PIN_6
#define UI_COMM_IO6             UI_COMM_IO6_PORT,UI_COMM_IO6_PIN

#define UI_COMM_IO7_PORT        GPIOB
#define UI_COMM_IO7_PIN         GPIO_PIN_5
#define UI_COMM_IO7             UI_COMM_IO7_PORT,UI_COMM_IO7_PIN

#define UI_COMM_IO8_PORT        GPIOB
#define UI_COMM_IO8_PIN         GPIO_PIN_4
#define UI_COMM_IO8             UI_COMM_IO8_PORT,UI_COMM_IO8_PIN

#define UI_COMM_IO9_PORT        GPIOB
#define UI_COMM_IO9_PIN         GPIO_PIN_7
#define UI_COMM_IO9             UI_COMM_IO9_PORT,UI_COMM_IO9_PIN

#define UI_COMM_IO10_PORT       GPIOB
#define UI_COMM_IO10_PIN        GPIO_PIN_6
#define UI_COMM_IO10            UI_COMM_IO10_PORT,UI_COMM_IO10_PIN

#define UI_COMM_IO11_PORT       GPIOI
#define UI_COMM_IO11_PIN        GPIO_PIN_5
#define UI_COMM_IO11            UI_COMM_IO11_PORT,UI_COMM_IO11_PIN

#define UI_COMM_IO12_PORT       GPIOI
#define UI_COMM_IO12_PIN        GPIO_PIN_4
#define UI_COMM_IO12            UI_COMM_IO12_PORT,UI_COMM_IO12_PIN

#define UI_COMM_IO13_PORT       GPIOI
#define UI_COMM_IO13_PIN        GPIO_PIN_6
#define UI_COMM_IO13            UI_COMM_IO13_PORT,UI_COMM_IO13_PIN

#define UI_COMM_IO14_PORT       GPIOC
#define UI_COMM_IO14_PIN        GPIO_PIN_13
#define UI_COMM_IO14            UI_COMM_IO14_PORT,UI_COMM_IO14_PIN

#define UI_COMM_IO15_PORT       GPIOI
#define UI_COMM_IO15_PIN        GPIO_PIN_8
#define UI_COMM_IO15            UI_COMM_IO15_PORT,UI_COMM_IO15_PIN

#define UI_COMM_IO16_PORT       GPIOI
#define UI_COMM_IO16_PIN        GPIO_PIN_10
#define UI_COMM_IO16            UI_COMM_IO16_PORT,UI_COMM_IO16_PIN

#define UI_COMM_IO17_PORT       GPIOI
#define UI_COMM_IO17_PIN        GPIO_PIN_9
#define UI_COMM_IO17            UI_COMM_IO17_PORT,UI_COMM_IO17_PIN

#define UI_COMM_IO18_PORT       GPIOG
#define UI_COMM_IO18_PIN        GPIO_PIN_10
#define UI_COMM_IO18            UI_COMM_IO18_PORT,UI_COMM_IO18_PIN

#define UI_COMM_IO19_PORT       GPIOJ
#define UI_COMM_IO19_PIN        GPIO_PIN_14
#define UI_COMM_IO19            UI_COMM_IO19_PORT,UI_COMM_IO19_PIN

#define UI_COMM_IO20_PORT       GPIOJ
#define UI_COMM_IO20_PIN        GPIO_PIN_15
#define UI_COMM_IO20            UI_COMM_IO20_PORT,UI_COMM_IO20_PIN

#define ETH_RST_PORT            GPIOA
#define ETH_RST_PIN             GPIO_PIN_4
#define ETH_RST                 ETH_RST_PORT,ETH_RST_PIN


#define UI_ONBOARD_LED1_PORT    GPIOH
#define UI_ONBOARD_LED1_PIN     GPIO_PIN_3
#define UI_ONBOARD_LED1         UI_ONBOARD_LED1_PORT,UI_ONBOARD_LED1_PIN

#define UI_ONBOARD_LED2_PORT    GPIOA
#define UI_ONBOARD_LED2_PIN     GPIO_PIN_6
#define UI_ONBOARD_LED2         UI_ONBOARD_LED2_PORT,UI_ONBOARD_LED2_PIN

#define PANEL_LED_HV_PORT       GPIOH
#define PANEL_LED_HV_PIN        GPIO_PIN_2
#define PANEL_LED_HV            PANEL_LED_HV_PORT,PANEL_LED_HV_PIN

#define PANEL_LED_HEALTHY_PORT  GPIOH
#define PANEL_LED_HEALTHY_PIN   GPIO_PIN_5
#define PANEL_LED_HEALTHY       PANEL_LED_HEALTHY_PORT,PANEL_LED_HEALTHY_PIN

#define PANEL_LED_FAULT_PORT    GPIOB
#define PANEL_LED_FAULT_PIN     GPIO_PIN_1
#define PANEL_LED_FAULT         PANEL_LED_FAULT_PORT,PANEL_LED_FAULT_PIN

#define PANEL_LED_ERR_PORT      GPIOB
#define PANEL_LED_ERR_PIN       GPIO_PIN_1
#define PANEL_LED_ERR           PANEL_LED_ERR_PORT,PANEL_LED_ERR_PIN


#define WRITEPIN                HAL_GPIO_WritePin
#define READPIN                 HAL_GPIO_ReadPin





#define RESET_UI                UI_COMM_IO1
#define ADC_READY               UI_COMM_IO4
#define ADC_IN_BOOTLOADER       UI_COMM_IO3

#define UI_NEED_RESET           UI_COMM_IO11
#define UI_READY                UI_COMM_IO12
#define UI_IN_BOOTLOADER        UI_COMM_IO13
#define STAY_IN_BOOTLOADER      UI_COMM_IO14
#define ADC_NEED_RESET          UI_COMM_IO15





void GPIO_Init(void);
#endif