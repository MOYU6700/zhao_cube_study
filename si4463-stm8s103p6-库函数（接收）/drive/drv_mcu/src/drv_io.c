#include "drv_io.h"
#include "user_config.h" 
#include  "modbus_crc.h" 
#include "user_flash.h"

void LED_Init(void)
{
   GPIO_Init(LED1_PORT,LED1_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );//定义LED的管脚的模式
}

void LED1_Toggle(void)
{
GPIO_WriteReverse(LED1_PORT,LED1_PIN);
}

void GPIO_Config(void)
{
  GPIO_Init(IRQ_EXTI_PORT, IRQ_EXTI_PIN, GPIO_MODE_IN_PU_IT);
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY); 
  EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);
  enableInterrupts();
}
