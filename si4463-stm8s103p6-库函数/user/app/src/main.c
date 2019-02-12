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



#include "main.h"							//main.h 中含有TX/RX、软件SPI/硬件SPI选择配置选项
#include "user_flash.h"
#include "drv_SI4438.h"
#include "drv_io.h"
#include "string.h"
#include "user_config.h"

uint8_t tx_packet[128] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
96, 97, 98, 99, 100,101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,123, 124, 125, 126, 127};

uint8_t g_TxMode = 0, g_UartRxFlag = 0;
uint8_t g_UartRxBuffer[ 64 ] = { 0 };
uint8_t g_SI4463ItStatus[ 9 ] = { 0 };
uint8_t g_SI4463RxBuffer[ 64 ] = { 0 }; 
uint8_t channel=0;
uint8_t si4463_tx_buff[64]={0};
struct PacketTxData PacketTxData;
struct LongPacketData LongPacketData;
void set_packages(uint8_t *address,uint16_t len);
/**
  * @brief :主函数
  * @param :无
  * @note  :无
  * @retval:无
  */
int main( void )
{		
        CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	//串口初始化 波特率默认设置为250000
	drv_uart_init( 250000 );	
	//SPI初始化
	drv_spi_init( );
	user_write_flash(CHANNLE_MESSAGE_ROM,0);
        channel=flash_channel();
	//SI4463初始化
	SI446x_Init();
        GPIO_Config();	 
//        memset(PacketTxData.buf,2,480);
//        memset(PacketTxData.buf+480,3,32);
#ifdef	__SI4438_TX_TEST__		
//=========================================================================================//	
//*****************************************************************************************//
//************************************* 发送 **********************************************//
//*****************************************************************************************//
//=========================================================================================//			
	while( 1 )	
	{          
 
			//动态数据长度
//                     if(PacketTxData.DMXSignalFlag)
//                     {
                       PacketTxData.DMXSignalFlag=0;                 
		       #if PACKET_LENGTH == 0                     
                     SI446x_Send_Packet( (uint8_t *)tx_packet, PACKET_LENGTH, channel, 0 ); 
                     drv_delay_ms( 2000 );   
			#else	   
                           set_packages(PacketTxData.buf,512);
                           drv_delay_ms( 3000 ); 
			#endif 
//                     }      
			//外部通过串口发送数据到单片机，单片机通过SI4463将数据发送出去            
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
		
#endif	
}

void set_packages(uint8_t *address,uint16_t len)
{
  uint8_t loop_counter=0;
  uint8_t remain_byte=0; 
  uint8_t *addr=0;
  addr=address;
  uint8_t i=0;
  loop_counter=len/60;
  remain_byte=len%60;
  for(i=0;i<(loop_counter+1);i++)
  {
    if(i<loop_counter)
    {
      si4463_tx_buff[0]=i+1;
      si4463_tx_buff[1]=60;
      memcpy(si4463_tx_buff+2,addr,60);
      if(i<loop_counter)
      {
        addr+=60;
      }
      else
      {
        addr+=32;
      }  
    } 
    else
    {
      si4463_tx_buff[0]=i+1;
      si4463_tx_buff[1]=remain_byte;
      memcpy(si4463_tx_buff+2,addr,remain_byte);
    } 
    SI446x_Send_Packet( (uint8_t *)si4463_tx_buff, PACKET_LENGTH, channel, 0 ); 
    while(!LongPacketData.TxlengthGet);
    LongPacketData.TxlengthGet =0;
   }  
  
}
