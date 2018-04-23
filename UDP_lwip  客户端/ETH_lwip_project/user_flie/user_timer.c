#include "user_timer.h"

static uint8_t timer2_5s_flag;
void user_timer_init(void)
{
	HAL_TIM_Base_Start_IT(&htim2);			
}	

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)
	{
		timer2_5s_flag=1;
	}	
}	

uint8_t get_timer2_flag(void)
{
	uint8_t flag;
	return flag=timer2_5s_flag;
}

void clr_timer2_flag(void)
{
	timer2_5s_flag=0;
}
