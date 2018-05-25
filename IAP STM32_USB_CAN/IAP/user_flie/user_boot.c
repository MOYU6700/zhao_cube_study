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
	if(boot_status->update_flag==false) //判断是否激活更新的状态位/没有激活就直接跳到APP去；
	{
		user_boot_app();
	}
	else                                   //激活就直接跳到IAP去；
	{
		boot_clean_update_flag();
	}	
}	

/*用户进入APP的入口函数调用*/
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

/*清除BIN更新的状态位*/
void boot_clean_update_flag(void)
{
	bool update_flag;
	update_flag = false;
	user_flash_write((uint32_t)&(boot_status->update_flag),
	                 sizeof(update_flag),
				     (uint8_t *)&(update_flag));
}


uint32_t iapbuf[512];   
//appxaddr:应用程序的起始地址
//appbuf:应用程序CODE.
//appsize:应用程序大小(字节).
void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize)
{
	uint16_t t;
	uint16_t i=0;
	uint32_t temp;
	uint32_t fwaddr=appxaddr;//当前写入的地址
	uint8_t *dfu=appbuf;
	for(t=0;t<appsize;t+=4)
	{		
		temp=(uint32_t) dfu[3]<<24;
		temp|=(uint32_t) dfu[2]<<16;
		temp|=(uint32_t) dfu[1]<<8;
		temp|=(uint32_t)dfu[0];	  
		dfu+=4;//偏移4个字节
		iapbuf[i++]=temp;	    
		if(i==512)
		{
			i=0;
			FLASH_If_Write(fwaddr,iapbuf,512);	
			fwaddr+=2048;//偏移2048  16=2*8.所以要乘以2.
		}
	}
	if(i)FLASH_If_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.  
}
