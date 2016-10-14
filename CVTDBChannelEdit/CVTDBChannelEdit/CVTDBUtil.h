#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include "sqlite3.h"
#include "stdafx.h"

using namespace std;

#define DEF_INVALID_VALUE           0xFFFFFFFF

typedef enum {
	BOARD_T_MSD30X_B55TA_TYPE ,
	BOARD_TYPE_NONE,
}BOARD_TYPE;
typedef enum {
	TV_TYPE_START,
	TV_ATV_TYPE = TV_TYPE_START,
	TV_DTV_TYPE,
	TV_RADIO_TYPE,
	TV_DATA_TYPE,
	TV_TYPE_END,
}TV_TYPE;
/*
#define TV_ATV_TYPE					0x00
#define TV_DTV_TYPE					0x01
#define TV_RADIO_TYPE				0x02
#define TV_DATA_TYPE				0x03
*/


#define ATV_CHANNEL_NO_BYTE_OFFSET        0
#define ATV_ALL_DATA_START_BYTE_SIZE 13
#define ATV_CHANNEL_COUNT_BYTE_SIZE  4

#define DTV_ALL_DATA_START_BYTE_SIZE 13
#define DTV_CHANNEL_COUNT_BYTE_SIZE  4

#define ATV_CHANNEL_POS_LOW_BYTE_OFFSET 2
#define ATV_CHANNEL_POS_HIGH_BYTE_OFFSET 3


#define DTV_CHANNEL_POS_LOW_BYTE_OFFSET 208
#define DTV_CHANNEL_POS_HIGH_BYTE_OFFSET 209
#define DTV_CHANNEL_NAME_BYTE_OFFSET        170
#define DTV_CHANNEL_NAME_BYTE_SIZE          34
#define DTV_CHANNEL_SKIP_BYTE_OFFSET 210
#define DTV_CHANNEL_LOCK_BYTE_OFFSET 210
#define DTV_CHANNEL_IS_LOCK     0x04
#define DTV_CHANNEL_IS_SKIP     0x01

#define ATV_CHANNEL_SKIP_BYTE_OFFSET 24
#define ATV_CHANNEL_LOCK_BYTE_OFFSET 24
#define ATV_CHANNEL_IS_LOCK     0x08
#define ATV_CHANNEL_IS_SKIP     0x02

#define DTV_CHANNEL_SERVICE_TYPE_BYTE_OFFSET 34
#define ATV_CHANNEL_NAME_BYTE_OFFSET        6
#define ATV_CHANNEL_NAME_BYTE_SIZE          8

#define DATABASE_HEAD_BYTE_SIZE                  12

#define DATABASE_CHECKSUM_BYTE_SIZE              4
#define ATV_CHECKSUM_BYTE_SIZE              2
#define DTV_CHECKSUM_BYTE_SIZE              2

#define FM_CHANNEL_COUNT_HIGHT_BYTE_OFFSET  12
#define FM_CHANNEL_COUNT_LOW_BYTE_OFFSET    13
#define FM_CHANNEL_INFO_BYTE_SIZE                4
#define FM_CHANNEL_COUNT_BYTE_SIZE          2

#define TV_CHANNEL_COUNT_HIGHT_BYTE_OFFSET  14
#define TV_CHANNEL_COUNT_LOW_BYTE_OFFSETE   15
#define TV_CHANNEL_INFO_BYTE_SIZE                40
#define TV_CHANNEL_COUNT_BYTE_SIZE          2

#define ATV_CHANNEL_DATA_BYTE_SIZE               28
#define DTV_CHANNEL_DATA_BYTE_SIZE               212

#define ATV_INFO_START_STR                 "ATVChannel"
#define DTV_INFO_START_STR                 "DTVChannel"
#define ATV_ITEM_INFO_START_STR            "ATVCH"
#define ATV_ITEM_INFO_END_STR              "_END_"
#define DTV_ITEM_INFO_START_STR            "DTVCH"
#define DTV_ITEM_INFO_END_STR              "_END_"

#define BOARD_T_MSD30X_B55TA_STR     "T_MSD30X_B55TA"

typedef enum {
	DATA_TYPE_START,
	CHANNEL_SELECT = DATA_TYPE_START,
	CHANNEL_POS,
	CHANNEL_NAME,
	CHANNEL_SKIP,
	CHANNEL_LOCK,
	CHANNEL_TYPE,
	DATA_TYPE_END,
}TAB_ROW_TYPE;
typedef struct {
	BOARD_TYPE boardType;
	unsigned int tvCloneDataOffset;
	unsigned int tvCloneDataSize;
	unsigned int atvChannelDataOffset;
	unsigned int atvChannelDataSize;
	unsigned int dtvChannelDataOffset;
	unsigned int dtvChannelDataSize;
	unsigned char* pSourceData;
	unsigned char* pDBSaveData;
	unsigned char* pTvCloneData;
	unsigned char* pAtvChannelData;
	unsigned char* pDtvChannelData;
	unsigned int atvCount;
	unsigned int dtvCount;
	unsigned char atvCheckSum[2];
	unsigned char dtvCheckSum[2];
	unsigned char dataCheckSum[4];
	unsigned int sourceDataLen;
	BYTE tvDtvTypeCountry;
	BYTE tvRadioTypeCountry;
	BYTE tvDataTypeCountry;
}DataBlockInfo;
typedef struct stChannelInfo{
	char name[34];
	bool isSkip;
	bool isLock;
	bool isSelect;
	unsigned int indexForAtv;
	unsigned int indexForDtv;
	unsigned int atvChannelNo;
	unsigned int channelPos;
	unsigned int channelType;
	unsigned int dbChannelItemDataOffset;
	unsigned int dbChannelItemDataSize;
	stChannelInfo():isSkip(false), isLock(false),isSelect(false), indexForAtv(DEF_INVALID_VALUE), indexForDtv(DEF_INVALID_VALUE),
		atvChannelNo(DEF_INVALID_VALUE), channelPos(DEF_INVALID_VALUE), channelType(DEF_INVALID_VALUE), dbChannelItemDataOffset(DEF_INVALID_VALUE), dbChannelItemDataSize(0){};
}ChannelInfo;
typedef vector<ChannelInfo> ChannelVector;
class CVTDBUtil
{
	public:
		static CVTDBUtil* GetInstance();
		static void DeleteInstance();
		BOOL OpenDb();
		BOOL SaveDataToDb();
		BOOL ParseRAWData();
		BOOL UpadtePosValueToContinue();
		BOOL ChangeChannnelPos(unsigned int from, unsigned int to);
		ChannelVector* GetChannelVectorByPos();
		void SetDBFilePath(CString path);
		void CloseDb();
		void InitData();
		char* GetDBFilePath();
		unsigned int DeleteSelectChannel();
		BOOL UpdateAtvChannelNo();
		BOARD_TYPE GetBoardType();
	private:
		static CVTDBUtil* pCVTDBUtil;
		char* pDbFilePath;
		sqlite3* pChannelDataDB;
		CWnd* pCWnd;
		DataBlockInfo dataBlockInfo;
		ChannelVector allChannelVector;
		ChannelVector deleteChannelVector;
		CVTDBUtil();
		virtual ~CVTDBUtil();
		BOOL MemFindDataFirstOffset(unsigned char * sourceData, unsigned int sourceDataLen, unsigned char * findData, unsigned int findDataLen, unsigned int & offset);
		BOOL ParseATVData();
		BOOL ParseDTVData();
		unsigned int TVCalCheckSum(BYTE *pBuf, DWORD wBufLen);
		unsigned int DataCalCheckSum(BYTE *pBuf, DWORD wBufLen);
};

