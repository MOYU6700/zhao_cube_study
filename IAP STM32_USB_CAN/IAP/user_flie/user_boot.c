#include "user_boot.h"
#include "user_config.h"
#include "flash_if.h"
#include "user_flash.h"


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

