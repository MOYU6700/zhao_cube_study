#ifndef __SPI_LCD_H
#define __SPI_LCD_H

#include "stm32f1xx_hal.h"

#define X_MAX_PIXEL	        128
#define Y_MAX_PIXEL	        160

#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GRAY1   0x8410      	//灰色1      00000 000000 00000
#define GRAY2   0x4208      	//灰色2  1111111111011111


//#define LCD_CS_SET(x) LCD_CTRL->ODR=(LCD_CTRL->ODR&~LCD_CS)|(x ? LCD_CS:0)

//液晶控制口置1操作语句宏定义 
#define	LCD_CS_SET  	HAL_GPIO_WritePin(OUTPUT_LCD_CS_GPIO_Port, OUTPUT_LCD_CS_Pin, GPIO_PIN_SET) 
  
#define	LCD_LED_SET  	HAL_GPIO_WritePin(OUTPUT_LCD_BL_GPIO_Port, OUTPUT_LCD_BL_Pin, GPIO_PIN_SET)  
#define	LCD_RS_SET  	HAL_GPIO_WritePin(OUTPUT_LCD_DC_GPIO_Port, OUTPUT_LCD_DC_Pin, GPIO_PIN_SET)
#define	LCD_RST_SET  	HAL_GPIO_WritePin(OUTPUT_LCD_RES_GPIO_Port, OUTPUT_LCD_RES_Pin, GPIO_PIN_SET)
//液晶控制口置0操作语句宏定义
#define	LCD_CS_CLR  	HAL_GPIO_WritePin(OUTPUT_LCD_CS_GPIO_Port, OUTPUT_LCD_CS_Pin, GPIO_PIN_RESET) 
    
#define	LCD_LED_CLR  	HAL_GPIO_WritePin(OUTPUT_LCD_BL_GPIO_Port, OUTPUT_LCD_BL_Pin, GPIO_PIN_RESET) 
#define	LCD_RST_CLR  	HAL_GPIO_WritePin(OUTPUT_LCD_RES_GPIO_Port, OUTPUT_LCD_RES_Pin, GPIO_PIN_RESET)
#define	LCD_RS_CLR  	HAL_GPIO_WritePin(OUTPUT_LCD_DC_GPIO_Port, OUTPUT_LCD_DC_Pin, GPIO_PIN_RESET)

#define LCD_UsrLog(...) \
do{ \
	lcd_printf(__VA_ARGS__); \
	printf(__VA_ARGS__); \
}while(0)

extern void lcd_printf(const char *fmt /*format*/, ...);
extern void spi_lcd_init(void);
#endif

