#include "user_flash.h"
#include "user_config.h"
#include "string.h"

FLASH_EraseInitTypeDef erase_init;

typedef struct flash_write_buf
{
	uint64_t buf[FLASH_PAGE_SIZE/8];
} flash_write_t; 

static uint8_t flash_page_buf[FLASH_PAGE_SIZE] = {0};
/*32位的指针地址强制转换为8位的指针地址 */
static flash_write_t *flash_write = (flash_write_t *)flash_page_buf;

void user_flash_read(uint32_t addr, uint16_t len, uint8_t *data);

void user_flash_write(uint32_t addr, uint16_t len, uint8_t *data)
{
	uint32_t flash_addr;
	flash_addr = addr/FLASH_PAGE_SIZE*FLASH_PAGE_SIZE;
	int i;	

	//读出全部flash data
	user_flash_read(flash_addr, FLASH_PAGE_SIZE, flash_page_buf);
	//拷贝需要更新的flash data
	memcpy((flash_page_buf+(addr-flash_addr)), data, len);
	//解锁FLASH
	HAL_FLASH_Unlock();
	/*设定要刷除地址的参数*/
	erase_init.TypeErase=FLASH_TYPEERASE_PAGES;  //页刷除；
	erase_init.PageAddress=flash_addr;					//传入FLASH的地址；
	erase_init.NbPages=1;
	/*刷除EEPROM的内容*/
	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&erase_init, &PageError);
	/*写入EEPROM对FLASH烧写*/
	for (i=0; i<(FLASH_PAGE_SIZE/8); i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flash_addr, flash_write->buf[i]);
		flash_addr += 8;
	}
	/*锁住FLASH*/
	HAL_FLASH_Lock();	
}	


void user_flash_read(uint32_t addr, uint16_t len, uint8_t *data)
{
	memcpy(data, (uint8_t *)addr, len);
}
