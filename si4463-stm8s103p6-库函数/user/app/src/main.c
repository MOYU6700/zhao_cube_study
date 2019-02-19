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
#include "drv_io.h"
#include "string.h"
#include "user_config.h"
#include "modbus_crc.h"

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
uint8_t rssi_SI4463ItStatus[ 9 ] = { 0 };
uint8_t g_SI4463RxBuffer[ 64 ] = { 0 }; 
uint8_t channel=0;
uint8_t si4463_tx_buff[64]={0};
uint8_t pre_channel=0;
struct PacketTxData PacketTxData;
struct LongPacketData LongPacketData;
void set_packages(uint8_t *address,uint16_t len);
void check_for_route(uint8_t rssi_threshold);
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
	//SPI��ʼ��
	drv_spi_init( );	
        user_write_flash(CHANNLE_MESSAGE_ROM,0);
        channel=flash_channel();
	//SI4463��ʼ��
	SI446x_Init();
        GPIO_Config();	 
        pre_channel=channel;
#ifdef	__SI4438_TX_TEST__		
//=========================================================================================//	
//*****************************************************************************************//
//************************************* ���� **********************************************//
//*****************************************************************************************//
//=========================================================================================//			
	while( 1 )	
	{          
 
                      /*��⵱ǰ���ź��Ƿ����*/
                     SI446x_Change_Status( 6 );
                     while( 6 != SI446x_Get_Device_Status( ));
		     SI446x_Start_Rx(  channel, 0, PACKET_LENGTH,0,0,3 );
                     check_for_route(0x50);   
                     if(PacketTxData.DMXSignalFlag)
                     {
                       PacketTxData.DMXSignalFlag=0;                 
		       #if PACKET_LENGTH == 0                     
                     SI446x_Send_Packet( (uint8_t *)tx_packet, PACKET_LENGTH, channel, 0 ); 
                     drv_delay_ms( 2000 );   
			#else	   
                           set_packages(PacketTxData.buf,512);                                
			#endif 
                     }      
			//�ⲿͨ�����ڷ������ݵ���Ƭ������Ƭ��ͨ��SI4463�����ݷ��ͳ�ȥ            
	}
	
#else		
//=========================================================================================//	
//*****************************************************************************************//
//************************************* ���� **********************************************//
//*****************************************************************************************//
//=========================================================================================//	
			
#endif	
}

void set_packages(uint8_t *address,uint16_t len)
{
  uint8_t loop_counter=0;
  uint8_t remain_byte=0; 
  uint16_t temp_value=0;
  uint8_t crc_lsb=0;
  uint8_t crc_msb=0;
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
      temp_value=crc16(si4463_tx_buff, 62);
      crc_lsb=temp_value;
      crc_msb=temp_value>>8;      
      si4463_tx_buff[62]=crc_lsb;
      si4463_tx_buff[63]=crc_msb;       
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
      si4463_tx_buff[1]=channel;     
      memcpy(si4463_tx_buff+2,addr,remain_byte);
      temp_value=crc16(si4463_tx_buff, 62);
      crc_lsb=temp_value;
      crc_msb=temp_value>>8;      
      si4463_tx_buff[62]=crc_lsb;
      si4463_tx_buff[63]=crc_msb;      
    } 
    SI446x_Send_Packet( (uint8_t *)si4463_tx_buff, PACKET_LENGTH, channel, 0 ); 
    while(!LongPacketData.TxlengthGet);
    LongPacketData.TxlengthGet =0;
   }  
  
}

void set_channel_ifo(uint8_t chain)
{
  uint16_t temp_value=0;
  uint8_t crc_lsb=0;
  uint8_t crc_msb=0;
  si4463_tx_buff[0]=0x55;
  si4463_tx_buff[1]=chain;   
  si4463_tx_buff[2]=chain;
  memset(si4463_tx_buff+3,0,59);
  temp_value=crc16(si4463_tx_buff, 62);
  crc_lsb=temp_value;
  crc_msb=temp_value>>8;      
  si4463_tx_buff[62]=crc_lsb;
  si4463_tx_buff[63]=crc_msb;        
  SI446x_Send_Packet( (uint8_t *)si4463_tx_buff, PACKET_LENGTH, channel, 0 ); 
  while(!LongPacketData.TxlengthGet);
  LongPacketData.TxlengthGet =0;   
}

/**
  * @brief  *��⵱ǰ���ź��Ƿ����*
  * @param  ����RSSI�ķ�ֵ
  * @retval None
  */
static uint8_t avr_counter=0;
uint8_t rssi_letch[21]={0};
uint16_t sum_rssi=0;
void check_for_route(uint8_t rssi_threshold)
{
  uint8_t cycle=20;
  SI446x_Modem_Status( rssi_SI4463ItStatus );
  rssi_letch[avr_counter]=rssi_SI4463ItStatus[3];
  sum_rssi+=rssi_letch[avr_counter];
  avr_counter++;
  if(avr_counter>=20)
  {
    rssi_letch[20]=sum_rssi/20;
    sum_rssi=0;
    avr_counter=0;
    if(rssi_letch[20]>=rssi_threshold)
    {
       pre_channel++;
     if(pre_channel >148)   //470MHZ
      {
        pre_channel=0;
      }
     /*д��FLASH����,�����ʼ��*/
     user_write_flash(CHANNLE_MESSAGE_ROM,pre_channel);
     SI446x_Change_Status( 5 );
     while( 5 != SI446x_Get_Device_Status( ));  
     while(cycle)
     {
      set_channel_ifo(pre_channel);
      drv_delay_ms( 50 );
      cycle--;
     }
     channel=pre_channel;
    } 
  } 
}
