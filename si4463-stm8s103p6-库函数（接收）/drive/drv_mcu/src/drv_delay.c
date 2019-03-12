/**
  ******************************************************************************
  * @author  ��ҫ�Ƽ� ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   DELAY����C�ļ�
  ******************************************************************************
  * @attention
  *
  * ����	:	http://www.ashining.com
  * �Ա�	:	https://shop105912646.taobao.com
  * ����Ͱ�:	https://cdzeyao.1688.com
  ******************************************************************************
  */



#include "drv_delay.h"
#include "stm8s_tim1.h"
#include "stm8s_tim2.h"


/**
  * @brief :1MS��ʱ����
  * @param :
  * @note  :12MHz ��1MS��ʱ
  * @retval:��
  */
static void drv_delay_1ms( void )
{
	uint16_t Ms = 1;
	uint32_t j = 30;
	
	while( Ms-- )
	{
		while( j-- );
	}
}

void delay_us_base(uint16_t ncount)
{
  for(;ncount!=0;ncount--);
}


/**
  * @brief :MS��ʱ����
  * @param :
  *			@Ms:��ʱ��MS��
  * @note  :��
  * @retval:��
  */
void drv_delay_ms( uint16_t Ms )
{
	while( Ms-- )
	{
		drv_delay_1ms( );
	}
}

/********************************************************************************
****��ʱ����ʼ��
********************************************************************************
*/
void TIM2_Config(void)//��ʱ��2��ʼ��
{
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_16, 50000);//��ʱ������1M�ļ���Ƶ�ʣ�1US�ķֱ��� ,��ʱ60ms
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  TIM2_ClearITPendingBit(TIM2_IT_UPDATE); 
  TIM2_ARRPreloadConfig(ENABLE );
  TIM2_Cmd(ENABLE);
}

static uint8_t timer3_update_flag=0;
uint8_t  get_timer3_flag(void)
{	
	uint8_t flag=0;
	flag=timer3_update_flag;
	return flag;
}	

void clr_timer3_flag(void)
{
	timer3_update_flag=0;
}
 INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
 {
    timer3_update_flag=1;
    TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
 }


void TIM1_Config(void)//��ʱ��1��ʼ��
{
  TIM1_DeInit();
  TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP,60000,0);//��ʱ������1M�ļ���Ƶ�ʣ�1US�ķֱ��� ,��ʱ60ms
//  TIM1_ARRPreloadConfig(ENABLE );
  TIM1_Cmd(ENABLE);
}

void delay_us(uint16_t us)
{
   uint16_t differ=0xffff-us-5;	
   //�趨��ʱ����������ʼֵ
   TIM1_SetCounter(differ);
				
  while(differ<0xffff-6)							//�������ж�
  {
    differ=TIM1_GetCounter();;			//��ѯ�������ļ���ֵ

  }
}

