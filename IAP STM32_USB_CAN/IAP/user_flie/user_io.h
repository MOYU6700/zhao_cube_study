#ifndef __USER_IO_H
#define __USER_IO_H


#define Uplimit1(x) HAL_GPIO_WritePin(Uplimit1_GPIO_Port, Uplimit1_Pin, (GPIO_PinState)(!(x)))
#define Uplimit2(x) HAL_GPIO_WritePin(Uplimit2_GPIO_Port, Uplimit2_Pin, (GPIO_PinState)(!(x)))
#define Uplimit3(x) HAL_GPIO_WritePin(Uplimit3_GPIO_Port, Uplimit3_Pin, (GPIO_PinState)(!(x)))
#define Downlimit1(x) HAL_GPIO_WritePin(Downlimit1_GPIO_Port, Downlimit1_Pin, (GPIO_PinState)(!(x)))
#define Downlimit2(x) HAL_GPIO_WritePin(Downlimit2_GPIO_Port, Downlimit2_Pin, (GPIO_PinState)(!(x)))
#define Downlimit3(x) HAL_GPIO_WritePin(Downlimit3_GPIO_Port, Downlimit3_Pin, (GPIO_PinState)(!(x)))

#define LED_UP_LIMIT1_TOGGLE() HAL_GPIO_TogglePin(Uplimit1_GPIO_Port, Uplimit1_Pin)
#define LED_UP_LIMIT2_TOGGLE() HAL_GPIO_TogglePin(Uplimit2_GPIO_Port, Uplimit2_Pin)
#define LED_UP_LIMIT3_TOGGLE() HAL_GPIO_TogglePin(Uplimit3_GPIO_Port, Uplimit3_Pin)
#define LED_DOWN_LIMIT1_TOGGLE() HAL_GPIO_TogglePin(Downlimit1_GPIO_Port, Downlimit1_Pin)
#define LED_DOWN_LIMIT2_TOGGLE() HAL_GPIO_TogglePin(Downlimit2_GPIO_Port, Downlimit2_Pin)
#define LED_DOWN_LIMIT3_TOGGLE() HAL_GPIO_TogglePin(Downlimit3_GPIO_Port, Downlimit3_Pin)


extern void user_io_init(void);
extern void user_adc_start(void);
extern void user_adc_stop(void);

#endif /* __USER_IO_H */
