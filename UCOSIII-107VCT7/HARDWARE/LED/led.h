#ifndef __LED_H__
#define __LED_H__

#include "stm32f1xx_hal.h"


#define D4_Pin GPIO_PIN_13
#define D4_GPIO_Port GPIOE
#define D3_Pin GPIO_PIN_14
#define D3_GPIO_Port GPIOE
#define D5_Pin GPIO_PIN_15
#define D5_GPIO_Port GPIOE

#define TOGGLE_LED3() HAL_GPIO_TogglePin(D3_GPIO_Port, D3_Pin);
#define TOGGLE_LED4() HAL_GPIO_TogglePin(D4_GPIO_Port, D4_Pin);
#define TOGGLE_LED5() HAL_GPIO_TogglePin(D5_GPIO_Port, D5_Pin);

#endif


