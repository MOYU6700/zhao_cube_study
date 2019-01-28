#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__


#define RADIO_MAX_PACKET_LENGTH 54
#define RADIO_ADD_PACKET_LENGTH 32
#define RADIO_MIN_PACKET_THRESHOLD 10
#define RADIO_PACKET_SEND 512
struct PacketrxData
{
  uint16_t length;
  uint8_t buf[512];
};

struct LongPacketData
{
 uint8_t NumTxPositionInPayload;
 uint8_t pTxPositionInPayload;
 uint8_t TxlengthGet;
 uint8_t sent_buff;
};

extern uint16_t date_len;
extern uint8_t channel;
extern struct PacketrxData PacketrxData;
extern struct LongPacketData LongPacketData;

#endif

