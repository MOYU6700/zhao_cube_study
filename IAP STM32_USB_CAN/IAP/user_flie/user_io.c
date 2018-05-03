#include "stm32f1xx_hal.h"
#include "user_config.h"
#include "user_io.h"

#include <string.h>


void user_io_init(void)
{	
	HAL_GPIO_WritePin(MAX485_IO_EN_GPIO_Port, MAX485_IO_EN_Pin, GPIO_PIN_SET);	//485接收	
}




