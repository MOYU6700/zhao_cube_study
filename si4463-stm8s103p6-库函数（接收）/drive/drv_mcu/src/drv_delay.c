/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   DELAY配置C文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */



#include "drv_delay.h"
#include "stm8s_tim1.h"
#include "stm8s_tim2.h"


/**
  * @brief :1MS延时函数
  * @param :
  * @note  :12MHz 下1MS延时
  * @retval:无
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

/**
  * @brief :MS延时函数
  * @param :
  *			@Ms:延时的MS数
  * @note  :无
  * @retval:无
  */
void drv_delay_ms( uint16_t Ms )
{
	while( Ms-- )
	{
		drv_delay_1ms( );
	}
}

/********************************************************************************
****定时器初始化
********************************************************************************
*/
void TIM2_Config(void)//定时器2初始化
{
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_16, 50000);//定时器设置1M的计数频率，1US的分辨率 ,计时60ms
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


void TIM1_Config(void)//定时器1初始化
{
  TIM1_DeInit();
  TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP,60000,0);//定时器设置1M的计数频率，1US的分辨率 ,计时60ms
  TIM1_ARRPreloadConfig(ENABLE );
  TIM1_Cmd(ENABLE);
}

void delay_us(uint16_t us)
{
   uint16_t differ=0xffff-us-5;	
   //设定定时器计数器起始值
   TIM1_SetCounter(differ);
				
  while(differ<0xffff-6)							//补偿，判断
  {
    differ=TIM1_GetCounter();;			//查询计数器的计数值

  }
}

