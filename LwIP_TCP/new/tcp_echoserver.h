#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#define TEST_STR "*TEST#\0"

void set_receive_handler(void *arg);
void tcp_echoserver_init(void);
void tcp_echoserver_deinit(void);
static err_t tcp_echoserver_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t tcp_echoserver_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_echoserver_error(void *arg, err_t err);
static err_t tcp_echoserver_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_echoserver_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_echoserver_connection_close(struct tcp_pcb *tpcb);
//static void handle_data_recieved(struct tcp_pcb *tpcb, char * rdata);
err_t tcp_puts(struct tcp_pcb *tpcb, const char * str_data);




