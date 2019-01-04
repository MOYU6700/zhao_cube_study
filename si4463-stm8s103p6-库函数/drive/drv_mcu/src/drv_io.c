#include "drv_io.h"

void GPIO_Config(void)
{
  GPIO_Init(IRQ_EXTI_PORT, IRQ_EXTI_PIN, GPIO_MODE_IN_PU_IT);
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY); 
  EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);
  enableInterrupts();
}
