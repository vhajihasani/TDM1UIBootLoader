#ifndef __SYSTEMVARIABLES_H__
#define __SYSTEMVARIABLES_H__

#include "stm32h7xx_hal.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include "stdlib.h"
#include "main.h"
#include "math.h"
#include "GPIODefine.h"
#include "TI_aes_128.h"

#pragma pack(push, 1)

/*                                                   DEFINES                                                   */
#define APP_START_ADDRESS   0x08020000


#define DATASIZE   1200000 //50s for 48KHz
#define RELAYTIMER 20    //20ms


#define GAINCHANGETIMER 500
#define CONNECTION_TIMEOUT 1000

#define STABLECOUNTER 50

#define MAX_RANGE   6e8
#define MIN_RANGE   -6e8
#define MIN_DEV     3e8
#define PI          3.141592653589793

#define COM_BUFFER_SIZE       8000
#define COM_DATA_SIZE         480
#define ADC_BUFF_UPDATE       20 // 1ms for 5 channel 4KHz = 5 * 4 = 20
#define ADC_CHANNEL_UPDATE    4
#define ADC_BUFF              400
#define CALIBRATION_NUM       200
#define ADCSAMPLING           4000
#define RESETFACTOR           0.97f
#define CALC_FILTER_BUFF      9600
#define NUMBER_OF_CYCLE_FOR_FREQUENCY_ESTIMATION 5
#define MIN_AMP_CLP                              1e-7


#define PICKUPFACTOR           1.1
#define RESETTIMEDEFINITE      200
#define PICKUPFACTOR_UV        0.9
#define RESETFACTOR_UV         1.05f
#define UNDER_INITIAL_BLOCK    0.24f
/*                                                    ENUMS                                                    */
typedef enum //BLError_t
{
	BL_ERR_NOERR = 0,
	BL_ERR_UIERASE = 1,
	BL_ERR_UIWRITE = 2,
	BL_ERR_ADCERASE = 3,
	BL_ERR_ADCWRITE = 4,
	BL_ERR_UIBADDATA = 5,
	BL_ERR_ADCBADDATA = 6,
	BL_ERR_ADCNORESPONSE = 7
}BLError_t;

typedef enum //ETHState_t
{
	ETH_IDLE = 0,
	ETH_SENDSTATUS = 1,
	ETH_ACKNOWLEDGE = 2,
	ETH_ERASE_OK = 3,
	ETH_WRITE_OK = 4,
	ETH_SENDDATA = 5,
	ETH_SENDERROR = 6
}ETHState_t;


typedef enum //ETHValue_t
{
	VAL_REFMEAN = 1,
	VAL_MEASMEAN = 2,
	VAL_REFSTATE = 3,
}ETHValue_t;


typedef enum //CalculationSeq_t
{
	CALC_IDLE      = 1,
	CALC_DATAREADY = 2,
	CALC_DFT       = 3,
	CALC_RESULT    = 4,
	CALC_INIT      = 5
}CalculationSeq_t;

typedef enum //UIADCChannel_t
{
	ADC_I_TR = 1,
	ADC_V_TR = 2,
	ADC_VPE1 = 3,
	ADC_VPE2 = 4
}UIADCChannel_t;

typedef enum //TestState_t
{
	TEST_IDLE = 0,
	UST_A = 1,
	UST_B = 2,
	UST_AB = 3,
	GST_A = 4,
	GST_B = 5,
	GST_AB = 6,
	GSTg_A = 7,
	GSTg_B = 8,
	GSTg_AB = 9,
	
	EXT_UST_A = 10,
	EXT_UST_B = 11,
	EXT_UST_AB = 12,
	EXT_GST_A = 13,
	EXT_GST_B = 14,
	EXT_GST_AB = 15,
	EXT_GSTg_A = 16,
	EXT_GSTg_B = 17,
	EXT_GSTg_AB = 18,
	
	HOT_COLLAR = 19,
	
	NORMAL_CAP_CALIBRATION = 80,
	EXT_REF_CALIBRATION = 81
	
}TestState_t;

typedef struct //SignalFFT_t
{
	double ampl;
	double angle; 
}DFT_t;
	

