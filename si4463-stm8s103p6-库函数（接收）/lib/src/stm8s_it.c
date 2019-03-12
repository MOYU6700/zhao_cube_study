/**
  ******************************************************************************
  * @file     stm8s_it.c
  * @author   MCD Application Team
  * @version  V2.0.4
  * @date     26-April-2018
  * @brief    Main Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#include "drv_delay.h"
#include "drv_io.h"
#include "drv_SI4438.h"
#include "main.h"
#include "user_config.h" 
#include  "modbus_crc.h" 
#include "string.h"
#include "user_flash.h"
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

/** @addtogroup TIM4_TimeBase
  * @{
  */
#ifdef _COSMIC_
/**
  * @brief  Dummy interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*_COSMIC_*/

/**
  * @brief  TRAP interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief  Top Level Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TLI_IRQHandler, 0)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */ 
}

/**
  * @brief  Auto Wake Up Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  Clock Controller Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTA Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  
}

/**
  * @brief  External Interrupt PORTB Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTC Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTD Interrupt routine
  * @param  None
  * @retval None
  */
 #ifndef max485  
uint8_t g_SI4463ItStatus[ 9 ] = { 0 };
uint8_t g_SI4463RxBuffer[ 600 ] = { 0 };  
uint8_t a_SI4463RxBuffer[ 64 ] = { 0 };
uint8_t error_cnt=0;
static uint8_t temp=0;
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  /*PD3口检测IRQ 的中断入口*/
  if ((GPIO_ReadInputData(IRQ_EXTI_PORT) & IRQ_EXTI_PIN) == 0x00)
  {
      /*要发送的字节长度 大于当前FIFO的空间（64字节）*/
    SI446x_Interrupt_Status( g_SI4463ItStatus ); 
      if( g_SI4463ItStatus[ 3 ] & ( 0x01 << 4 ))   
      {			
        SI446x_Read_Packet( a_SI4463RxBuffer );		//读接收到的数据
        if( a_SI4463RxBuffer[0]==1 )
        {
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
                             drv_uart_tx_bytes( g_SI4463RxBuffer,576 );	//串口输出SI4463接收到的数据							
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
                                    user_write_flash(CHANNLE_MESSAGE_ROM,channel);
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
#else
uint8_t g_SI4463ItStatus[ 9 ] = { 0 };
uint8_t g_SI4463RxBuffer[ 600 ] = { 0 };  
uint8_t a_SI4463RxBuffer[ 64 ] = { 0 };
uint8_t error_cnt=0;
static uint8_t temp=0;
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  /*PD3口检测IRQ 的中断入口*/
  if ((GPIO_ReadInputData(IRQ_EXTI_PORT) & IRQ_EXTI_PIN) == 0x00)
  {
      /*要发送的字节长度 大于当前FIFO的空间（64字节）*/
    SI446x_Interrupt_Status( g_SI4463ItStatus ); 
      if( g_SI4463ItStatus[ 3 ] & ( 0x01 << 4 ))   
      {			
        SI446x_Read_Packet( a_SI4463RxBuffer );		//读接收到的数据
        if( a_SI4463RxBuffer[0]==1 )
        {
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
                case 0:temp=0;
                             PacketrxData.dm512_cnt=0;
                             PacketrxData.protection_flag=1;											 
                             #ifdef DEBUG_MODE								 
                             memcpy(g_SI4463RxBuffer+512,a_SI4463RxBuffer,64);
                             #else
                             memcpy(g_SI4463RxBuffer,&a_SI4463RxBuffer[2],9);
                             #endif								 
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
                             drv_uart_tx_bytes( g_SI4463RxBuffer,576 );	//串口输出SI4463接收到的数据							
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
                                    user_write_flash(CHANNLE_MESSAGE_ROM,channel);
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
#endif
/**
  * @brief  External Interrupt PORTE Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#ifdef STM8S903
/**
  * @brief  External Interrupt PORTF Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S903*/

#if defined (STM8S208) || defined (STM8AF52Ax)
/**
  * @brief CAN RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  CAN TX Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_TX_IRQHandler, 9)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S208 || STM8AF52Ax */

/**
  * @brief  SPI Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  Timer1 Update/Overflow/Trigger/Break Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  Timer1 Capture/Compare Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#ifdef STM8S903
/**
  * @brief  Timer5 Update/Overflow/Break/Trigger Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief  Timer5 Capture/Compare Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8S001 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */
/**
  * @brief  Timer2 Update/Overflow/Break Interrupt routine
  * @param  None
  * @retval None
  */
// INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
//{
//  /* In order to detect unexpected events during development,
//     it is recommended to set a breakpoint on the following instruction.
//  */
//}

/**
  * @brief  Timer2 Capture/Compare Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S903*/

#if defined(STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) || defined(STM8AF62Ax) || defined(STM8AF52Ax) || defined(STM8AF626x)
/**
  * @brief Timer3 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  Timer3 Capture/Compare Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S208, STM8S207 or STM8S105 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */

#if defined(STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) || defined(STM8S001) || defined(STM8AF62Ax) || defined(STM8AF52Ax) || defined(STM8S903)
/**
  * @brief  UART1 TX Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  UART1 RX Interrupt routine
  * @param  None
  * @retval None
  */

 INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S105 || STM8S001 */

/**
  * @brief  I2C Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
/**
  * @brief  UART2 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }

/**
  * @brief  UART2 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }
#endif /* STM8S105*/

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief  UART3 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_TX_IRQHandler, 20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }

/**
  * @brief  UART3 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }
#endif /*STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief  ADC2 interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(ADC2_IRQHandler, 22)
{

    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    return;

}
#else /*STM8S105, STM8S103 or STM8S903 or STM8AF626x */
/**
  * @brief  ADC1 interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
{

    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    return;

}
#endif /*STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax */

#ifdef STM8S903
/**
  * @brief  Timer6 Update/Overflow/Trigger Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM6_UPD_OVF_TRG_IRQHandler, 23)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8S001 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */
/**
  * @brief  Timer4 Update/Overflow Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{

  /* Cleat Interrupt Pending bit */

}
#endif /*STM8S903*/

/**
  * @brief  Eeprom EEC Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
