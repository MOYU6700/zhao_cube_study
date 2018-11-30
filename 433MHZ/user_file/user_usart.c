#include "user_usart.h"
#include "string.h"

/* ff c2 00 00 00 ee */
uint8_t usart2_tx[9]={0x11,0x55,0xaa,0xff, 0xc2 ,00 ,00 ,00 , 0xee};
struct frame frame={0};
static uint8_t usart1_rx[9]={0};
/*初始化串口3*/
void user_io_init(void)
{
	__HAL_UART_ENABLE(&huart1);
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&usart1_rx, 9);
	__HAL_UART_ENABLE(&huart3);
	HAL_UART_Receive_IT(&huart3, (uint8_t *)&frame.data, 1);
}	

/*232串口1发送速度*/
int fputc(int ch, FILE *f)
{
	huart1.Instance->DR = ch;
	while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE) == RESET);
	return ch; 
}

void usart2_send(void) 
{
	HAL_UART_Transmit_DMA(&huart2, usart2_tx , sizeof(usart2_tx));
}	

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		memcpy((uint8_t *)&usart2_tx,(uint8_t *)&usart1_rx,9);
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&usart1_rx, 9);
	}	
	
	if(huart->Instance == USART3)
	{
		if(frame.index  == 0  )   //begin
		{
			frame.buff[frame.index ]=frame.data ;
			frame.index++;
			HAL_UART_Receive_IT(&huart3, (uint8_t *)&frame.data, 1);
			return;
		}
		if(frame.index ==1)   //second
		{
				switch(frame.data)
			{
				case 0x55 : frame.buff[frame.index ]=frame.data ;
										frame.index++;
										break;
				
				case 0xff:  frame.index =0;
										break;
				
				default:    frame.index =0;
										break;
			}	
			HAL_UART_Receive_IT(&huart3, (uint8_t *)&frame.data, 1);
			return;
		}
		
		if(frame.index ==2)   //second
		{
				switch(frame.data)
			{
				case 0xaa : frame.buff[frame.index ]=frame.data ;
										frame.index++;
										break;
				
				case 0xff:  frame.index =0;
										break;
				
				default:    frame.index =0;
										break;
			}	
			HAL_UART_Receive_IT(&huart3, (uint8_t *)&frame.data, 1);
			return;
		}		
		
		if(frame.index >7)  //last
		{
			switch(frame.data )
			{
				case 0xee:  frame.buff[frame.index ]=frame.data ;
				            usart2_tx[5]=0;  
										usart2_tx[6]=0;      
										usart2_tx[7]=0; 
										frame.enable=1;
										frame.index =0;
										break;
				
				default:    frame.index =0;
										break;
			}
			HAL_UART_Receive_IT(&huart3, (uint8_t *)&frame.data, 1);
			return;			
		}
		frame.buff[frame.index ]=frame.data ;
    frame.index++;
		HAL_UART_Receive_IT(&huart3, (uint8_t *)&frame.data, 1);
	}	
}
