#ifndef FRAM_H
#define FRAM_H

#include "main.h"
#include "systemVariables.h"

#define FRAM_SPI_INS            SPI4
#define FRAM_CS_PIN             GPIOE,GPIO_PIN_4
#define FM25CL64_WREN           0x06
#define FM25CL64_WRDI           0x04
#define FM25CL64_RDSR           0x05
#define FM25CL64_WRSR           0x01
#define FM25CL64_READ           0x03
#define FM25CL64_WRITE          0x02
#define FM25CL64_PROTECT        0x0c
#define FM25CL64_UNPROTECT      0x00
#define FRAM_CS_ON              HAL_GPIO_WritePin(FRAM_CS_PIN,0)
#define FRAM_CS_OFF             HAL_GPIO_WritePin(FRAM_CS_PIN,1)

void FRAM_SPI_init(SPI_HandleTypeDef *hspi);
void FRAM_Init(SPI_HandleTypeDef *hspi);
void FRAM_GPIO_Init(void);

uint8_t BSP_FM25CL64_ReadState(void);
void BSP_FM25CL64_WriteEnable(void);
void BSP_FM25CL64_WriteState(uint8_t data);
void BSP_FM25CL64_ProAll(void);
void BSP_FM25CL64_UProAll(void);
uint8_t BSP_FM25CL64_SingleRead(uint16_t address);
void BSP_FM25CL64_MultipleRead(uint16_t address, uint16_t num, uint8_t *pointer);
uint8_t BSP_FM25CL64_SingleWrite(uint16_t address, uint8_t value);
void BSP_FM25CL64_MultipleWrite(uint16_t address, uint16_t num, uint8_t *pointer);

void FRAM_Write(uint16_t page_num, uint8_t* pBuff);
void FRAM_Read(uint16_t page_num, uint8_t* pBuff,uint16_t buffSize);

void ReadFirmware(sysVar* systemVariables);
void WriteFirmware(sysVar* systemVariables);
void FirmwareReady(sysVar* systemVariables);

#endif
