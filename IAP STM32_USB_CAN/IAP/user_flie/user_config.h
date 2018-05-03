#ifndef __USER_CONFIG_H
#define __USER_CONFIG_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>


#define SAFE(x) do{ \
	__set_PRIMASK(1); \
	x; \
	__set_PRIMASK(0); \
}while(0)	//原子操作



extern CAN_HandleTypeDef hcan;

#endif /* __USER_CONFIG_H */