typedef enum //ADCState_t
{
	ADCIDLE     = 0,
	SHUNT1_G16  = 1,
	SHUNT1_G32  = 2,
	SHUNT1_G64  = 3,
	SHUNT2_G16  = 4,
	SHUNT2_G32  = 5,
	SHUNT2_G64  = 6,
	SHUNT3_G1   = 7,
	SHUNT3_G2   = 8,
	SHUNT3_G4   = 9,
	SHUNT3_G8   = 10,
	SHUNT3_G16  = 11,
	SHUNT4_G1   = 12,
	SHUNT4_G2   = 13,
	SHUNT4_G4   = 14,
	SHUNT4_G8   = 15,
	SHUNT4_G16  = 16,
	SHUNT5_G1   = 17,
	SHUNT5_G2   = 18,
	SHUNT5_G4   = 19,
	SHUNT5_G8   = 20,
	SHUNT5_G16  = 21,
	SHUNT6_G1   = 22,
	SHUNT6_G2   = 23,
	SHUNT6_G4   = 24,
	SHUNT6_G8   = 25,
	SHUNT6_G16  = 26,
	SHUNT6_G32  = 27,
	SHUNT6_G64  = 28,
	SHUNT6_G128 = 29,
	
}ADCState_t;

typedef enum // ADCDataFormat_t
{
	LeftJustified  = 0,
	I2S24Bit       = 1,
	RightJustified = 2,
	DSD1Bit        = 3
}ADCDataFormat_t;

typedef enum //ADCSamplingMode_t
{
	SingleRate = 0,
	DualRate   = 1,
	QuadRate   = 2
}ADCSamplingMode_t;


typedef enum // CommState_t
{
	COMM_IDLE = 0,
	COMM_DATA_READY = 1,
	COMM_BUSY = 2
}CommState_t;

typedef enum
{
	BL_IDLE = 0,
	
	BL_ERASE_ADC = 1,
	BL_WAIT_FOR_ERASE_ADC = 2,
	BL_ERASE_ADC_DONE = 3,
	BL_WRITE_ADC = 4,
	BL_WAIT_FOR_WRITE_ADC = 5,
	BL_WRITE_ADC_DONE = 6,
	
	BL_ERASE_UI = 7,
	BL_WAIT_FOR_ERASE_UI = 8,
	BL_ERASE_UI_DONE = 9,
	BL_WRITE_UI = 10,
	BL_WAIT_FOR_WRITE_UI = 11,
	BL_WRITE_UI_DONE = 12,
	
	BL_JUMP_TO_APP = 13,
	
}BootloaderState_t;

/*                                                   STRUCTS                                                   */
typedef struct //FFT_t
{
	double Value;
	double Angle;
}FFT_t;

typedef struct // TestCondition_t
{
	double Voltage;
	double Frequency;
	TestState_t TestState;
	float Time;
	uint8_t TestStart;
	
}TestCondition_t;

typedef struct //Calibration_t
{
	double MeasCoef[50];
	double RefCoef[50];
	
	double MeasOffset[50];
	double RefOffset[50];
}Calibration_t;


typedef struct //ADCCalibration_t
{
	double InputCurrentCoef;
	double InputVoltageCoef;
	double InputVPE1Coef;
	double InputVPE2Coef;
	
	double InputCurrentOffset;
	double InputVoltageOffset;
	double InputVPE1Offset;
	double InputVPE2Offset;
		
}UIADCCoef_t;

typedef struct //Error_t
{
	bool OverVoltageStart;
	bool OverVoltageTrip;
	bool UnderVoltageStart;
	bool UnderVoltageTrip;
	bool OverCurrentStart;
	bool OverCurrentTrip;
	bool LooseConnection;
	bool OpenCircuit;
	bool OLStart;
	bool OLTrip;
	bool NoEarth;
	bool Safety;
	
	
}Error_t;



typedef struct //Setting_t
{
	float TransformerTripTemp;
	float TransformerAlarmTemp;
	double NormalCapacitance;
	double NormalCapTanDelta;
	float OLTauHot;
	float OLTripTemp;
	float OLStartTemp;
	float OLPickup;
	float OLServiceFactor;
	float OVPickup;
	float OVDelay;
	float OCPickup;
	float OCDelay;
	float UVPickup;
	float UVBlock;
	float UVDelay;
	float TransPrimaryVoltage;
	float TransPrimaryCurrent;
}Setting_t;

typedef struct //Local_t
{
	bool SDRAM_Err;
	bool ADC_Err;
	uint8_t ADCChannel;
	FFT_t TransCurrent;
	FFT_t TransVoltage;
	FFT_t VPE1;
	FFT_t VPE2;
	float OLTripCounter;
	float EarthVoltageLimit;
}Local_t;






