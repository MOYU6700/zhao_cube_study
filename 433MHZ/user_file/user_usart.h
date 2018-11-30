#include "user_config.h"

#define USART3_BUFF 10
struct frame
{
	__IO uint8_t enable;
	__IO uint8_t data;
	__IO uint8_t index;
	__IO uint8_t buff[USART3_BUFF];
};	
extern struct frame frame;
extern void user_io_init(void);
extern void usart2_send(void);
extern uint8_t usart2_tx[9];

