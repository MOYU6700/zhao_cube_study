/**
  ******************************************************************************
  * @file    LwIP/LwIP_UDP_Echo_Client/Src/udp_echoclient.c
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    29-April-2016
  * @brief   UDP echo client
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright © 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

#include "app_ethernet.h"
#include "user_io.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//uint8_t udp_setbuff[100]="The UDP test is going on the road!";
/* Private function prototypes -----------------------------------------------*/
//½ÓÊÕÊý¾Ý»Øµ÷º¯ÊýÉùÃ÷
void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
void udp_client_send(uint8_t *data,uint16_t len);

u8_t   data[100];  //Êý¾Ýbuffer³õÊ¼»¯
__IO uint32_t message_count = 0;
struct udp_pcb *upcb_client;   //¶¨Òå¿Í»§¶ËµÄ³õÊ¼»¯Ä£¿é£»
struct udp_pcb *upcb_server;   //¶¨Òå·þÎñÆ÷µÄ³õÊ¼»¯Ä£¿é£»

/* Private functions ---------------------------------------------------------*/
/****************	·þÎñ¶Ë»Øµ÷º¯Êý***********************/
void udp_server_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	if (upcb_client->flags & UDP_FLAGS_CONNECTED)
	{
		/* free the UDP connection, so we can accept new clients */
		udp_disconnect(upcb_client);
	}

	if (!(upcb_client->flags & UDP_FLAGS_CONNECTED))
	{
			/* Connect to the remote client */
			udp_connect(upcb_client, addr, port);
	}

	/* Free receive pbuf */
	pbuf_free(p);
}

/**
  * @brief  Connect to UDP echo server
  * @param  None
  * @retval None
  */
//UDP¿Í»§¶ËÁ¬½Óº¯Êý
//ÔÚmain.cÎÄ¼þÖÐ£¬ÔÚLWIP³õÊ¼»¯Ö®ºó£¬Ôö¼Ó¿Í»§¶Ë³õÊ¼»¯º¯Êý£¬¼´¿ÉÊµÏÖ¿Í»§¶Ë¹¦ÄÜ£¬ÊµÏÖÊý¾Ý»ØÏÔ¹¦ÄÜ¡£
void udp_echoclient_connect(void)
{
  ip_addr_t DestIPaddr;
  err_t err;
  /******************¿Í»§¶ËµÄÁ¬½Ó**************************/
  /* Create a new UDP control block  */
  upcb_client = udp_new();
  
  if (upcb_client!=NULL)
  {
    /*assign destination IP address */
    IP4_ADDR( &DestIPaddr, DEST_IP_ADDR0, DEST_IP_ADDR1, DEST_IP_ADDR2, DEST_IP_ADDR3 );   //ÉèÖÃ·þÎñÆ÷¶ËµÄIPµØÖ·
		
    udp_bind(upcb_client,IP_ADDR_ANY,UDP_CLIENT_PORT);        //¿Í»§¶Ë¶Ë¿ÚµÄ°ó¶¨£»
		
    /* configure destination IP address and port */
		
    err= udp_connect(upcb_client, &DestIPaddr, UDP_REMOTE_PORT);   //·þÎñÆ÷¶ËµØÖ·¡¢¶Ë¿ÚÅäÖÃ£»
    
    if (err == ERR_OK)
    {
      /* Set a receive callback for the upcb */
      udp_recv(upcb_client, udp_receive_callback, NULL);      //×¢²á»Øµ÷º¯Êý
    }   
  }
	
	 /******************·þÎñÆ÷µÄÁ¬½Ó**************************/
	/* Create a new UDP control block  */
  upcb_server = udp_new();
  
  if (upcb_server!=NULL)
  {   
    /* configure destination IP address and port */
		err=udp_connect(upcb_server, IP_ADDR_ANY, UDP_SERVER_PORT); 
    if (err == ERR_OK)
    {
      /* Set a receive callback for the upcb */
      udp_recv(upcb_server, udp_server_receive_callback, NULL);       //×¢²á»Øµ÷º¯Êý
    }
  }
	
}

/***/
void udp_echoclient_send(void)
{
	
}	

/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
//¿Í»§¶ËÊý¾Ý·¢ËÍº¯Êý(¸öÈË¸Ä¶¯)
void udp_client_send(uint8_t *data,uint16_t len)
{
  struct pbuf *p;
  /* allocate pbuf from pool*/
  p = pbuf_alloc(PBUF_TRANSPORT,len, PBUF_POOL);
  if (p != NULL)
  {
    /* copy data to pbuf */
    pbuf_take(p, data, len);
		
    /* send udp data */
    udp_send(upcb_client, p);      //·¢ËÍÊý¾Ý
    
    /* free pbuf */
    pbuf_free(p);
  }
}

/*********************¶¨ÒåÍøÂç½ÓÊÕÊý¾ÝµÄº¯Êý****************************/
uint8_t udp_rec_buff[1024];
void udp_client_data_handle(uint8_t *data,uint8_t *rec_buff,uint16_t length)
{
	if(length!=0)//´ú±íÓÐÊý¾Ý£»
	{
		memcpy(udp_rec_buff,data,length);
	}	
}	

/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
//¿Í»§¶Ë½ÓÊÕÊý¾Ý»Øµ÷º¯Êý
void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
/* Connect to the remote client */
//  udp_connect(upcb, addr, UDP_REMOTE_PORT);
  udp_client_data_handle(p->payload,udp_rec_buff,p->len);
  /* Tell the client that we have accepted it */
  udp_send(upcb, p);
  /* free the UDP connection, so we can accept new clients */
//  udp_disconnect(upcb);
	
  /* Free receive pbuf */
  pbuf_free(p);			
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
