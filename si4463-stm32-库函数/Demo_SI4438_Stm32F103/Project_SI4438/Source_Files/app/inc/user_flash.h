#ifndef __USER_FLASH_H
#define __USER_FLASH_H

#include "stm32f10x.h"
#include "stm32f10x_flash.h" 

#define CHANNLE_MESSAGE_ROM 0x08060000

extern uint8_t channle_read_data(u32 faddr);
extern void user_flash_write(uint32_t addr,uint8_t chan);

#endif /* __USER_FLASH_H */
