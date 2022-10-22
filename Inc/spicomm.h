#ifndef __SPICOMM_H__
#define __SPICOMM_H__
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
extern SPI_HandleTypeDef hspi2;
void COMM_SPI_Init(void);
/* Definition for SPIx's NVIC */


#ifdef __cplusplus
}
#endif
#endif /*__ spi_H */

