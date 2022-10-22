
#include "tcp_echoserver.h"

void (*handle_data_recieved_fn)(struct tcp_pcb *tpcb, char * rdata, uint16_t rdata_len);

#if LWIP_TCP

static struct tcp_pcb *tcp_echoserver_pcb;

void set_receive_handler(void *arg)
{
	handle_data_recieved_fn = arg;
}

/**
  * @brief  Initializes the tcp echo server
  * @param  None
  * @retval None
  */
void tcp_echoserver_init(void)
{
  /* create new tcp pcb */
  tcp_echoserver_pcb = tcp_new();

  if (tcp_echoserver_pcb != NULL)
  {
    err_t err;
    
    /* bind echo_pcb to port 7 (ECHO protocol) */
    err = tcp_bind(tcp_echoserver_pcb, IP_ADDR_ANY, 7);

    if (err == ERR_OK)
    {
      /* start tcp listening for echo_pcb */
      tcp_echoserver_pcb = tcp_listen(tcp_echoserver_pcb);

      /* initialize LwIP tcp_accept callback function */
      tcp_accept(tcp_echoserver_pcb, tcp_echoserver_accept);
    }
    else 
    {
      /* deallocate the pcb */
      memp_free(MEMP_TCP_PCB, tcp_echoserver_pcb);
      //printf("Can not bind pcb\n");
    }
  }
  else
  {
    //printf("Can not create new pcb\n");
  }
}

void tcp_echoserver_deinit(void)
{
  /* create new tcp pcb */
  tcp_echoserver_pcb = tcp_new();

  if (tcp_echoserver_pcb != NULL)
  {
      /* deallocate the pcb */
      memp_free(MEMP_TCP_PCB, tcp_echoserver_pcb);
  }
  else
  {
    //printf("Can not create new pcb\n");
  }
}

/**
  * @brief  This function is the implementation of tcp_accept LwIP callback
  * @param  arg: not used
  * @param  newpcb: pointer on tcp_pcb struct for the newly created tcp connection
  * @param  err: not used 
  * @retval err_t: error status
  */
static err_t tcp_echoserver_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{

  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);

  /* set priority for the newly accepted tcp connection newpcb */
  tcp_setprio(newpcb, TCP_PRIO_MIN);
  
	/* initialize lwip tcp_recv callback function for newpcb  */ 
	tcp_recv(newpcb, tcp_echoserver_recv);
	
	/* initialize lwip tcp_err callback function for newpcb  */
	tcp_err(newpcb, tcp_echoserver_error);

	/* initialize lwip tcp_poll callback function for newpcb */
	tcp_poll(newpcb, tcp_echoserver_poll, 1);

	/* initialize LwIP tcp_sent callback function */
	tcp_sent(newpcb, tcp_echoserver_sent);

	return ERR_OK;  
}


/**
  * @brief  This function is the implementation for tcp_recv LwIP callback
  * @param  arg: pointer on a argument for the tcp_pcb connection
  * @param  tpcb: pointer on the tcp_pcb connection
  * @param  pbuf: pointer on the received pbuf
  * @param  err: error information regarding the reveived pbuf
  * @retval err_t: error code
  */
static err_t tcp_echoserver_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  err_t ret_err;
	//char rdata[20];

	LWIP_UNUSED_ARG(arg);

  /* if we receive an empty tcp frame from client => close connection */
  if (p == NULL)
  {
		 /* we're done sending, close connection */
		tcp_echoserver_connection_close(tpcb);
    ret_err = ERR_OK;
  }
  /* else : a non empty frame was received from client but for some reason err != ERR_OK */
  else if(err != ERR_OK)
  {
    /* free received pbuf*/
    pbuf_free(p);
    ret_err = err;
  }
  else
  {
    char rdata[2100];
    struct pbuf *p_temp;
    int len_temp = 0;
    p_temp = p;
    while (p_temp != NULL) {
      memcpy(&rdata[len_temp], p_temp->payload, p_temp->len);
      len_temp = len_temp + p_temp->len;
      p_temp = p_temp->next;
    }
    /* Update tcp window size to be advertized : should be called when received
    data (with the amount plen) has been processed by the application layer */
    tcp_recved(tpcb, p->tot_len);
    /* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
    pbuf_free(p);
    
    handle_data_recieved_fn(tpcb, rdata, len_temp);
    
    ret_err = ERR_OK;
  }  
  return ret_err;
}

/**
  * @brief  This function implements the tcp_err callback function (called
  *         when a fatal tcp_connection error occurs. 
  * @param  arg: pointer on argument parameter 
  * @param  err: not used
  * @retval None
  */
static void tcp_echoserver_error(void *arg, err_t err)
{
  LWIP_UNUSED_ARG(err);
	LWIP_UNUSED_ARG(arg);
}

/**
  * @brief  This function implements the tcp_poll LwIP callback function
  * @param  arg: pointer on argument passed to callback
  * @param  tpcb: pointer on the tcp_pcb for the current tcp connection
  * @retval err_t: error code
  */
static err_t tcp_echoserver_poll(void *arg, struct tcp_pcb *tpcb)
{
  LWIP_UNUSED_ARG(arg);
  return ERR_OK;
}

/**
  * @brief  This function implements the tcp_sent LwIP callback (called when ACK
  *         is received from remote host for sent data) 
  * @param  None
  * @retval None
  */
static err_t tcp_echoserver_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  LWIP_UNUSED_ARG(len);
	LWIP_UNUSED_ARG(arg);
  return ERR_OK;
}


/**
  * @brief  This functions closes the tcp connection
  * @param  tcp_pcb: pointer on the tcp connection
  * @param  es: pointer on echo_state structure
  * @retval None
  */
static void tcp_echoserver_connection_close(struct tcp_pcb *tpcb)
{
  
  /* remove all callbacks */
  tcp_arg(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_recv(tpcb, NULL);
  tcp_err(tpcb, NULL);
  tcp_poll(tpcb, NULL, 0);
  
  /* close tcp connection */
  tcp_close(tpcb);
}

err_t tcp_puts(struct tcp_pcb *tpcb, const char * str_data)
{
	err_t wr_err = ERR_OK;
	wr_err = tcp_write(tpcb, (void *)str_data, strlen(str_data), 1);
	return wr_err;
}

#endif /* LWIP_TCP */
