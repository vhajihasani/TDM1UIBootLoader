#include "main.h"
#include "lwip.h"
#include "usart.h"
#include "tcp_echoserver.h"
#include "ethernet_handler.h"
#include "spicomm.h"
#include "fmc.h"
#include "BSP_SDRAM.h"
#include "systemVariables.h"
#include "RTD.h"
#include "FRAM.h"
#include "adc.h"
#include "math.h"
#include "GPIODefine.h"

int globalint1 = 0;
int globalint2 = 0;
int i = 0;
char globalchar[20];
SPI_HandleTypeDef hspi2;//COMM
SPI_HandleTypeDef hspi4;//FRAM
SPI_HandleTypeDef hspi5;//RTD

TIM_HandleTypeDef htim6;//TIM ADC
ADC_HandleTypeDef hadc3;//ADC
DMA_HandleTypeDef hdma_adc3;//ADC
extern IWDG_HandleTypeDef hiwdg1;//WWDG
uint8_t RxData[COM_BUFFER_SIZE] __attribute__((at(0x24048000)));
uint8_t TxData[COM_BUFFER_SIZE] __attribute__((at(0x2404A000)));
sysVar systemVariables;

float globalfloat = 0;

extern double globaldouble1[10];
extern double globaldouble2[10];
extern double globaldouble3[10];

uint8_t TestRx[256];
uint8_t TestTx[256];

bool globalbool;
void SystemClock_Config(void);
static void MX_CRC_Init(void);
static void MPU_Config(void);
static bool LastStart = 0;

int main(void)
{
	HardwareInit(&systemVariables);
	MX_LWIP_Init();
	tcp_echoserver_init();
	set_receive_handler(handle_data_recieved);
	systemVariables.Eth_Enable = 1;
	LEDBlinker(&systemVariables);
	FRAM_Init(&hspi4);
	ReadFirmware(&systemVariables);
	
	WRITEPIN(PANEL_LED_HV,1);
	WRITEPIN(PANEL_LED_HEALTHY,1);
	
	
	////pppp First Time
	//WriteFirmware(&systemVariables);
	////pppp vayse too bootloader
	//systemVariables.StayInBootloader = 1;
	if(systemVariables.StayInBootloader == 0)
	{
		FirmwareReady(&systemVariables);
		jump_to_app();
	}
	else
	{
		WRITEPIN(STAY_IN_BOOTLOADER,1);
	}
	
	WRITEPIN(UI_READY,1);
	HAL_Delay(2000);
	COMM_SPI_Init();
	HAL_SPI_TransmitReceive_DMA(&hspi2 ,(uint8_t *)&TxData[0],(uint8_t *)&RxData[0],COM_BUFFER_SIZE);
	InitSystem(&systemVariables);
	
	
	while (1)
	{
		BootloaderHandler(&systemVariables);
	}
}



void Error_Handler(void)
{
	printf("Error_Handler\r\n");
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
