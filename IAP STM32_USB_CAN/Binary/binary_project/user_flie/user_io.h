#ifndef __USER_IO_H
#define __USER_IO_H


#define up_limit1() HAL_GPIO_TogglePin(up_limit1_GPIO_Port, up_limit1_Pin)
#define up_limit2() HAL_GPIO_TogglePin(up_limit2_GPIO_Port, up_limit2_Pin)
#define up_limit3() HAL_GPIO_TogglePin(up_limit3_GPIO_Port, up_limit3_Pin)

#define down_limit1() HAL_GPIO_TogglePin(down_limit1_GPIO_Port, down_limit1_Pin)
#define down_limit2() HAL_GPIO_TogglePin(down_limit2_GPIO_Port, down_limit2_Pin)
#define down_limit3() HAL_GPIO_TogglePin(down_limit3_GPIO_Port, down_limit3_Pin)

#endif

