#include "user_flash.h"
#include "stm8s_flash.h"

/**
  * @addtogroup FLASH_ByteReadWriteOperation
  * @{
  */

/**
  * @brief How to Read / Write / Erase one Byte on FLASH memory.
  * @par   Examples description
  *        - Read one byte at address 0x40A5
  *        - Write its complement value at address + 1
  *        - Check programed value
  *        - Erase 2 byte (address 40A5 & 40A6)
  *        - Check the 2 bytes value is 0x00.
  * @param  None
  * @retval None
  */
void user_write_flash(uint32_t add,uint8_t data)
{
    /* Define FLASH programming time */
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

    /* Unlock Data memory */
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    
    FLASH_EraseByte(add);
     
    FLASH_ProgramByte(add , data);
 
    FLASH_Lock(FLASH_MEMTYPE_DATA);
}

uint8_t  user_read_flash(uint32_t add)
{
  uint8_t val=0;
    /* Define FLASH programming time */
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

    /* Unlock Data memory */
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    
    val = FLASH_ReadByte(add );
    
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    return val;
}

uint8_t flash_channel(void)
{
  uint8_t pchannel=0;
  pchannel=user_read_flash(CHANNLE_MESSAGE_ROM);
  if(pchannel >=148)   //470MHZ
  {
    pchannel=148;
  }  
  return pchannel; 
}
