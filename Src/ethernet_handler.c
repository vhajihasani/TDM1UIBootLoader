#include "ethernet_handler.h"
#include "cJSON.h"

struct tcp_pcb *Tpcb;
char tx_buf[20];
enum eth_rx_state rx_state = IDLE_STATE;
extern ETH_HandleTypeDef heth;
extern sysVar systemVariables;
__IO uint32_t  EthStatus = 0;
extern struct netif gnetif;

void handle_data_send(struct tcp_pcb *tpcb, const char *str_data)
{
	Tpcb = tpcb;
	////pppptcp_puts(Tpcb,str_data);
}
extern double Data[2][DATASIZE];
static uint8_t SendData  = 0;
static uint32_t DataSendIndex  = 0;
void handle_data_recieved(struct tcp_pcb *tpcb, char * rdata, uint16_t rdata_len)
{
	cJSON* rootMessage;
	cJSON* MessageType;
	
	Tpcb = tpcb;
	
	switch(rx_state)
	{
		case IDLE_STATE:
		{
			rdata[rdata_len] = '\0';
			
			rootMessage=cJSON_Parse(rdata);
			if(!rootMessage)
			{
				cJSON_Delete(rootMessage);
				return;
			}
			if(!cJSON_IsObject(rootMessage))
			{
				cJSON_Delete(rootMessage);
				return;
			}
			
			MessageType = cJSON_GetObjectItem(rootMessage,"messageType");
			if(!strcmp(MessageType->valuestring,"deviceStatus"))
			{
				systemVariables.ETHConf.ETHState = ETH_SENDSTATUS;
				systemVariables.StayInBootloader = 1;
			}
			else if(!strcmp(MessageType->valuestring,"TDMUpdate"))
			{
				systemVariables.ETHConf.ETHState = ETH_SENDSTATUS;
				systemVariables.StayInBootloader = 1;
			}
			else if(!strcmp(MessageType->valuestring,"eraseADC"))
			{
				systemVariables.BootloaderState = BL_ERASE_ADC;
			}
			else if(!strcmp(MessageType->valuestring,"eraseUI"))
			{
				systemVariables.BootloaderState = BL_ERASE_UI;
			}
			else if(!strcmp(MessageType->valuestring,"runProgram"))
			{
				systemVariables.BootloaderState = BL_JUMP_TO_APP;
			}
			else if(!strcmp(MessageType->valuestring,"writeUIFlash"))
			{
				if(systemVariables.FlashDataSeq == 0)
				{
					MessageType = cJSON_GetObjectItem(rootMessage,"Address");
					systemVariables.FlashAddr = MessageType->valueint;
				}
				
				MessageType = cJSON_GetObjectItem(rootMessage,"Data");
				int size = cJSON_GetArraySize(MessageType);
				for(int i = 0; i < 16; i++)
				{
					systemVariables.FlashData[i + (16 * systemVariables.FlashDataSeq)] = cJSON_GetArrayItem(MessageType,i)->valueint;
				}
				if(cJSON_HasObjectItem(rootMessage,"last"))
					systemVariables.LastData = 1;
				else
					systemVariables.LastData = 0;
				
				
				
				if(systemVariables.FlashDataSeq || systemVariables.LastData)
					systemVariables.BootloaderState = BL_WRITE_UI;
				else
					systemVariables.ETHConf.ETHState = ETH_ACKNOWLEDGE;
				
				
				systemVariables.FlashDataSeq = 1 - systemVariables.FlashDataSeq;
				
				
			}
			else if(!strcmp(MessageType->valuestring,"writeADCFlash"))
			{
				if(systemVariables.ADCFlashDataSeq == 0)
				{
					MessageType = cJSON_GetObjectItem(rootMessage,"Address");
					systemVariables.CommandStr.FlashAddress = MessageType->valueint;
					systemVariables.FlashAddr = systemVariables.CommandStr.FlashAddress;
				}
				
				MessageType = cJSON_GetObjectItem(rootMessage,"Data");
				int size = cJSON_GetArraySize(MessageType);
				for(int i = 0; i < 16; i++)
				{
					systemVariables.CommandStr.FlashData[i + (16 * systemVariables.ADCFlashDataSeq)] = cJSON_GetArrayItem(MessageType,i)->valueint;
				}
				if(cJSON_HasObjectItem(rootMessage,"last"))
					systemVariables.LastData = 1;
				else
					systemVariables.LastData = 0;
				if(systemVariables.ADCFlashDataSeq || systemVariables.LastData)
					systemVariables.BootloaderState = BL_WRITE_ADC;
				else
					systemVariables.ETHConf.ETHState = ETH_ACKNOWLEDGE;
				systemVariables.ADCFlashDataSeq = 1 - systemVariables.ADCFlashDataSeq;
			}
			cJSON_Delete(rootMessage);
			break;
		}
		default:
			break;
	}
	clear_data_buf(rdata, rdata_len);
}

void clear_data_buf(char *data, uint16_t data_len)
{
	memset(data, 0, data_len);
}


void EthernetHandler(void)///pppsysVar* systemVariables,
{
	cJSON* rootMessage;
	cJSON* MessageType;
	char wdata[700] = {0};
	switch(systemVariables.ETHConf.ETHState)
	{
		case ETH_IDLE:
		break;
		case ETH_SENDSTATUS:
			sprintf(wdata,"{\"deviceStatus\": \"bootLoader\" }");
			tcp_write(Tpcb, (uint8_t *)&wdata[0], strlen(wdata) , 1);
			tcp_output(Tpcb);
			systemVariables.ETHConf.ETHState = ETH_IDLE;
		break;
		
		case ETH_ERASE_OK:
			sprintf(wdata,"{\"messageType\": \"eraseADCOK\" }");
			tcp_write(Tpcb, (uint8_t *)&wdata[0], strlen(wdata) , 1);
			tcp_output(Tpcb);
			systemVariables.ETHConf.ETHState = ETH_IDLE;
		break;
		
		case ETH_WRITE_OK:
			sprintf(wdata,"{\"messageType\": \"writeADCOK\" , \"flashAddress\": %d }",systemVariables.FlashAddr);
			tcp_write(Tpcb, (uint8_t *)&wdata[0], strlen(wdata) , 1);
			tcp_output(Tpcb);
			systemVariables.ETHConf.ETHState = ETH_IDLE;
		break;
		case ETH_ACKNOWLEDGE:
			sprintf(wdata,"{\"messageType\": \"ack\" }");
			tcp_write(Tpcb, (uint8_t *)&wdata[0], strlen(wdata) , 1);
			tcp_output(Tpcb);
			systemVariables.ETHConf.ETHState = ETH_IDLE;
		break;
		
		default:
		break;
	}
}




#define WAIT_MS(n) if (cnt == n) {state++;cnt = 0;} else cnt++;
#define IP_ADDR0   systemVariables.LCDVar.EthIP1//192
#define IP_ADDR1   systemVariables.LCDVar.EthIP2//168
#define IP_ADDR2   systemVariables.LCDVar.EthIP3//0
#define IP_ADDR3   systemVariables.LCDVar.EthIP4	//10
/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0
#define PHY_MISR               ((uint16_t)0x12) /* MII Interrupt Status and Misc. Control Register */
#define PHY_LINK_STATUS        ((uint16_t)0x2000) /* PHY link status interrupt mask */
/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   0
#define GW_ADDR3   1

