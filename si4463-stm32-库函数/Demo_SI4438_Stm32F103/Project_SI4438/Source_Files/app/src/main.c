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
#include  "modbus_crc.h"  
#include "user_flash.h"
uint8_t g_TxMode = 0, g_UartRxFlag = 0;
uint8_t g_SI4463ItStatus[ 9 ] = { 0 };
uint8_t g_SI4463RxBuffer[ 600 ] = { 0 };  
uint8_t a_SI4463RxBuffer[ 64 ] = { 0 };
struct PacketrxData PacketrxData; 
struct LongPacketData LongPacketData;
uint8_t error_cnt=0;
uint8_t channel=0;

void invailid_preamble(uint8_t invailid_cnt,uint16_t change_time,uint8_t *buff);
/**
  * @brief :主函数 
  * @param :无
  * @note  :无
  * @retval:无
  */ 
int main( void )
{	
	uint16_t i = 0;
	uint8_t temp=0;
	//串口初始化波特率 9600
	drv_uart_init( 115200 );
	
	//延时初始化
	drv_delay_init( );
//	handle_cnt_init();
//	EXTIX_Init(); 
	
	//LED初始化
	drv_led_init( );
	
	//SPI初始化
	drv_spi_init( );
	user_flash_write(CHANNLE_MESSAGE_ROM,1);
  channel=channle_read_data(CHANNLE_MESSAGE_ROM);  
  if(channel==0xff)   
  channel=0;		
	//SI4463初始化	
	SI446x_Init( );
	led_red_off( );
	led_green_off( );
	for( i = 0; i < 6; i++ )		//模块初始化完成，LED灯闪烁3个周期
	{
		led_red_flashing( );
		led_green_flashing( );
//		drv_delay_500Ms( 1 );
	}	
#ifdef	__SI4438_TX_TEST__		
//=========================================================================================//	
//*****************************************************************************************//
//************************************* 发送 **********************************************//
//*****************************************************************************************//
//=========================================================================================//		
	
#else		
//=========================================================================================//	
//*****************************************************************************************//
//************************************* 接收 **********************************************//
//*****************************************************************************************//
//=========================================================================================//	
	
//	while( 1 )
//	{
//		SI446x_Interrupt_Status( g_SI4463ItStatus );		//读中断状态
//		
//		if( g_SI4463ItStatus[ 3 ] & ( 0x01 << 4 ))
//        {
//			i = SI446x_Read_Packet( g_SI4463RxBuffer );		//读FIFO数据
//			if( i != 0 )
//			{
//				led_green_flashing( );
//				drv_uart_tx_bytes( g_SI4463RxBuffer,i );	//输出接收到的字节
//			}
//		
//			SI446x_Change_Status( 6 );
//			while( 6 != SI446x_Get_Device_Status( ));
//			SI446x_Start_Rx(  0, 0, PACKET_LENGTH,0,0,3 );
//		}
//		else
//		{
//			if( 3000 == i++ )
//			{
//				i = 0;
//				SI446x_Init( );
//			}
//			drv_delay_ms( 1 );
//		}
//	}
			
	
	
	while( 1 )
	{				
		SI446x_Interrupt_Status( g_SI4463ItStatus );		//查询中断状态	
		if(get_timer3_flag())
		{			
			clr_timer3_flag();			
			invailid_preamble(10,1,g_SI4463ItStatus);	
		}		
		if( g_SI4463ItStatus[ 3 ] & ( 0x01 << 4 ))   
    {
			TIM_Cmd( HANDLE_TIME_BASE, DISABLE );				
			i = SI446x_Read_Packet( a_SI4463RxBuffer );		//读接收到的数据
			if( a_SI4463RxBuffer[0]==1 )
			{
				PacketrxData.flag=1;					
			}	
			if(a_SI4463RxBuffer[0]==0x55)
			{
				PacketrxData.flag=1;	
				temp=0x55;
			}	
			if(PacketrxData.flag)
			{				
				switch(temp)
				{
					case 0:temp++;
								 memcpy(g_SI4463RxBuffer,a_SI4463RxBuffer,64);
					       if(check_crc(a_SI4463RxBuffer, 64)==0)
								 {
									error_cnt++;
									temp=0;
									PacketrxData.flag=0;									 
									memset(g_SI4463RxBuffer,0,512);
								 } 
								 break;
					case 1:temp++;
								 memcpy(g_SI4463RxBuffer+64,a_SI4463RxBuffer,64);
					       if(check_crc(a_SI4463RxBuffer, 64)==0)
								 {
									error_cnt++;
									temp=0;
									PacketrxData.flag=0;									 
									memset(g_SI4463RxBuffer,0,512);
								 } 								 
								 break;
					case 2:temp++;
								 memcpy(g_SI4463RxBuffer+128,a_SI4463RxBuffer,64);
					       if(check_crc(a_SI4463RxBuffer, 64)==0)
								 {
									error_cnt++;
									temp=0;
									PacketrxData.flag=0;									 
									memset(g_SI4463RxBuffer,0,512); 
								 } 								 
								 break;
					case 3:temp++;
								 memcpy(g_SI4463RxBuffer+192,a_SI4463RxBuffer,64);
					       if(check_crc(a_SI4463RxBuffer, 64)==0)
								 {
									error_cnt++;
									temp=0;
									PacketrxData.flag=0;								 
									memset(g_SI4463RxBuffer,0,512); 
								 } 								 
								 break;		
					case 4:temp++;
								 memcpy(g_SI4463RxBuffer+256,a_SI4463RxBuffer,64);
					       if(check_crc(a_SI4463RxBuffer, 64)==0)
								 {
									error_cnt++;
									temp=0;
									PacketrxData.flag=0;									 
									memset(g_SI4463RxBuffer,0,512); 
								 } 								 
								 break;
					case 5:temp++;
								 memcpy(g_SI4463RxBuffer+320,a_SI4463RxBuffer,64);
					       if(check_crc(a_SI4463RxBuffer, 64)==0)
								 {
									error_cnt++;
									temp=0;
									PacketrxData.flag=0;									 
									memset(g_SI4463RxBuffer,0,512); 
								 } 								 
								 break;
					case 6:temp++;
								 memcpy(g_SI4463RxBuffer+384,a_SI4463RxBuffer,64);
					       if(check_crc(a_SI4463RxBuffer, 64)==0)
								 {
									error_cnt++;
									temp=0;
									PacketrxData.flag=0;									 
									memset(g_SI4463RxBuffer,0,512); 
								 } 								 
								 break;
					case 7:temp++;
								 memcpy(g_SI4463RxBuffer+448,a_SI4463RxBuffer,64);
					       if(check_crc(a_SI4463RxBuffer, 64)==0)
								 {
									error_cnt++;
									temp=0;
									PacketrxData.flag=0;									 
									memset(g_SI4463RxBuffer,0,512); 
								 } 								 
								 break;	
					case 8:temp=0;
								 TIM_SetCounter(TIM3, 50000);	
								 memcpy(g_SI4463RxBuffer+512,a_SI4463RxBuffer,64);
								 g_SI4463RxBuffer[514]=channel;
					       if(check_crc(a_SI4463RxBuffer, 64)==0)
								 {
									error_cnt++;
									temp=0;
									PacketrxData.flag=0;									 
									memset(g_SI4463RxBuffer,0,512); 
								 } 								 
								 PacketrxData.flag=0;
								 error_cnt=0;
								 drv_uart_tx_bytes( g_SI4463RxBuffer,576 );	//串口输出SI4463接收到的数据
								 break;	
					case 0x55:
					       if(check_crc(a_SI4463RxBuffer, 64)==1)						
								 {
									  temp=0;		
										PacketrxData.flag=0;
									  if(a_SI4463RxBuffer[1]==a_SI4463RxBuffer[2])
									  {
											channel=a_SI4463RxBuffer[1];
											user_flash_write(CHANNLE_MESSAGE_ROM,channel);
										}
								 }
								    break;
					default:temp=0;
									PacketrxData.flag=0;
									break;
				}				
			}	
			SI446x_Change_Status( 6 );
			while( 6 != SI446x_Get_Device_Status( ));
			SI446x_Start_Rx(channel, 0, PACKET_LENGTH,0,0,3 );
		}
//		else
//		{
//			if( 3000 == i++ )
//			{
//				i = 0;
//				SI446x_Init( );
//			}
//			drv_delay_ms( 1 );
//		}		
		TIM3->CR1|=0x0001;
	}	
#endif
}
/**
  * @brief :检测无效的前导码
  * @param :invailid_cnt  无效的次数设置
  * @param :change_time   每个信道切换的时间500ms
  * @param :uint8_t *buff IT的保存数组指针
  * @note  :无
  * @retval:无
  */
void invailid_preamble(uint8_t invailid_cnt,uint16_t change_time,uint8_t *buff)
{
			channel++;
			if(channel>148)
			channel=0;	
			SI446x_Change_Status( 6 );
			while( 6 != SI446x_Get_Device_Status( ));
			SI446x_Start_Rx(channel, 0, PACKET_LENGTH,0,0,3 );		
      user_flash_write(CHANNLE_MESSAGE_ROM,channel);			
}	
