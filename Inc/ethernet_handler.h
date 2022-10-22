#ifndef __ETHERNET_HANDLER_H__
#define __ETHERNET_HANDLER_H__

#include "main.h"
#include <string.h>
#include "tcp_echoserver.h"
#include "ethernetif.h"
#include "systemVariables.h"

enum eth_rx_state {
	IDLE_STATE,
	SET_SETTING_STATE,
	SET_SETTING_STATE2,
	SET_PARAMS_STATE,
	MAINT_MODE_STATE,
	SET_MODEL_STATE,
};

extern struct tcp_pcb *Tpcb;
void clear_data_buf(char *data, uint16_t data_len);

void Ethernet_Init(void);
void Ethernet_Handle(void);
void handle_data_recieved(struct tcp_pcb *tpcb, char * rdata, uint16_t rdata_len);
void Send_Data_From_SD(uint8_t FL_DevNum, uint16_t start_block_addr, uint16_t block_length);
void EthernetHandler(void);
uint8_t Eth_Link_Handler(void);
void GetCalibrationRegister(const char* message);
double SendCalibrationRegister(const char* message);
double SendValue(const char* message);
#endif
