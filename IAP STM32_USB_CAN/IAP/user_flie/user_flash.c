#include "user_flash.h"
#include "user_config.h"
#include "string.h"

FLASH_EraseInitTypeDef erase_init;

typedef struct flash_write_buf
{
	uint64_t buf[FLASH_PAGE_SIZE/8];
} flash_write_t;

static uint8_t flash_page_buf[FLASH_PAGE_SIZE] = {0};
static flash_write_t *flash_write = (flash_write_t *)flash_page_buf;

void user_flash_read(uint32_t addr, uint16_t len, uint8_t *data);

void user_flash_write(uint32_t addr, uint16_t len, uint8_t *data)
{
	uint32_t flash_addr;
	flash_addr = addr/FLASH_PAGE_SIZE*FLASH_PAGE_SIZE;
	int i;	

	//����ȫ��flash data
	user_flash_read(flash_addr, FLASH_PAGE_SIZE, flash_page_buf);
	//������Ҫ���µ�flash data
	memcpy((flash_page_buf+(addr-flash_addr)), data, len);
	//����FLASH
	HAL_FLASH_Unlock();
	/*�趨Ҫˢ����ַ�Ĳ���*/
	erase_init.TypeErase=FLASH_TYPEERASE_PAGES;  //ҳˢ����
	erase_init.PageAddress=flash_addr;					//����FLASH�ĵ�ַ��
	erase_init.NbPages=1;
	/*ˢ��EEPROM������*/
	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&erase_init, &PageError);
	/*д��EEPROM*/
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flash_addr,data);
	/*��סFLASH*/
	HAL_FLASH_Lock();	
}	


void user_flash_read(uint32_t addr, uint16_t len, uint8_t *data)
{
	memcpy(data, (uint8_t *)addr, len);
}
