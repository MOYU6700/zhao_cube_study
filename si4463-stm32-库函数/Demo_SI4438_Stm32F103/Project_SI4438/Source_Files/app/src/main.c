/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   主函数C文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */



#include "main.h"				//main.h 中含有TX/RX、软件SPI/硬件SPI选择配置选项
#include "string.h"
#include "drv_uart.h"
#include  "stdio.h"
#include  "drv_io.h"
#include  "user_config.h"   

const char *g_Ashining = "ashining";
uint8_t g_TxMode = 0, g_UartRxFlag = 0;
uint8_t g_UartRxBuffer[ 64 ] = { 0 };
uint8_t g_SI4463ItStatus[ 9 ] = { 0 };
uint8_t g_SI4463RxBuffer[ 520 ] = { 0 }; 
uint8_t a_SI4463RxBuffer[ 64 ] = { 0 };
struct PacketrxData PacketrxData; 
struct LongPacketData LongPacketData;
uint32_t counter=0;
/**
  * @brief :主函数 
  * @param :无
  * @note  :无
  * @retval:无
  */ 
int main( void )
{	
	uint16_t i = 0;
	//串口初始化波特率 9600
	drv_uart_init( 115200 );
	
	//延时初始化
	drv_delay_init( );
	
//	EXTIX_Init(); 
	
	//LED初始化
	drv_led_init( );
	
	//SPI初始化
	drv_spi_init( );

	//SI4463初始化	
	SI446x_Init( );
	
	led_red_off( );
	led_green_off( );
	for( i = 0; i < 6; i++ )		//模块初始化完成，LED灯闪烁3个周期
	{
		led_red_flashing( );
		led_green_flashing( );
		drv_delay_500Ms( 1 );
	}
//	LongPacketData.TxlengthGet=1;
		
	
#ifdef	__SI4438_TX_TEST__		
//=========================================================================================//	
//*****************************************************************************************//
//************************************* 发送 **********************************************//
//*****************************************************************************************//
//=========================================================================================//	
	
	//按键初始化
	drv_button_init( );				//Demo程序中 只有在发送时才会使用按键
	
	while( 1 )	
	{
		//模式切换
		if( BUTOTN_PRESS_DOWN == drv_button_check( ))	//检查按键动作
		{
			g_TxMode = 1 - g_TxMode;		//模式会在 TX_MODE_1( 0 ),TX_MODE_2( 1 )之间切换
			
			//状态显示清零
			led_green_off( );
			led_red_off( );
			
			if( TX_MODE_1 == g_TxMode )
			{
				for( i = 0; i < 6; i++ )		//固定发送模式，红灯闪烁3次
				{
					led_red_flashing( );	
					drv_delay_500Ms( 1 );		
				}
			}
			else
			{
				for( i = 0; i < 6; i++ )		//串口发送模式，绿灯闪烁3次
				{
					led_green_flashing( );	
					drv_delay_500Ms( 1 );
				}
			}
		}
		
		//发送固定字符串
		if( TX_MODE_1 == g_TxMode )
		{
			//发送数据
			#if PACKET_LENGTH == 0
				SI446x_Send_Packet( (uint8_t *)g_Ashining, 8, 0, 0 );
			#else
				SI446x_Send_Packet( (uint8_t *)g_Ashining, PACKET_LENGTH, 0, 0 );
			#endif
			drv_delay_500Ms( 1 );	
			led_red_flashing( );			//1S左右发送一包 每发送一包红灯闪烁一次
			drv_delay_500Ms( 1 );	
		}
		else	//发送串口接收到的字符串
		{	
			//查询串口数据
			i = drv_uart_rx_bytes( g_UartRxBuffer );
			
			if( 0 != i )
			{
				if( 16 < i )
				{
					i = 0;
				}
				#if PACKET_LENGTH == 0
					SI446x_Send_Packet( (uint8_t *)g_UartRxBuffer, i, 0, 0 );
				#else
					SI446x_Send_Packet( (uint8_t *)g_UartRxBuffer, PACKET_LENGTH, 0, 0 );
				#endif
				led_red_flashing( );
			}
		}
	}	
#else		
//=========================================================================================//	
//*****************************************************************************************//
//************************************* 接收 **********************************************//
//*****************************************************************************************//
//=========================================================================================//	

	while( 1 )
	{
		SI446x_Interrupt_Status( g_SI4463ItStatus );		//查询中断状态
		
		if( g_SI4463ItStatus[ 3 ] & ( 0x01 << 4 ))   
    {
			i = SI446x_Read_Packet( g_SI4463RxBuffer );		//读接收到的数据
			if( i != 0 )
			{
				drv_uart_tx_bytes( g_SI4463RxBuffer,i );	//串口输出SI4463接收到的数据
			}
		
			SI446x_Change_Status( 6 );
			while( 6 != SI446x_Get_Device_Status( ));
			SI446x_Start_Rx(  0, 0, PACKET_LENGTH,0,0,3 );
		}
		else
		{
			if( 3000 == i++ )
			{
				i = 0;
				SI446x_Init( );
			}
			drv_delay_ms( 1 );
		}
	}
	
//	SI446x_Change_Status( 6 );
//	while( 6 != SI446x_Get_Device_Status( ));
//	SI446x_Start_Rx(  0, 0, PACKET_LENGTH,0,0,3 );
//	
//	while( 1 )
//	{
//		SI446x_Interrupt_Status( g_SI4463ItStatus );		//查询中断状态
//		
//		if ((g_SI4463ItStatus[3]&(0x01<<4)) ||
//				(g_SI4463ItStatus[3]&(0x01<<0)))
//        {
//			i = i+SI446x_Read_Packet( g_SI4463RxBuffer );		//读接收到的数据
//			if(g_SI4463ItStatus[3]&(0x01<<4))
//			{				
////				drv_uart_tx_bytes( g_SI4463RxBuffer,i );	//串口输出SI4463接收到的数据
//				drv_uart_tx_bytes( (uint8_t *)&i,1 );	//串口输出SI4463接收到的数据
//				i = 0;
//			}
////			i = 0;
//		
////			SI446x_Change_Status( 6 );
////			while( 6 != SI446x_Get_Device_Status( ));
////			SI446x_Start_Rx(  0, 0, PACKET_LENGTH,0,0,3 );
//		}
//		else
//		{
//			if( 30000 == i++ )
//			{
//				i = 0;
//				SI446x_Init( );
//			}
//			drv_delay_ms( 1 );
//		}
//	}	
	
//	while( 1 )
//	{				
//				/*发送当前接收到的数据*/
//				if(LongPacketData.sent_buff)
//				{	
//					
//					counter++;
//					if(counter%100)
//					{
//						led_green_flashing( );
//						
//					}	
//					drv_uart_tx_bytes( g_SI4463RxBuffer,512 );	
//          LongPacketData.sent_buff=0;					
//				}
//	}		

#endif
}


