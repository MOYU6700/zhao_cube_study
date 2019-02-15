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
#include "misc.h"
#include "drv_led.h"

/**
  * @brief :延时初始化
  * @param :无
  * @note  :定时器默认初始化成us级计数
  * @retval:无
  */
void drv_delay_init( void )
{
	TIM_TimeBaseInitTypeDef	 TimerInitStructer;
	RCC_ClocksTypeDef RCC_ClocksStatus;
	
	//获取系统时钟
	RCC_GetClocksFreq(&RCC_ClocksStatus);		//用于计算定时器分频系数
	
	//使能用作延时的TIMER时钟
	RCC_APB1PeriphClockCmd( DELAY_TIME_BASE_CLK, ENABLE );
	
	TimerInitStructer.TIM_ClockDivision = TIM_CKD_DIV1;
	TimerInitStructer.TIM_CounterMode = TIM_CounterMode_Up;
	TimerInitStructer.TIM_Period = 0x00FF;
	TimerInitStructer.TIM_RepetitionCounter = DISABLE;
	TimerInitStructer.TIM_Prescaler = ( RCC_ClocksStatus.PCLK1_Frequency / 1000000 ) - 1;	//初始化默认配置为1us精度
	TIM_TimeBaseInit( DELAY_TIME_BASE, &TimerInitStructer );
	
	TIM_ClearFlag( DELAY_TIME_BASE, TIM_FLAG_Update ); 
	TIM_SetCounter( DELAY_TIME_BASE, 0 );
	TIM_Cmd( DELAY_TIME_BASE, ENABLE );
	while( RESET == TIM_GetFlagStatus( DELAY_TIME_BASE, TIM_FLAG_Update ));
	TIM_Cmd( DELAY_TIME_BASE, DISABLE );
	TIM_SetCounter( DELAY_TIME_BASE, 0 );
	TIM_ClearFlag( DELAY_TIME_BASE, TIM_FLAG_Update ); 
}

/**
  * @brief :延时(us)
  * @param :
*			@Us:延时的us数
  * @note  :不超过65535
  * @retval:无
  */
void drv_delay_us( uint16_t Us )
{
	DELAY_TIME_BASE->ARR = Us;		//写如周期值 us
	DELAY_TIME_BASE->CNT = 0;		//计数器清零
	DELAY_TIME_BASE->CR1 |= (uint32_t)0x01;	//使能计数
	while( RESET == ( DELAY_TIME_BASE->SR & TIM_FLAG_Update ));		//等待溢出，溢出说明时间到
	DELAY_TIME_BASE->SR &= (uint32_t)( ~(uint32_t)TIM_FLAG_Update );//清溢出标志
	DELAY_TIME_BASE->CR1 &= (uint32_t)( ~(uint32_t)0x01 );			//停止计数
}

/**
  * @brief :延时(ms)
  * @param :
  *			@Ms:延时的Ms数
  * @note  :不超过65
  * @retval:无
  */
void drv_delay_ms( uint8_t Ms )
{
	DELAY_TIME_BASE->ARR = Ms * 1000;	//写如周期值 us * 1000 = Ms
	DELAY_TIME_BASE->CNT = 0;			//计数器清零
	DELAY_TIME_BASE->CR1 |= (uint32_t)0x01;	//使能计数
	while( RESET == ( DELAY_TIME_BASE->SR & TIM_FLAG_Update ));	//等待溢出，溢出说明时间到
	DELAY_TIME_BASE->SR &= (uint32_t)( ~(uint32_t)TIM_FLAG_Update );	//清溢出标志
	DELAY_TIME_BASE->CR1 &= (uint32_t)( ~(uint32_t)0x01 );	//停止计数
}

/**
  * @brief :延时(500Ms)
  * @param :
*			@Ms:延时的500Ms倍数
  * @note  :不超过255
  * @retval:无
  */
void drv_delay_500Ms( uint8_t Ms_500 )
{
	while( Ms_500 -- )
	{
		drv_delay_ms( 50 );		//1 * 50ms
		drv_delay_ms( 50 );		//2 * 50ms
		drv_delay_ms( 50 );		//3 * 50ms
		drv_delay_ms( 50 );		//4 * 50ms
		drv_delay_ms( 50 );		//5 * 50ms
		drv_delay_ms( 50 );		//6 * 50ms
		drv_delay_ms( 50 );		//7 * 50ms
		drv_delay_ms( 50 );		//8 * 50ms
		drv_delay_ms( 50 );		//9 * 50ms
		drv_delay_ms( 50 );		//10 * 50ms = 500ms
		drv_delay_ms( 50 );
	}
}

/**
  * @brief :自由延时
  * @param :无
  * @note  :无
  * @retval:无
  */
void drv_delay_free( uint32_t Delay_Time )
{
	while( Delay_Time-- )
	{
			//
	}
}

/**
  * @brief :延时初始化
  * @param :无
  * @note  :定时器默认初始化成us级计数
  * @retval:无
  */
void handle_cnt_init( void )
{
	TIM_TimeBaseInitTypeDef	 TimerInitStructer;
	
	NVIC_InitTypeDef NVIC_InitStructure;  
	//使能用作延时的TIMER时钟
	RCC_APB1PeriphClockCmd( HANDLE_TIME_BASE_CLK, ENABLE );
	
	TimerInitStructer.TIM_Period = 50000;
	TimerInitStructer.TIM_Prescaler = 7199;	//初始化默认配置为10ms精度 
	TimerInitStructer.TIM_ClockDivision = TIM_CKD_DIV1;
	TimerInitStructer.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInit( HANDLE_TIME_BASE, &TimerInitStructer );
		
	//中断优先级 NVIC 设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_ClearITPendingBit(HANDLE_TIME_BASE, TIM_IT_Update ); //清除 TIM3 更新中断标志
	TIM_SetCounter(TIM3, 50000);
	TIM_ITConfig(HANDLE_TIME_BASE,TIM_IT_Update,ENABLE );	
	TIM_Cmd( HANDLE_TIME_BASE, ENABLE );
}

//定时器 3 中断服务程序
static uint8_t timer3_update_flag=0;
void TIM3_IRQHandler(void) //TIM3 中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查 TIM3 更新中断发生与否
	{
		timer3_update_flag=1;
		led_green_flashing( );
		TIM_SetCounter(TIM3, 50000);
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
	}
}

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
