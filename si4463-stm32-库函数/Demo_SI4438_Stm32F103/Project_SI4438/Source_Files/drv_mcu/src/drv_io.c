#include "drv_io.h"
#include "user_config.h" 
#include "string.h"
void EXTIX_Init(void)
{
		EXTI_InitTypeDef EXTI_InitStructure; 
		NVIC_InitTypeDef NVIC_InitStructure; 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
		//�ⲿ�жϣ���Ҫʹ�� AFIO ʱ��
		//GPIOC.4 �ж����Լ��жϳ�ʼ������
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
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; //�����ȼ� 1 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
		NVIC_Init(&NVIC_InitStructure);	  
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
			/*�ж��Ƿ���յ������ж�*/
			if(GPIO_ReadInputDataBit(SI4463_IRQ_PORT,SI4463_IRQ_PIN)==0)
			{
					/*��ȡ��ǰ���ж�״̬*/
//					SI446x_Interrupt_Status( t_SI4463ItStatus );
					/*CRC Error interrupt occurred*/
//					if(t_SI4463ItStatus[3]&(0x01<<3))
//					{
//						SI446x_Change_Status( 6 );
//						while( 6 != SI446x_Get_Device_Status( ));
//						SI446x_Start_Rx(  0, 0, PACKET_LENGTH,0,0,3 );						
//					}				
					/*packet received interrupt occurred*/
//					if(t_SI4463ItStatus[3]&(0x01<<4))
//					{			
//					}	
			}	    
	}	
	EXTI_ClearITPendingBit(EXTI_Line4);  //���EXTI4��·����λ 
} 
