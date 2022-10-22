#ifndef __RTD_H__
#define __RTD_H__

#include "systemvariables.h"
#include "GPIODefine.h"
#include "main.h"
#include <math.h>
#define RTD_SPI_INS SPI5

#define RTD_CONFIG_REG             0x00
#define RTD_CONFIG_BIAS            0x80
#define RTD_CONFIG_MODEAUTO        0x40
#define RTD_CONFIG_MODEOFF         0x00
#define RTD_CONFIG_1SHOT           0x20
#define RTD_CONFIG_3WIRE           0x10
#define RTD_CONFIG_24WIRE          0x00
#define RTD_CONFIG_FAULTSTAT       0x02
#define RTD_CONFIG_FILT50HZ        0x01
#define RTD_CONFIG_FILT60HZ        0x00

#define RTD_RTDMSB_REG             0x01
#define RTD_RTDLSB_REG             0x02
#define RTD_HFAULTMSB_REG          0x03
#define RTD_HFAULTLSB_REG          0x04
#define RTD_LFAULTMSB_REG          0x05
#define RTD_LFAULTLSB_REG          0x06
#define RTD_FAULTSTAT_REG          0x07


#define RTD_FAULT_HIGHTHRESH       0x80
#define RTD_FAULT_LOWTHRESH        0x40
#define RTD_FAULT_REFINLOW         0x20
#define RTD_FAULT_REFINHIGH        0x10
#define RTD_FAULT_RTDINLOW         0x08
#define RTD_FAULT_OVUV             0x04

#define RTD_RREF      400.0f
#define RTD_RNOMINAL  100.0f

#define RTD_MAX_ADC 32768

#define RTD_A 3.9083e-3
#define RTD_B -5.775e-7

#define RTD_CS_ON     HAL_GPIO_WritePin(RTD_CS,0)
#define RTD_CS_OFF    HAL_GPIO_WritePin(RTD_CS,1)
void RTD_Init(void);
void WriteRTDReg(uint8_t Addr,uint8_t Data);
uint8_t ReadRTDReg(uint8_t Addr);
uint16_t Read2RTDReg(uint8_t Addr);
uint8_t RTDReadFault(void);
void RTDClearFault(void);
void RTDEnableBias(bool Enable);
void RTDAutoConvert(bool Enable);
void RTDWireSelect(uint8_t WireNum);
void RTDFilterSelect(uint8_t filterHz);
uint16_t RTDReadData(void);
bool RTDTempC(float* ReadTemp);
void RTDTransTemprature(sysVar* systemVariables);
#endif