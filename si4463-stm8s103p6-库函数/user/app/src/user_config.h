#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__


#define RADIO_MAX_PACKET_LENGTH 64
#define RADIO_ADD_PACKET_LENGTH 48
#define RADIO_MIN_PACKET_THRESHOLD 10

struct PacketTxData
{
  uint16_t length;
  uint8_t buf[512];
};

struct LongPacketData
{
 uint8_t NumTxPositionInPayload;
 uint8_t * pTxPositionInPayload;
 uint8_t TxlengthGet;
};

extern uint16_t date_len;
extern uint8_t channel;
extern struct PacketTxData PacketTxData;
extern struct LongPacketData LongPacketData;

#endif