/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   BUTTON配置H文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */


#ifndef __DRV_BUTTON_H__
#define __DRV_BUTTON_H__


#include "stm8s_gpio.h"


//按键硬件接口定义
#define BUTOTN_GPIO_PORT			GPIOC									
#define BUTTON_GPIO_PIN				GPIO_PIN_0


/** 按键定义 */
enum
{
	BUTOTN_UP = 0,		//按键没有按下
	BUTOTN_PRESS_DOWN	//按键按下
};



void drv_button_init( void );
uint8_t drv_button_check( void );

#endif

