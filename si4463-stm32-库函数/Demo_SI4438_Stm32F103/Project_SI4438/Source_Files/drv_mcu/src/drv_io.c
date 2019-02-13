#include "drv_io.h"
#include "user_config.h" 
#include "string.h"
void EXTIX_Init(void)
{
		EXTI_InitTypeDef EXTI_InitStructure; 
		NVIC_InitTypeDef NVIC_InitStructure; 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
		//外部中断，需要使能 AFIO 时钟
		//GPIOC.4 中断线以及中断初始化配置
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);
		EXTI_InitStructure.EXTI_Line=EXTI_Line4;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);


		//根据 NVIC_InitStruct 中指定的参数初始化外设 NVIC 寄存器
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; 
		//使能按键所在的外部中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级 2，
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; //子优先级 1 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
		NVIC_Init(&NVIC_InitStructure);	  
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
			/*判断是否接收到数据中断*/
			if(GPIO_ReadInputDataBit(SI4463_IRQ_PORT,SI4463_IRQ_PIN)==0)
			{
					/*读取当前的中断状态*/
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
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除EXTI4线路挂起位 
} 
