/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   UART配置C文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */


#include "drv_uart.h"
#include "stdio.h"

/**
  * @brief :串口初始化
  * @param :
  *			@UartBaudRate:串口波特率
  * @note  :无
  * @retval:无
  */	
	
void drv_uart_init( uint32_t UartBaudRate )
{
	GPIO_InitTypeDef	UartGpioInitStructer;
	USART_InitTypeDef	UartinitStructer;
	
	//在配置过程中，为防止TX RX不再同一个端口上，增强可移植性，固分开配置
	//初始化串口TX RX 引脚 
	RCC_APB2PeriphClockCmd( UART_TX_GPIO_CLK | UART_RX_GPIO_CLK, ENABLE );	//打开TX RX 端口时钟
	
	UartGpioInitStructer.GPIO_Mode = GPIO_Mode_AF_PP;
	UartGpioInitStructer.GPIO_Speed = GPIO_Speed_50MHz;
	//TX
	UartGpioInitStructer.GPIO_Pin = UART_TX_GPIO_PIN;
	GPIO_Init( UART_TX_GPIO_PORT, &UartGpioInitStructer );		//初始化TX引脚  配置为复用功能
	//RX
	UartGpioInitStructer.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_AF_PP;//GPIO_Mode_AF_OD;//GPIO_Mode_AIN;//GPIO_Mode_IN_FLOATING;
	UartGpioInitStructer.GPIO_Pin = UART_RX_GPIO_PIN;
	GPIO_Init( UART_RX_GPIO_PORT, &UartGpioInitStructer );		//初始化RX引脚  配置为输入
	
	//配置USART外设
	USART_DeInit( UART_PORT );		//外设复位
	
	if( USART1 == UART_PORT )		//使能外设时钟
	{
		RCC_APB2PeriphClockCmd( UART_PORT_CLK, ENABLE );			
	}																	//不同的USART外设可能在不同的APB时钟上														
	else																//STM32F103单片机只有USART1在APB2上，如平台有差异做相应改变即可
	{
		RCC_APB1PeriphClockCmd( UART_PORT_CLK, ENABLE );
	}
	
	UartinitStructer.USART_BaudRate = UartBaudRate;						//设置波特率
	UartinitStructer.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//不使用流控制
	UartinitStructer.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		//发送和接收	
	UartinitStructer.USART_Parity = USART_Parity_No;					//不带校验
	UartinitStructer.USART_StopBits = USART_StopBits_1;					//一个停止位
	#ifdef DEBUG_MODE
	UartinitStructer.USART_WordLength = USART_WordLength_8b;			//8个数据位
	#else
	UartinitStructer.USART_WordLength = USART_WordLength_9b;			//9个数据位
	#endif
	USART_Cmd( UART_PORT, DISABLE );									//失能外设
	USART_Init( UART_PORT, &UartinitStructer );							//初始化外设
	USART_Cmd( UART_PORT, ENABLE );										//使能外设	
}


void UART1_SendData8(uint8_t Data)
{
    /* Transmit Data */
    UART_PORT->DR = Data;
}
/**
  * @brief :串口发送数据
  * @param :
  *			@TxBuffer:发送数据首地址
  *			@Length:数据长度
  * @note  :无
  * @retval:无
  */
void drv_uart_tx_bytes( uint8_t* TxBuffer, uint16_t Length ) 
{
	while( Length-- )
	{
		while( RESET == USART_GetFlagStatus( UART_PORT, USART_FLAG_TXE ));
		UART1_SendData8( * TxBuffer );
		TxBuffer++;
	}
}

/**
  * @brief :串口接收数据
  * @param :
  *			@RxBuffer:发送数据首地址
  * @note  :无
  * @retval:接收到的字节个数
  */
uint8_t drv_uart_rx_bytes( uint8_t* RxBuffer )
{
	uint8_t l_RxLength = 0;
	uint16_t l_UartRxTimOut = 0x7FFF;
	
	while( l_UartRxTimOut-- )			//等待查询串口数据
	{
		if( RESET != USART_GetFlagStatus( UART_PORT, USART_FLAG_RXNE ))
		{
			*RxBuffer = (uint8_t)UART_PORT->DR;
			RxBuffer++;
			l_RxLength++;
			l_UartRxTimOut = 0x7FFF;	//接收到一个字符，回复等待时间
		}
		if( 64 == l_RxLength )
		{
			break;		//不能超过64个字节
		}
	}
	
	return l_RxLength;					//等待超时，数据接收完成
}

/**
  * @brief :串口接收数据十六进制转化成十进制
  * @param :
  *			@RxBuffer:接收数据首地址
  * @note  :无
  * @retval:返回接收的十进制数值
  */
uint32_t hex_to_dec(uint8_t *RxBuffer)
{ 
	uint32_t temp[5];
	uint8_t i;
	for(i=0;i<5;i++)
	{
		switch(RxBuffer[14+i])
		{
			case '0':temp[i]=0;break;
			case '1':temp[i]=1;break;
			case '2':temp[i]=2;break;
			case '3':temp[i]=3;break;
			case '4':temp[i]=4;break;
			case '5':temp[i]=5;break;
			case '6':temp[i]=6;break;
			case '7':temp[i]=7;break;
			case '8':temp[i]=8;break;
			case '9':temp[i]=9;break;
			default: return 55;
		}
	}
	return temp[0]*10000+temp[1]*1000+temp[2]*100+temp[3]*10+temp[4];
}	




struct __FILE  
{  
int handle;  
/* Whatever you require here. If the only file you are using is */  
/* standard output using printf() for debugging, no file handling */  
/* is required. */  
};  
/* FILE is typedef’ d in stdio.h. */  
FILE __stdout;  

/// <summary>  
/// 定义_sys_exit()以避免使用半主机模式  
/// </summary>  
/// <param name="x"></param>  
/// <returns></returns>  
_sys_exit(int x)   
{  
x = x;  
}  



int fputc(int ch, FILE *f) 
{ 
    //USART_SendData(USART1, (u8) ch); 
    USART1->DR = (u8) ch; 
     
    /* Loop until the end of transmission */ 
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    { 
    } 

    return ch; 
} 
