#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__


#define RADIO_MAX_PACKET_LENGTH 54
#define RADIO_ADD_PACKET_LENGTH 32
#define RADIO_MIN_PACKET_THRESHOLD 10
#define RADIO_PACKET_SEND 512
#define TIME_OVERTIME 499    //0.1mSPER/uint
//#define DEBUG_MODE
#define max485

//#define SAFE(x) do{ \
//	__set_PRIMASK(1); \
//	x; \
//	__set_PRIMASK(0); \
//}while(0)	//ԭ�Ӳ���

struct PacketrxData
{
  uint16_t length;
  uint8_t flag;
  uint8_t dm512_cnt;
  uint8_t protection_flag;
};

//struct LongPacketData
//{
// uint8_t NumTxPositionInPayload;
// uint8_t pTxPositionInPayload;
// uint8_t TxlengthGet;
// uint8_t sent_buff;
//};

extern uint16_t date_len;
extern uint8_t channel;
extern struct PacketrxData PacketrxData;
//extern struct LongPacketData LongPacketData;

#endif