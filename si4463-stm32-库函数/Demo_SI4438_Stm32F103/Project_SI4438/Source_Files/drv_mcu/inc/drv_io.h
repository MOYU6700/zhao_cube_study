#ifndef __DRV_IO_H__
#define __DRV_IO_H__

#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "drv_SI4438.h"

extern void EXTIX_Init(void);
extern void EXTI4_IRQHandler(void);
#endif


