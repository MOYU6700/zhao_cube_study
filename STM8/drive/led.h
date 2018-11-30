#include "stm8s.h"

#define LED_ON()        GPIO_WriteLow(GPIOB, GPIO_PIN_5)
#define LED_OFF()       GPIO_WriteHigh(GPIOB, GPIO_PIN_5)
#define LOGGLE_LED()    GPIO_WriteReverse(GPIOB, GPIO_PIN_5)


extern void led_init(void);