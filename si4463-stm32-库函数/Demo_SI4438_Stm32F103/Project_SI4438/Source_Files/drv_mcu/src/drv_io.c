#include "drv_io.h"
#include "user_config.h" 
#include "string.h"
#include  "modbus_crc.h" 
#include "user_flash.h"

void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTCʱ��

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
		
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;//PC4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC4
} 

void EXTIX_Init(void)
{
		EXTI_InitTypeDef EXTI_InitStructure; 
		NVIC_InitTypeDef NVIC_InitStructure; 
		//�ⲿ�жϣ���Ҫʹ�� AFIO ʱ��
	  KEY_Init();//��ʼ��������Ӧioģʽ
		//GPIOC.4 �ж����Լ��жϳ�ʼ������
		
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);
		EXTI_InitStructure.EXTI_Line=EXTI_Line4;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);

		//���� NVIC_InitStruct ��ָ���Ĳ�����ʼ������ NVIC �Ĵ���
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; 
		//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //��ռ���ȼ� 2��
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; //�����ȼ� 1 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
		NVIC_Init(&NVIC_InitStructure);	  
}

uint8_t g_SI4463ItStatus[ 9 ] = { 0 };
uint8_t g_SI4463RxBuffer[ 600 ] = { 0 };  
uint8_t a_SI4463RxBuffer[ 64 ] = { 0 };
uint8_t error_cnt=0;
static uint8_t temp=0;
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
			/*�ж��Ƿ���յ������ж�*/
			if(GPIO_ReadInputDataBit(SI4463_IRQ_PORT,SI4463_IRQ_PIN)==0)
			{
								/*��ȡ��ǰ���ж�״̬*/
					SI446x_Interrupt_Status( g_SI4463ItStatus );		//��ѯ�ж�״̬	
					if( g_SI4463ItStatus[ 3 ] & ( 0x01 << 4 ))   
					{			
						SI446x_Read_Packet( a_SI4463RxBuffer );		//�����յ�������
						if( a_SI4463RxBuffer[0]==1 )
						{
							led_red_flashing( );
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
								case 0:temp=1;
											 #ifdef DEBUG_MODE
											 memcpy(g_SI4463RxBuffer,a_SI4463RxBuffer,64);
											 #else
											 memcpy(g_SI4463RxBuffer,&a_SI4463RxBuffer[2],60);
											 #endif
											 if(check_crc(a_SI4463RxBuffer, 64)==0)
											 {
												error_cnt++;
												temp=0;
												PacketrxData.flag=0;									 
												memset(g_SI4463RxBuffer,0,512);
											 } 
											 break;
								case 1:temp=2;
											 #ifdef DEBUG_MODE
											 memcpy(g_SI4463RxBuffer+64,a_SI4463RxBuffer,64);
											 #else
											 memcpy(g_SI4463RxBuffer+60,&a_SI4463RxBuffer[2],60);
											 #endif								 
											 if(check_crc(a_SI4463RxBuffer, 64)==0)
											 {
												error_cnt++;
												temp=0;
												PacketrxData.flag=0;									 
												memset(g_SI4463RxBuffer,0,512);
											 } 								 
											 break;
								case 2:temp=3;
											 #ifdef DEBUG_MODE
											 memcpy(g_SI4463RxBuffer+128,a_SI4463RxBuffer,64);
											 #else
											 memcpy(g_SI4463RxBuffer+120,&a_SI4463RxBuffer[2],60);
											 #endif								 
											 if(check_crc(a_SI4463RxBuffer, 64)==0)
											 {
												error_cnt++;
												temp=0;
												PacketrxData.flag=0;									 
												memset(g_SI4463RxBuffer,0,512); 
											 } 								 
											 break;
								case 3:temp=4;
											 #ifdef DEBUG_MODE
											 memcpy(g_SI4463RxBuffer+192,a_SI4463RxBuffer,64);
											 #else
											 memcpy(g_SI4463RxBuffer+180,&a_SI4463RxBuffer[2],60);
											 #endif									 
											 if(check_crc(a_SI4463RxBuffer, 64)==0)
											 {
												error_cnt++;
												temp=0;
												PacketrxData.flag=0;								 
												memset(g_SI4463RxBuffer,0,512); 
											 } 								 
											 break;		
								case 4:temp=5;
											 #ifdef DEBUG_MODE
											 memcpy(g_SI4463RxBuffer+256,a_SI4463RxBuffer,64);
											 #else
											 memcpy(g_SI4463RxBuffer+240,&a_SI4463RxBuffer[2],60);
											 #endif									 
											 if(check_crc(a_SI4463RxBuffer, 64)==0)
											 {
												error_cnt++;
												temp=0;
												PacketrxData.flag=0;									 
												memset(g_SI4463RxBuffer,0,512); 
											 } 								 
											 break;
								case 5:temp=6;
											 #ifdef DEBUG_MODE
											 memcpy(g_SI4463RxBuffer+320,a_SI4463RxBuffer,64);
											 #else
											 memcpy(g_SI4463RxBuffer+300,&a_SI4463RxBuffer[2],60);
											 #endif									 
											 if(check_crc(a_SI4463RxBuffer, 64)==0)
											 {
												error_cnt++;
												temp=0;
												PacketrxData.flag=0;									 
												memset(g_SI4463RxBuffer,0,512); 
											 } 								 
											 break;
								case 6:temp=7;
											 #ifdef DEBUG_MODE
											 memcpy(g_SI4463RxBuffer+384,a_SI4463RxBuffer,64);
											 #else
											 memcpy(g_SI4463RxBuffer+360,&a_SI4463RxBuffer[2],60);
											 #endif									 
											 if(check_crc(a_SI4463RxBuffer, 64)==0)
											 {
												error_cnt++;
												temp=0;
												PacketrxData.flag=0;									 
												memset(g_SI4463RxBuffer,0,512); 
											 } 								 
											 break;
								case 7:temp=8;
											 #ifdef DEBUG_MODE
											 memcpy(g_SI4463RxBuffer+448,a_SI4463RxBuffer,64);
											 #else
											 memcpy(g_SI4463RxBuffer+420,&a_SI4463RxBuffer[2],60);
											 #endif										 
											 if(check_crc(a_SI4463RxBuffer, 64)==0)
											 {
												error_cnt++;
												temp=0;
												PacketrxData.flag=0;									 
												memset(g_SI4463RxBuffer,0,512); 
											 } 								 
											 break;	
								case 8:temp=0;
											 PacketrxData.dm512_cnt=0;
											 PacketrxData.protection_flag=1;											 
											 #ifdef DEBUG_MODE								 
											 memcpy(g_SI4463RxBuffer+512,a_SI4463RxBuffer,64);
											 #else
											 memcpy(g_SI4463RxBuffer+480,&a_SI4463RxBuffer[2],32);
											 #endif								 
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
											 #ifdef DEBUG_MODE	
											 drv_uart_tx_bytes( g_SI4463RxBuffer,576 );	//�������SI4463���յ�������							
											 #endif								 
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
														SI446x_Init( );
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
						SI446x_Start_Rx(0, 0, PACKET_LENGTH,0,0,3);
					}		
			}	    
	}	
	EXTI_ClearITPendingBit(EXTI_Line4);  //���EXTI4��·����λ 
} 
