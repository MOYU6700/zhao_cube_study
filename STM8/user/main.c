
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_clk.h"
#include "led.h"
#include "usart.h"

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**********************************************/
/* �������ܣ��򵥵��ӳٺ���                   */
/* ��ڲ�������                               */
/**********************************************/
void delay()
{
	int i,j;
  for(i=0;i<1000;i++)
	  {
   for(j=0;j<1000;j++);
	}
}
/**********************************************/
/* �������ܣ�������                           */
/* ��ڲ�������                               */
/**********************************************/
int main(void)
{
  /* Infinite loop */

  /*�����ڲ�����ʱ��16MΪ��ʱ��*/ 
   CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
   led_init();
   usart1_init();
   while(1)
   {
    LED_ON();
    delay();
   } 
}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

