#ifndef __USER_UART_H
#define __USER_UART_H

#include "user_config.h"


extern uint32_t uart_cnt;
extern uint8_t uart_rec_buff[UART_BUFF_LEN];
extern void user_uart_init(void);
extern void user_uart_stop(void);

#endif /* __USER_UART_H */
