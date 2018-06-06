#include "user_boot.h"
#include "user_config.h"
#include "flash_if.h"
#include "user_flash.h"
#include "user_uart.h"

void user_boot_app(void);
void boot_clean_update_flag(void);
#pragma pack(1)
typedef struct boot_status
{
	bool update_flag;
}boot_status_t;

boot_status_t *boot_status=(boot_status_t *)USER_FLASH_BOOT_DATA_BASE; 

#pragma pack()

void user_boot(void)
{
	if(boot_status->update_flag==false) //�ж��Ƿ񼤻���µ�״̬λ/û�м����ֱ������APPȥ��
	{
		user_boot_app();
	}
	else                                   //�����ֱ������IAPȥ��
	{
		boot_clean_update_flag();
	}	
}	

/*�û�����APP����ں�������*/
typedef  void (*pFunction)(void);
void user_boot_app(void)
{
	pFunction JumpToApplication;
	uint32_t JumpAddress;
	/* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
	if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000) 
	{
		/* Jump to user application */
		JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
		JumpToApplication = (pFunction) JumpAddress;
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
		JumpToApplication();
	}
}	

/*���BIN���µ�״̬λ*/
void boot_clean_update_flag(void)
{
	bool update_flag;
	update_flag = false;
	user_flash_write((uint32_t)&(boot_status->update_flag),
	                 sizeof(update_flag),
				     (uint8_t *)&(update_flag));
}

uint32_t iapbuf[512]={0};   
//appxaddr:Ӧ�ó������ʼ��ַ
//appbuf:Ӧ�ó���CODE.
//appsize:Ӧ�ó����С(�ֽ�).
uint32_t a[4];
void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize)
{
	uint16_t t;
	uint16_t i=0;
	uint32_t temp;
	uint32_t fwaddr=appxaddr;//��ǰд��ĵ�ַ
	uint8_t *dfu=appbuf;
	for(t=0;t<appsize;t+=4)
	{	
		temp=(uint32_t)dfu[3]<<24;
		temp|=(uint32_t)dfu[2]<<16;
		temp|=(uint32_t)dfu[1]<<8;
		temp|=(uint32_t)dfu[0];	  
		a[3]=(uint32_t)dfu[3];
		a[2]=(uint32_t)dfu[2];
		a[1]=(uint32_t)dfu[1];
		a[0]=(uint32_t)dfu[0];
		dfu+=4;//ƫ��4���ֽ�
		iapbuf[i++]=temp;	  	
		if(i==512)
		{
			i=0;
			FLASH_If_Write(fwaddr,iapbuf,512);	
			fwaddr+=2048;//ƫ��2048  16=2*8.����Ҫ����2.
		}
	}
	if(i)FLASH_If_Write(fwaddr,iapbuf,i);//������һЩ�����ֽ�д��ȥ.  
}


//��flash д���� ���룺
//http://bbs.21ic.com/icview-1260346-1-1.html
static uint32_t Update_usart = 0u;
static uint16_t oldcount=0;	//�ϵĴ��ڽ�������ֵ
static uint16_t applenth=0;	//���յ���app���볤��
void usart_update_detection(void)
{
//	uint16_t k;
		if (HAL_GetTick() - Update_usart >= 1000)
	{		
	 	if(uart_cnt)
		{
			if(oldcount==uart_cnt)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
			{
				applenth=uart_cnt;
				oldcount=0;
				uart_cnt=0;
 				if(((*(uint32_t*)(USER_FLASH_APP_BASE+4))&0xFF000000)==0X08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
				{	 			
					FLASH_If_Erase(USER_FLASH_APP_BASE);
					iap_write_appbin(USER_FLASH_APP_BASE,uart_rec_buff,applenth);//����FLASH����   
//					HAL_GPIO_WritePin(MAX485_IO_EN_GPIO_Port, MAX485_IO_EN_Pin, GPIO_PIN_RESET);
//				  HAL_Delay(10);
//				  for(k=0;k<applenth;k++)
//					 {
//							printf("%x  \n",*(uint8_t *)(0X08010000+k));
//					 }
//					 HAL_Delay(10);
//					 HAL_GPIO_WritePin(MAX485_IO_EN_GPIO_Port, MAX485_IO_EN_Pin, GPIO_PIN_SET);
					boot_clean_update_flag();	
					HAL_NVIC_SystemReset();
					while(1);					
				} 				
			}
			else 
			{
				oldcount=uart_cnt;
			}
		}	
		Update_usart=HAL_GetTick();	
	}	
}
