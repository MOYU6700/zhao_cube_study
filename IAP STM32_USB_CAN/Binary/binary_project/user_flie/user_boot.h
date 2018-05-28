#ifndef __USER_BOOT_H
#define  __USER_BOOT_H
#include "stm32f1xx_hal.h"

extern void user_boot(void);
extern void boot_clean_update_flag(void);
extern void boot_set_update_flag(void);
extern void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize);
#endif
