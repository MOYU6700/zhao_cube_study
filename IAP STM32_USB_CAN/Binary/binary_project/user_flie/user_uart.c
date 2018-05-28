#include "stm32f1xx_hal.h"
#include "user_uart.h"
#include "flash_map.h"
#include "user_boot.h"
#include <string.h>


uint8_t uart_rec_buff[5];

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
static uint8_t index;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t temp_buff;
	if(huart->Instance==USART1)
	{
		temp_buff=huart1.Instance->DR;
		switch(index)  //00 11 22 33 44 
		{
			case 0:if((temp_buff==0x00)&&(index==0))
							index++;
				break;
			case 1:if((temp_buff==0x11)&&(index==1))
							index++;
				break;
			case 2:if((temp_buff==0x22)&&(index==2))
							index++;
				break;
			case 3:if((temp_buff==0x33)&&(index==3))
							index++;
				break;
			case 4:if((temp_buff==0x44)&&(index==4))
							boot_set_update_flag();
							index=0;
							HAL_NVIC_SystemReset();
							while(1);				
			default:
				break;
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
