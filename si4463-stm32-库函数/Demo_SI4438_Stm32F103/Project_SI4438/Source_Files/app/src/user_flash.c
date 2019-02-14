#include "string.h"
#include "user_flash.h"


uint8_t channle_read_data(u32 faddr)
{
	return *(uint8_t*)faddr;
}

void user_flash_write(uint32_t addr,uint8_t chan)
{
	//1、解锁FLASH
	FLASH_Unlock();
	FLASH_ErasePage(addr);
  FLASH_ProgramHalfWord(addr, chan);
	FLASH_Lock();
}


