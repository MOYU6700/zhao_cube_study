#ifndef __MODBUS_CRC_H
#define __MODBUS_CRC_H
#include "stm32f10x.h"

extern uint16_t crc16(uint8_t *data, uint16_t len); 
extern uint8_t check_crc(uint8_t *adu, uint16_t len);

#endif /* __MODBUS_CRC_H */
