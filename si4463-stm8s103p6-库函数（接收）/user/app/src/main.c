#include "main.h"							//main.h �к���TX/RX�����SPI/Ӳ��SPIѡ������ѡ��
#include "user_flash.h"
#include "drv_SI4438.h"
#include "drv_io.h"
#include "user_config.h"
#include "modbus_crc.h"
#include "drv_delay.h"

uint8_t g_TxMode = 0, g_UartRxFlag = 0;
struct PacketrxData PacketrxData; 
uint8_t channel=0;
uint16_t count=0;
void invailid_preamble(void);
void dmx_sendpacket(void);
void dmx_init(void); //DMX512��ʼ��
/**
  * @brief :������
  * @param :��
  * @note  :��
  * @retval:��
  */
int main( void )
{		
        CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	//���ڳ�ʼ�� ������Ĭ������Ϊ250000
	drv_uart_init( 250000 );
        GPIO_Config();	
        TIM1_Config();//��ʱ��1��ʼ��
        TIM2_Config();//��ʱ��2��ʼ��	
	//SPI��ʼ��
	drv_spi_init( );	
//        user_write_flash(CHANNLE_MESSAGE_ROM,0);
        channel=user_read_flash(CHANNLE_MESSAGE_ROM);  
        if(channel>11)   
        channel=0;		
	//SI4463��ʼ��
	SI446x_Init();
        LED_Init(); 
        dmx_init() ;//DMX512��ʼ��        
#ifdef	__SI4438_TX_TEST__		
//=========================================================================================//	
//*****************************************************************************************//
//************************************* ���� **********************************************//
//*****************************************************************************************//
//=========================================================================================//			
	
#else		
//=========================================================================================//	
//*****************************************************************************************//
//************************************* ���� **********************************************//
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
         dmx_sendpacket();
         LED1_Toggle();
         PacketrxData.protection_flag=0;
       } 			
      }	
#endif 
}

void dmx_init(void) //DMX512��ʼ��
{
  int i;
  g_SI4463RxBuffer[0] = 0; //��ʼ��00
  for(i = 1; i<=512; i++)
  {
     g_SI4463RxBuffer[i] = 0;
  }
}

/**
  * @brief :�����Ч��ǰ����
  * @param :invailid_cnt  ��Ч�Ĵ�������
  * @param :change_time   ÿ���ŵ��л���ʱ��500ms
  * @param :uint8_t *buff IT�ı�������ָ��
  * @note  :��
  * @retval:��
  */
void invailid_preamble(void)
{
    channel++;
    if(channel>11)
    channel=0;		
    SI446x_Init( );
    user_write_flash(CHANNLE_MESSAGE_ROM,channel);			
}	

void gpio_tx_config(uint8_t Set)
{
    if(0 == Set)
    {
          UART1_Cmd(DISABLE);	
          UART1_DeInit();
//          GPIO_Init( UART_TX_GPIO_PORT, UART_TX_GPIO_PIN,GPIO_MODE_OUT_PP_HIGH_FAST  );
    }
    else
    {
//	UART1_DeInit();		//���ڸ�λ
	//���ڳ�ʼ�� 8λ���� 1��ֹͣλ ��У�� ���ͽ��� �����ʿɱ�
//	UART1_Init( 250000, UART1_WORDLENGTH_9D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TX_ENABLE );
        UART1_Cmd(ENABLE);	//ʹ�ܴ���
    }
}

uint16_t pDMX_buf = 0;
void dmx_sendpacket(void) //����DMX512����
{
	//����Ϊ��ͨIO
	pDMX_buf = 0;
	gpio_tx_config(0);					//���÷��͵�����Ϊ��ͨIO
        GPIO_WriteLow(UART_TX_GPIO_PORT, UART_TX_GPIO_PIN);										//����͵�ƽ
	delay_us(120); 	
        UART1_Cmd(ENABLE);//��ʱ150us
        UART1_Init( 250000, UART1_WORDLENGTH_9D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TX_ENABLE );
        delay_us(10); 	
	GPIO_WriteHigh(UART_TX_GPIO_PORT, UART_TX_GPIO_PIN); 										//����ߵ�ƽ
//	delay_us(13); 							//��ʱ13us
//	gpio_tx_config(1);							
  while(pDMX_buf <= 512) //1-512
  {
    while((UART1->SR&0X40)==0){};//ѭ������,ֱ���������
    if(UART1->SR & (1<<6))
    { 
       /*������ʼ�� 00*/
       UART1->DR =0x0100 | g_SI4463RxBuffer[pDMX_buf];
       pDMX_buf++;
     }  
  }
}			

