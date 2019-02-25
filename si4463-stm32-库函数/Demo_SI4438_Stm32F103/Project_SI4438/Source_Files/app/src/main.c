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
struct PacketrxData PacketrxData; 
struct LongPacketData LongPacketData;
uint8_t channel=0;
uint16_t count=0;
void SystemTimeSet(unsigned char value);
void invailid_preamble(void);
void dmx_sendpacket(void);
void dmx_init(void); //DMX512初始化

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
	#ifdef DEBUG_MODE
	drv_uart_init( 115200 );
	#else
	drv_uart_init( 250000 );
	#endif
	
	//延时初始化
	drv_delay_init( );
	TIME4_init( ); 
	EXTIX_Init(); 
	
	//LED初始化
	drv_led_init( );
	
	//SPI初始化
	drv_spi_init( );
	user_flash_write(CHANNLE_MESSAGE_ROM,0);
  channel=channle_read_data(CHANNLE_MESSAGE_ROM);  
  if(channel==0xff)   
  channel=0;		
	//SI4463初始化	
	SI446x_Init( );
	led_red_off( );
	led_green_off( );
	dmx_init() ;//DMX512初始化
	for( i = 0; i < 6; i++ )		//模块初始化完成，LED灯闪烁3个周期
	{
		led_red_flashing( );
		led_green_flashing( );
		drv_delay_500Ms( 1 );
	}	
		handle_cnt_init();
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
	while( 1 )
	{		
		if(get_timer3_flag())
		{			
			clr_timer3_flag();	
			PacketrxData.dm512_cnt++;	
			if(PacketrxData.dm512_cnt>=20)
			{
				PacketrxData.dm512_cnt=0;
				invailid_preamble();	
				led_green_flashing( );
			}	
		}		
   if(PacketrxData.protection_flag)
	 {
		 SAFE(dmx_sendpacket());	
		 PacketrxData.protection_flag=0;
	 } 
//		else
//		{
//			if( 3000 == count++ )
//			{
//				count = 0;
//				SI446x_Init( );
//			}
//			drv_delay_ms( 1 );
//		}				
	}	
#endif 
}

void dmx_init(void) //DMX512初始化
{
	int i;
  g_SI4463RxBuffer[0] = 0; //起始码00
  for(i = 1; i<=512; i++)
  {
     g_SI4463RxBuffer[i] = 0;
  }
//	g_SI4463RxBuffer[10]=0xff;    //步进电机粗调
//	g_SI4463RxBuffer[11]=0xff;    //步进电机细调
//	g_SI4463RxBuffer[14]=0xff;    //R色调节
//	g_SI4463RxBuffer[15]=0xff;    //G色调节
//	g_SI4463RxBuffer[16]=0xff;    //B色调节	
//	g_SI4463RxBuffer[17]=0xff;
}

/**
  * @brief :检测无效的前导码
  * @param :invailid_cnt  无效的次数设置
  * @param :change_time   每个信道切换的时间500ms
  * @param :uint8_t *buff IT的保存数组指针
  * @note  :无
  * @retval:无
  */
void invailid_preamble(void)
{
			channel++;
			if(channel>11)
			channel=0;		
			SI446x_Init( );
      user_flash_write(CHANNLE_MESSAGE_ROM,channel);			
}	

void gpio_tx_config(uint8_t Set)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  //设置发送的引脚为普通IO
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	if(0 == Set)
	{
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //普通IO
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用IO
	}
  GPIO_Init(GPIOA, &GPIO_InitStructure); //引脚初始化
}

uint16_t pDMX_buf = 0;
void dmx_sendpacket(void) //发送DMX512数据
{
	//配置为普通IO
	pDMX_buf = 0;
	gpio_tx_config(0);					//设置发送的引脚为普通IO
	GPIO_WriteBit(GPIOA, GPIO_Pin_9,Bit_RESET); 										//输出低电平
	delay_us(150); 							//延时150us
	GPIO_WriteBit(GPIOA, GPIO_Pin_9,Bit_SET); ; 										//输出高电平
	delay_us(13); 							//延时13us
	gpio_tx_config(1);					//设置发送的引脚为普通IO
		
	while(pDMX_buf <= 512) //1-512
  {
		while((USART1->SR&0X40)==0){};//循环发送,直到发送完毕
    if(USART1->SR & (1<<6))
    { 
       /*发送起始码 00*/
			 USART1->DR =0x0100 | g_SI4463RxBuffer[pDMX_buf];
       pDMX_buf++;
     }  
  }
}
