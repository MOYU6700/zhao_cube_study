#include "usart.h"

void usart1_init(void)
{
 UART1_Init((u32)2400, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TX_ENABLE);
 UART1_Cmd(ENABLE);
}

void usart1_sent_data(uint8_t * buffer, uint16_t len)
{
  uint16_t num;
  for(num=0;num<len,num++)
  {
    UART1_SendData8(buffer[num]);
  }
}


