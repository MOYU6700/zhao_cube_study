#include "main.h"							//main.h 中含有TX/RX、软件SPI/硬件SPI选择配置选项
#include "user_flash.h"
#include "drv_SI4438.h"
#include "drv_io.h"
#include "user_config.h"
#include "modbus_crc.h"
#include "drv_delay.h"
#include "oled.h"

uint8_t g_TxMode = 0, g_UartRxFlag = 0;
struct PacketrxData PacketrxData; 
uint8_t channel=0;
uint16_t count=0;
uint32_t oled_for_count='0';
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
        CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
 #ifndef max485       
	//串口初始化 波特率默认设置为250000
	drv_uart_init( 250000 );
#else
        drv_uart_init_485( 9600 );
#endif	
        GPIO_Config();	
        TIM1_Config();//定时器1初始化
        TIM2_Config();//定时器2初始化	
	//SPI初始化
	drv_spi_init( );	
//        user_write_flash(CHANNLE_MESSAGE_ROM,0);
        channel=user_read_flash(CHANNLE_MESSAGE_ROM);  
        if(channel>11)   
        channel=0;		
	//SI4463初始化
	SI446x_Init();
        LED_Init(); 
        OLED_Init();	
        dmx_init() ;//DMX512初始化  
//        g_SI4463RxBuffer[2]=0xff;    //步进电机粗调
//	g_SI4463RxBuffer[3]=0xc2;    //步进电机细调
//	g_SI4463RxBuffer[4]=0x7f;    //R色调节
//	g_SI4463RxBuffer[5]=0x7f;    //G色调节
//	g_SI4463RxBuffer[6]=0x7f;    //B色调节	
//	g_SI4463RxBuffer[7]=0x00;  
//        g_SI4463RxBuffer[8]=0x01;
//        g_SI4463RxBuffer[9]=0x02;
//        g_SI4463RxBuffer[10]=0xee;
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
          }	
        }		
   if(PacketrxData.protection_flag)
       {
        oled_for_count++;
        if(oled_for_count>=0xffffff00)
          oled_for_count=0;
         dmx_sendpacket();
         LED1_Toggle();
         PacketrxData.protection_flag=0;
       } 
       oled_test_handle();
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
    user_write_flash(CHANNLE_MESSAGE_ROM,channel);			
}	
 #ifndef max485  
void gpio_tx_config(uint8_t Set)
{
    if(0 == Set)
    {
          UART1_Cmd(DISABLE);	
//          UART1_DeInit();
//          GPIO_Init( UART_TX_GPIO_PORT, UART_TX_GPIO_PIN,GPIO_MODE_OUT_PP_HIGH_FAST  );
    }
    else
    {
//	UART1_DeInit();		//串口复位
	//串口初始化 8位数据 1个停止位 无校验 发送接收 波特率可变
//	UART1_Init( 250000, UART1_WORDLENGTH_9D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TX_ENABLE );
        UART1_Cmd(ENABLE);	//使能串口
    }
}
uint16_t pDMX_buf = 0;
void dmx_sendpacket(void) //发送DMX512数据
{
	//配置为普通IO
	pDMX_buf = 0;
	gpio_tx_config(0);					//设置发送的引脚为普通IO
        GPIO_WriteLow(UART_TX_GPIO_PORT, UART_TX_GPIO_PIN);										//输出低电平	
        delay_us_base(150);
        GPIO_WriteHigh(UART_TX_GPIO_PORT, UART_TX_GPIO_PIN);    
        delay_us_base(13);
	gpio_tx_config(1);							
  while(pDMX_buf <= 512) //1-512
  {
    while((UART1->SR&0X40)==0){};//循环发送,直到发送完毕
    if(UART1->SR & (1<<6))
    { 
       /*发送起始码 00*/
       UART1->DR =0x0100 | g_SI4463RxBuffer[pDMX_buf];
       pDMX_buf++;
     }  
  }
}			
#else
void dmx_sendpacket(void) //发送485数据
{
  uint16_t pDMX_buf = 0;
  while(pDMX_buf <= 8) 
  {
    while((UART1->SR&0X40)==0){};//循环发送,直到发送完毕
    if(UART1->SR & (1<<6))
    { 
       /*发送起始码 00*/
       UART1->DR = g_SI4463RxBuffer[pDMX_buf];
       pDMX_buf++;
     }  
  }
}
#endif