////pppptypedef struct // DataStr_t
////pppp{
////pppp	uint8_t    ErasingADC;
////pppp	uint8_t    EraseADCCompleted;
////pppp	uint8_t    WritingADCFlash;
////pppp	uint8_t    WriteADCFlashCompleted;
////pppp	uint32_t   LastWriteAddress;
////pppp	uint8_t    Reserved;
////pppp	
////pppp}DataStr_t;
typedef struct // DataStr_t
{
	CommState_t  CommState;
	ADCState_t   ADCStateMeas;
	ADCState_t   ADCStateRef;
	uint16_t     Index;
	double       DataRef[COM_DATA_SIZE];
	double       DataMeas[COM_DATA_SIZE];
	uint8_t      RefClip;
	uint8_t      RefDev;
	uint8_t      RefLow;
	uint8_t      MeasClip;
	uint8_t      MeasDev;
	uint8_t      MeasLow;
	uint8_t      LastChangeGain;
	uint8_t      DataValid;
	uint8_t    ErasingADC;
	uint8_t    EraseADCCompleted;
	uint8_t    WritingADCFlash;
	uint8_t    WriteADCFlashCompleted;
	uint32_t   LastWriteAddress;
	uint8_t    Reserved;
	
}DataStr_t;

typedef struct 
{
	double a[10];
	double b[10];
}FilterCoef_t;


typedef struct 
{
	double x[2][10];
	double y[2][10];
}FilterData_t;


typedef struct // CommandStr_t
{
	uint8_t      EraseADC;
	uint8_t      WriteFlash;
	uint32_t     FlashAddress;
	uint8_t      FlashData[32];
}CommandStr_t;

typedef struct //Eth
{
	ETHState_t   ETHState;
	double       ValueDouble;
	uint32_t     ValueUint32;
	uint8_t      CalibrationDestination;
	uint32_t     CalibrationIndex;
	ETHValue_t   ValueType;
	
	
}ETHConf_t;



typedef struct //sysVar
{
	Setting_t Setting;
	Error_t ErrorFlag;
	Local_t Local;
	
	uint32_t DataIndex;
	bool boolian;
	
	int32_t RefMAXRANGE;
	int32_t RefMINRANGE;
	int32_t RefMAXDEV;
	
	uint16_t MeasCalibIndex;
	uint16_t RefCalibIndex;
	
	bool ResetCalibration;
	bool SaveCalibration;
	bool ReadRTD;
	
	double RefRMS;
	
	int32_t RefMaxValue;
	int32_t RefMinValue;
	
	bool StartSampling;
	uint32_t DataBufferIndex;
	uint32_t MiliSecond;
	float FloatData;
	
	bool TanDeltaCalibration;
	
	
	uint32_t LM35DataCounter;
	float LM35MeanBuff;
	float AmbiantTemprature;
	double CalibrationValue;
	UIADCChannel_t UIADCChannel;
	uint8_t StartCalibration;
	bool systemInitialOK;
	UIADCCoef_t UIADCCoef;
	uint8_t ADCTimer;
	CalculationSeq_t CalculationSeq;
	uint32_t EndCalcIndex;
	uint32_t CurrentCalcIndex;
	uint32_t CalculationSamples;
	bool IIRFilterEnable;
	FFT_t IREF;
	FFT_t IMEAS;
	double SumSine[2];
	double SumCosine[2];
	uint32_t IIRIndex;
	uint16_t  DFTCycle;
	ETHConf_t ETHConf;
	bool DataReady;
	double SumRef;
	double SumMeas;
	double MeanRef;
	double MeanMeas;
	FFT_t Vout;
	FFT_t Iout;
	double TanDelta;
	double TanDeltaCorrected;
	double Capacitance;
	double WattLosses;
	
	uint32_t MeanIndex;
	uint32_t ConnectionTimer;
	bool WriteFRAM;
	bool ErrorState;
	bool ValidData;
	uint32_t ValidTimer;
	uint32_t SafetyTimer;
	double tempd1[2500];
	DataStr_t DataStr;
	double tempd[1200];
	CommandStr_t CommandStr;
	double tempd2[1200];
	bool StayInBootloader;
	BootloaderState_t BootloaderState;
	uint32_t FlashAddr;
	uint8_t FlashData[32];
	uint8_t FlashDataSeq;
	uint8_t ADCFlashDataSeq;
	uint8_t tryCounter;
	
	BLError_t BLError;
	uint32_t ErrorAddress;
	uint32_t ErrorData;
	uint8_t LastData;
	uint8_t Eth_Enable;
}sysVar;
/*                                                    FUNCS                                                    */
void MX_IWDG1_Init(void);
void InitSystem(sysVar* systemVariables);
void ADCFilterInit(sysVar* systemVariables);
bool DFTCalculation(sysVar* systemVariables);
float freqEstimation(sysVar* systemVariables);
void IIRInit(sysVar* systemVariables);
void CalculationFilterInit(sysVar* systemVariables);
bool ResultCalculation(sysVar* systemVariables);
void HardwareInit(sysVar* systemVariables);
void LEDBlinker(sysVar* systemVariables);
void BootloaderHandler(sysVar* systemVariables);
void jump_to_app();
void jump_to_boot();
void cmd_erase();
uint8_t cmd_write(sysVar* systemVariables);

#pragma pack(pop)



#endif
