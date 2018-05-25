#include "stm32f1xx_hal.h"
#include "user_uart.h"
#include "flash_map.h" 
#include "user_boot.h"
#include <string.h>
#include "user_io.h"

uint32_t uart_cnt=0;
uint8_t uart_rec_buff[UART_BUFF_LEN] __attribute__ ((at(USER_FLASH_BOOT_DATA_BASE)));//接收缓冲,最大USART_REC_LEN个字节,起始地址为USER_FLASH_BIN_BASE.  

extern UART_HandleTypeDef huart1;

void user_uart_init(void)
{
	__HAL_UART_ENABLE(&huart1);
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_rec_buff, 1);	//串口接收一个字节，并通过中断返回结果
}

void user_uart_stop(void)
{
	__HAL_UART_DISABLE(&huart1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t temp_buff;
	if(huart->Instance==USART1)
	{
		LED_UP_LIMIT1_TOGGLE();
		if(HAL_UART_GetState(&huart1)==HAL_UART_STATE_READY)
		{
			temp_buff=huart1.Instance->DR;
		}
		/*判断接收的数据是否超出最大缓存区*/
		if(uart_cnt<UART_BUFF_LEN)
		{
			uart_rec_buff[uart_cnt]=temp_buff;
			uart_cnt++;
		}	
		else  //数据缓存大于最大值，报错；
		{
			printf("Is error for recive the BIN update flie!\n");
		}	
	}
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_rec_buff, 1);	//串口接收一个字节，并通过中断返回结果
}


int fputc(int ch, FILE *f)
{
	huart1.Instance->DR = ch;
	while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE) == RESET);
	return ch; 
}

/*******增加错误处理复位机制***********/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		switch(huart->ErrorCode)
		{
			case HAL_UART_ERROR_PE: 
						__HAL_UART_CLEAR_PEFLAG(&huart1); 
						break;
			
			case HAL_UART_ERROR_NE: 
						__HAL_UART_CLEAR_NEFLAG(&huart1);
						break;
			
			case HAL_UART_ERROR_FE: 
					 __HAL_UART_CLEAR_FEFLAG(&huart1);
					 break;
			
			case HAL_UART_ERROR_ORE: 
					 __HAL_UART_CLEAR_OREFLAG(&huart1);
					 break;
		}
	}	
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_rec_buff, 1);
}	
