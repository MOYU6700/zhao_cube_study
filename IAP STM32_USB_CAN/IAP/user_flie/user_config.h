#ifndef __USER_CONFIG_H
#define __USER_CONFIG_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>


#define SAFE(x) do{ \
	__set_PRIMASK(1); \
	x; \
	__set_PRIMASK(0); \
}while(0)	//原子操作

#define UART_BUFF_LEN 50*1024  //定义最大接收字节数 100K

extern CAN_HandleTypeDef hcan;
extern UART_HandleTypeDef huart1; 

#endif /* __USER_CONFIG_H */
