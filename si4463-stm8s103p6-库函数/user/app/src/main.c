/**
  ******************************************************************************
  * @author  ��ҫ�Ƽ� ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   ������C�ļ�
  ******************************************************************************
  * @attention
  *
  * ����	:	http://www.ashining.com
  * �Ա�	:	https://shop105912646.taobao.com
  * ����Ͱ�:	https://cdzeyao.1688.com
  ******************************************************************************
  */



#include "main.h"							//main.h �к���TX/RX�����SPI/Ӳ��SPIѡ������ѡ��
#include "user_flash.h"
#include "drv_SI4438.h"

char  g_Ashining[] = "ashining Tx is00000 123456789123456789123456789123456789123456789123456789";
uint8_t g_TxMode = 0, g_UartRxFlag = 0;
uint8_t g_UartRxBuffer[ 64 ] = { 0 };
uint8_t g_SI4463ItStatus[ 9 ] = { 0 };
uint8_t g_SI4463RxBuffer[ 64 ] = { 0 }; 
uint8_t channel=0;


/**
  * @brief :������
  * @param :��
  * @note  :��
  * @retval:��
  */
int main( void )
{	
	uint16_t i = 0;

	//���ڳ�ʼ�� ������Ĭ������Ϊ9600
	drv_uart_init( 9600 );
	
	//SPI��ʼ��
	drv_spi_init( );
	user_write_flash(CHANNLE_MESSAGE_ROM,0);
        channel=flash_channel();
	//SI4463��ʼ��
	SI446x_Init();
			
	
#ifdef	__SI4438_TX_TEST__		
//=========================================================================================//	
//*****************************************************************************************//
//************************************* ���� **********************************************//
//*****************************************************************************************//
//=========================================================================================//	
		
	while( 1 )	
	{
		//ģʽ1 1S���͡�ashining��һ��
		if( TX_MODE_1 == g_TxMode )
		{
			//��̬���ݳ���
			#if PACKET_LENGTH == 0                              
				SI446x_Send_Packet( (uint8_t *)g_Ashining,60, channel, 0 ); 
                                SI446x_Interrupt_Status( g_SI4463ItStatus );
			#else
				SI446x_Send_Packet( (uint8_t *)g_Ashining, PACKET_LENGTH, channel, 0 );
			#endif
			drv_delay_ms( 100 );             
                        g_Ashining[18]++;
                        if(g_Ashining[18]>'9')
                        {
                         g_Ashining[18]='0';
                         g_Ashining[17]++;
                         if(g_Ashining[17]>'9')
                         {
                          g_Ashining[17]='0';
                          g_Ashining[16]++;
                          if(g_Ashining[16]>'9')
                          {
                            g_Ashining[16]='0';
                            g_Ashining[15]++;
                            if(g_Ashining[15]>'9')
                            {
                              g_Ashining[15]='0';
                              g_Ashining[14]++;                             
                              if(g_Ashining[14]>'9')  
                              {
                                g_Ashining[14]='F';
                                g_Ashining[15]='F';
                                g_Ashining[16]='F';
                                g_Ashining[17]='F';
                                g_Ashining[18]='F';
                              }
                            } 
                          } 
                         } 
                        }
                        
		}
		else	//ģʽ2 �ⲿͨ�����ڷ������ݵ���Ƭ������Ƭ��ͨ��SI4463�����ݷ��ͳ�ȥ
		{	
			//��ѵ���մ�������
			i = drv_uart_rx_bytes( g_UartRxBuffer );
			
			if( 0 != i )
			{
				if( 17 < i )
				{
					i = 0;		//һ�β�����17���ֽ�
				}
				#if PACKET_LENGTH == 0
					SI446x_Send_Packet( (uint8_t *)g_UartRxBuffer, i, channel, 0 );
				#else
					SI446x_Send_Packet( (uint8_t *)g_UartRxBuffer, PACKET_LENGTH, channel, 0 );
				#endif
			}
		}
              
	}
	
#else		
//=========================================================================================//	
//*****************************************************************************************//
//************************************* ���� **********************************************//
//*****************************************************************************************//
//=========================================================================================//	
	
	while( 1 )
	{
		SI446x_Interrupt_Status( g_SI4463ItStatus );		//��ѯ�ж�״̬
		
		if( g_SI4463ItStatus[ 3 ] & ( 0x01 << 4 ))
        {
			i = SI446x_Read_Packet( g_SI4463RxBuffer );		//�����յ�������
			if( i != 0 )
			{
				drv_uart_tx_bytes( g_SI4463RxBuffer,i );	//�������SI4463���յ�������
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

