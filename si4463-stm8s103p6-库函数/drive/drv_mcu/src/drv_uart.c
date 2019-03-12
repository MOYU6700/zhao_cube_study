#include "drv_uart.h"



/**
  * @brief :���ڳ�ʼ��
  * @param :��
  * @note  :��
  * @retval:��
  */
 #ifndef max485  
void drv_uart_init( uint32_t UartBaudRate )
{
	//������������ TX������� RX��������
	GPIO_Init( UART_TX_GPIO_PORT, UART_TX_GPIO_PIN,GPIO_MODE_OUT_PP_LOW_SLOW  );
	GPIO_Init( UART_RX_GPIO_PORT, UART_RX_GPIO_PIN,GPIO_MODE_IN_PU_NO_IT );
	
	//USART��������
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);	//ʹ�ܴ���ʱ��
	UART1_DeInit();		//���ڸ�λ
	//���ڳ�ʼ�� 8λ���� 1��ֹͣλ ��У�� ���ͽ��� �����ʿɱ�
	UART1_Init( UartBaudRate, UART1_WORDLENGTH_9D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE );
        UART1_ITConfig(UART1_IT_RXNE,ENABLE );        
	UART1_Cmd(ENABLE);	//ʹ�ܴ���
        enableInterrupts(); //�����ж�
}
#else
void drv_uart_init_485( uint32_t UartBaudRate )
{
 	//������������ TX������� RX��������
	GPIO_Init( UART_TX_GPIO_PORT, UART_TX_GPIO_PIN,GPIO_MODE_OUT_PP_LOW_SLOW  );
	GPIO_Init( UART_RX_GPIO_PORT, UART_RX_GPIO_PIN,GPIO_MODE_IN_PU_NO_IT );
	
	//USART��������
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);	//ʹ�ܴ���ʱ��
	UART1_DeInit();		//���ڸ�λ
	//���ڳ�ʼ�� 8λ���� 1��ֹͣλ ��У�� ���ͽ��� �����ʿɱ�
	UART1_Init( UartBaudRate, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE );
        UART1_ITConfig(UART1_IT_RXNE,ENABLE );        
	UART1_Cmd(ENABLE);	//ʹ�ܴ���
        enableInterrupts(); //�����ж� 
}
#endif
/**
  * @brief :���ڷ�������
  * @param :
  *			@TxBuffer:���������׵�ַ
  *			@Length:���ݳ���
  * @note  :��
  * @retval:��
  */
void drv_uart_tx_bytes( uint8_t* TxBuffer, uint8_t Length )
{
	while( Length-- )
	{
		while( RESET == UART1_GetFlagStatus(UART1_FLAG_TXE));
		UART1_SendData8( * TxBuffer );
		TxBuffer++;
	}
}

/**
  * @brief :���ڽ�������
  * @param :
  *			@RxBuffer:���������׵�ַ
  * @note  :��
  * @retval:���յ����ֽڸ���
  */
uint16_t drv_uart_rx_bytes( uint8_t* RxBuffer )
{
	uint16_t l_RxLength = 0;
	uint16_t l_UartRxTimOut = 0xFFF;
	
	while( l_UartRxTimOut-- )			//�ڳ�ʱ��Χ�ڲ�ѯ����
	{
		if( RESET != UART1_GetFlagStatus(UART1_FLAG_RXNE))
		{
			*RxBuffer = UART1_ReceiveData8( );
			RxBuffer++;
			l_RxLength++;
			l_UartRxTimOut = 0xFFF;		//�ָ���ʱ�ȴ�ʱ��
		}
		if( 512 == l_RxLength )
		{
			break;						//�ֽڲ��ܳ���100���ֽڣ����ڲ���8λ���ڴ��С������buffer���ý�С
		}
	}
	
	return l_RxLength;					//���ؽ��յ����ֽڸ���
}

