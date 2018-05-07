#ifndef __USER_BOOT_H
#define  __USER_BOOT_H
#include "user_config.h"

extern void user_boot(void);
extern void boot_clean_update_flag(void);
extern void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize);
#endif
