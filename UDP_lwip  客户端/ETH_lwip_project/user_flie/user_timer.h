#ifndef __USER_TIMER_H
#define __USER_TIMER_H
#include "stm32f1xx_hal.h"

extern void user_timer_init(void);
extern uint8_t get_timer2_flag(void);
extern void clr_timer2_flag(void);
extern TIM_HandleTypeDef htim2;
#endif
