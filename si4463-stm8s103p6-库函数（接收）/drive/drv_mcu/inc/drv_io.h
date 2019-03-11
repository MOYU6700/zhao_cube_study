#ifndef __DRV_IO_H__
#define __DRV_IO_H__
#include "stm8s.h"
#include "stm8s_exti.h"

#define LED1_PIN        GPIO_PIN_5 
#define LED1_PORT       GPIOB

#define IRQ_EXTI_PORT  GPIOD
#define IRQ_EXTI_PIN   GPIO_PIN_3

extern void GPIO_Config(void);
extern void LED_Init(void);
extern void LED1_Toggle(void);
#endif