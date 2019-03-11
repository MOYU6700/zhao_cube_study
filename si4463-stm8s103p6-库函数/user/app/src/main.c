#include "main.h"							//main.h 中含有TX/RX、软件SPI/硬件SPI选择配置选项
#include "user_flash.h"
#include "drv_SI4438.h"
#include "drv_io.h"
#include "string.h"
#include "user_config.h"
#include "modbus_crc.h"
#include "iwdg.h"

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
static uint16_t timer_cnt_ms=0;
struct PacketTxData PacketTxData;
struct LongPacketData LongPacketData;
void set_packages(uint8_t *address,uint16_t len);
void check_for_route(uint8_t rssi_threshold);
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
//        user_write_flash(CHANNLE_MESSAGE_ROM,0);
        channel=flash_channel();
	//SI4463初始化
	SI446x_Init();
        GPIO_Config();	
        LED_Init();       
        pre_channel=channel;
        IWDG_Configuration();
//	PacketTxData.buf[9]=0xff;    //步进电机粗调
//	PacketTxData.buf[10]=0xff;    //步进电机细调
//	PacketTxData.buf[13]=0xff;    //R色调节
//	PacketTxData.buf[14]=0xff;    //G色调节
//	PacketTxData.buf[15]=0xff;    //B色调节	
//	PacketTxData.buf[16]=0xff;    
#ifdef	__SI4438_TX_TEST__		
//=========================================================================================//	
//*****************************************************************************************//
//************************************* 发送 **********************************************//
//*****************************************************************************************//
//=========================================================================================//			
	while( 1 )	
	{          
 
                      /*检测当前的信号是否空闲*/                  
                     if(timer_cnt_ms++>=30000)
                     {
                       timer_cnt_ms=0;
                       SI446x_Change_Status( 6 );
                       while( 6 != SI446x_Get_Device_Status( ));
                       SI446x_Start_Rx(  channel, 0, PACKET_LENGTH,0,0,3 );
                       check_for_route(0x7F);   
                     }
                     if(PacketTxData.DMXSignalFlag==1)
                     {
                       PacketTxData.DMXSignalFlag=0;    
                       LED1_Toggle(); 
		       #if PACKET_LENGTH == 0                     
                     SI446x_Send_Packet( (uint8_t *)tx_packet, PACKET_LENGTH, channel, 0 ); 
                     drv_delay_ms( 2000 );   
			#else	   
                           set_packages(PacketTxData.buf,512);
			#endif 
                     }
                     IWDG_ReloadCounter();
			//外部通过串口发送数据到单片机，单片机通过SI4463将数据发送出去            
	}
	
#else		
//=========================================================================================//	
//*****************************************************************************************//
//************************************* 接收 **********************************************//
//*****************************************************************************************//
//=========================================================================================//	
			
#endif	
}

void set_packages(uint8_t *address,uint16_t len)
{
  uint16_t timeout_mark=0;
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
    while((!LongPacketData.TxlengthGet)||timeout_mark>=1000)
    {     
      drv_delay_ms( 1 );
      timeout_mark++;  
    } 
    LongPacketData.TxlengthGet =0;
   }  
  
}

void set_channel_ifo(uint8_t chain)
{
  uint16_t timeout_mark=0;
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
  while((!LongPacketData.TxlengthGet)||timeout_mark>=1000)
  {
    drv_delay_ms( 1 );
    timeout_mark++;  
  }
  LongPacketData.TxlengthGet =0;   
}

/**
  * @brief  *检测当前的信号是否空闲*
  * @param  设置RSSI的阀值
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
     if(pre_channel >11)   //470MHZ
      {
        pre_channel=0;
      }
     /*写入FLASH保存,并用最开始的*/
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
