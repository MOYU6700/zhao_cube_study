#ifndef __USER_FLASH_H__
#define __USER_FLASH_H__
#include "stm8s.h"

#define CHANNLE_MESSAGE_ROM 0x40A5

extern void user_write_flash(uint32_t add,uint8_t data);
extern uint8_t  user_read_flash(uint32_t add);
extern uint8_t flash_channel(void);

#endif