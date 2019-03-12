#ifndef __MAIN_H__
#define __MAIN_H__


#include "drv_delay.h"
#include "drv_led.h"
#include "drv_spi.h"
#include "drv_uart.h"
#include "drv_SI4438.h"


#define 	__SI4438_TX_TEST__							//**@@ 如果测试发送功能则需要定义该宏，如果测试接收则需要屏蔽该宏 **@@//
//#define 	__USE_SOFT_SPI_INTERFACE__					//**@@ 如果使用软件SPI则需要定义该宏，反之如果使用硬件SPI则需要屏蔽该宏 **@@//

extern uint32_t oled_for_count;
#endif
