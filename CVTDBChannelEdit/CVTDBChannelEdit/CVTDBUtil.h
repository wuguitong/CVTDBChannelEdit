#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include "sqlite3.h"
#include "stdafx.h"
#include "CharTranslate.h"

using namespace std;

#define DEF_INVALID_VALUE           0xFFFFFFFF

typedef enum {
	BOARD_T_MSD30X_B55TA_TYPE ,
	BOARD_MSD3393_CVT_55A_TYPE,
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



//MSD309
#define MSD309_TV_ATV_TYPE					0x00
#define MSD309_TV_DTV_TYPE					0x01
#define MSD309_TV_RADIO_TYPE				0x02
#define MSD309_TV_DATA_TYPE					0x03

#define MSD309_ATV_CHANNEL_NO_BYTE_OFFSET						0
#define MSD309_ATV_ALL_DATA_START_BYTE_SIZE						13
#define MSD309_ATV_CHANNEL_COUNT_BYTE_SIZE						4

#define MSD309_DTV_ALL_DATA_START_BYTE_SIZE						13
#define MSD309_DTV_CHANNEL_COUNT_BYTE_SIZE						4

#define MSD309_ATV_CHANNEL_POS_LOW_BYTE_OFFSET					2
#define MSD309_ATV_CHANNEL_POS_HIGH_BYTE_OFFSET					3


#define MSD309_DTV_CHANNEL_POS_LOW_BYTE_OFFSET					208
#define MSD309_DTV_CHANNEL_POS_HIGH_BYTE_OFFSET					209
#define MSD309_DTV_CHANNEL_NAME_BYTE_OFFSET						170
#define MSD309_DTV_CHANNEL_NAME_BYTE_SIZE						34
#define MSD309_DTV_CHANNEL_SKIP_BYTE_OFFSET						210
#define MSD309_DTV_CHANNEL_LOCK_BYTE_OFFSET						210
#define MSD309_DTV_CHANNEL_IS_LOCK								0x04
#define MSD309_DTV_CHANNEL_IS_SKIP								0x01

#define MSD309_ATV_CHANNEL_SKIP_BYTE_OFFSET						24
#define MSD309_ATV_CHANNEL_LOCK_BYTE_OFFSET						24
#define MSD309_ATV_CHANNEL_IS_LOCK								0x08
#define MSD309_ATV_CHANNEL_IS_SKIP								0x02

#define MSD309_DTV_CHANNEL_SERVICE_TYPE_BYTE_OFFSET				34
#define MSD309_ATV_CHANNEL_NAME_BYTE_OFFSET						6
#define MSD309_ATV_CHANNEL_NAME_BYTE_SIZE						8

#define MSD309_DATABASE_HEAD_BYTE_SIZE							12

#define MSD309_DATABASE_CHECKSUM_BYTE_SIZE						4
#define MSD309_ATV_CHECKSUM_BYTE_SIZE							2
#define MSD309_DTV_CHECKSUM_BYTE_SIZE							2

#define MSD309_FM_CHANNEL_COUNT_HIGH_BYTE_OFFSET				12
#define MSD309_FM_CHANNEL_COUNT_LOW_BYTE_OFFSET					13
#define MSD309_FM_CHANNEL_INFO_BYTE_SIZE						4
#define MSD309_FM_CHANNEL_COUNT_BYTE_SIZE						2

#define MSD309_TV_CHANNEL_COUNT_HIGH_BYTE_OFFSET				14
#define MSD309_TV_CHANNEL_COUNT_LOW_BYTE_OFFSET					15
#define MSD309_TV_CHANNEL_INFO_BYTE_SIZE						40
#define MSD309_TV_CHANNEL_COUNT_BYTE_SIZE						2

#define MSD309_TV_CHANNEL_INFO_SERVICE_TYPE_BYTE_OFFSET			0
#define MSD309_TV_CHANNEL_INFO_MAJOR_CHANNO_HIGH_BYTE_OFFSET	1
#define MSD309_TV_CHANNEL_INFO_MAJOR_CHANNO_LOW_BYTE_OFFSET		2
#define MSD309_TV_CHANNEL_INFO_MINOR_CHANNO_HIGH_BYTE_OFFSET	3
#define MSD309_TV_CHANNEL_INFO_MINOR_CHANNO_LOW_BYTE_OFFSET		4
#define MSD309_TV_CHANNEL_INFO_IS_SECURE_BYTE_OFFSET			5
#define MSD309_TV_CHANNEL_INFO_IS_PROME_BYTE_OFFSET				6
#define MSD309_TV_CHANNEL_INFO_CHNAME_BYTE_OFFSET				7
#define MSD309_TV_CHANNEL_INFO_CHNAME_BYTE_SIZE					33
#define MSD309_TV_CHANNEL_INFO_IS_SECURE_DATA					0x10
#define MSD309_TV_CHANNEL_INFO_NOT_SECURE_DATA					0x00

#define MSD309_TV_CHANNEL_INFO_DEFAULT_MINOR_CHANNO				0
#define MSD309_TV_CHANNEL_INFO_DEFAULT_IS_PROME					0

#define MSD309_ATV_CHANNEL_DATA_BYTE_SIZE						28
#define MSD309_DTV_CHANNEL_DATA_BYTE_SIZE						212

#define MSD309_ATV_INFO_START_STR								"ATVChannel"
#define MSD309_DTV_INFO_START_STR								"DTVChannel"
#define MSD309_ATV_ITEM_INFO_START_STR							"ATVCH"
#define MSD309_ATV_ITEM_INFO_END_STR							"_END_"
#define MSD309_DTV_ITEM_INFO_START_STR							"DTVCH"
#define MSD309_DTV_ITEM_INFO_END_STR							"_END_"



//MSD3393
#define MSD3393_TV_ATV_TYPE					0x00
#define MSD3393_TV_DTV_TYPE					0x02
#define MSD3393_TV_RADIO_TYPE				0x03
#define MSD3393_TV_DATA_TYPE				0x04

#define MSD3393_DATABASE_HEAD_BYTE_SIZE							12
#define MSD3393_DATABASE_CHECKSUM_BYTE_SIZE						4
#define MSD3393_VIRTUAL_CHANNEL_CHECKSUM_BYTE_SIZE				2
#define MSD3393_CHANNEL_SETTIN_CHECKSUM_BYTE_SIZE				2

#define MSD3393_FM_CHANNEL_COUNT_HIGH_BYTE_OFFSET				12
#define MSD3393_FM_CHANNEL_COUNT_LOW_BYTE_OFFSET				13
#define MSD3393_FM_CHANNEL_INFO_BYTE_SIZE						4
#define MSD3393_FM_CHANNEL_COUNT_BYTE_SIZE						2

#define MSD3393_TV_CHANNEL_COUNT_HIGH_BYTE_OFFSET				14
#define MSD3393_TV_CHANNEL_COUNT_LOW_BYTE_OFFSET				15
#define MSD3393_TV_CHANNEL_INFO_BYTE_SIZE						40
#define MSD3393_TV_CHANNEL_COUNT_BYTE_SIZE						2

#define MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_HEAD_BYTE_SIZE          12
#define MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_DATA_LEN_BYTE_SIZE      4    
#define MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_CHECKSUM_BYTE_SIZE      2
#define MSD3393_ZERO_END_BYTE_SIZE								  1
#define MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_ITEM_BYTE_SIZE		  56

#define MSD3393_TVCHANNEL_CHANNEL_SETTING_DATA_LEN_BYTE_SIZE      4    
#define MSD3393_TVCHANNEL_CHANNEL_SETTING_CHECKSUM_BYTE_SIZE      2

#define MSD3393_CHANNEL_SETTING_MAIN_LIST_SRV_NUM_HIGH_BYTE_OFFSET                2701
#define MSD3393_CHANNEL_SETTING_MAIN_LIST_SRV_NUM_LOW_BYTE_OFFSET                 2700

#define MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_STRUCT_BYTE_SIZE                    2
#define MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_BYTE_OFFSET                         1292
#define MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_COUNT                               352

#define MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_STRUCT_BYTE_SIZE              4
#define MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_BYTE_OFFSET                   44
#define MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_COUNT                         136

#define MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_STRUCT_BYTE_SIZE          2
#define MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_BYTE_OFFSET               588
#define MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFOL_COUNT                    352

#define MSD3393_TV_CHANNEL_INFO_SERVICE_NAME_BYTE_SIZE						      8
#define MSD3393_VIRTUAL_CHANNEL_SERVICE_NAME_BYTE_OFFSET                          0
#define MSD3393_VIRTUAL_CHANNEL_MAJOR_NUM_HIGH_BYTE_OFFSET                        37
#define MSD3393_VIRTUAL_CHANNEL_MAJOR_NUM_LOW_BYTE_OFFSET                         36
#define MSD3393_VIRTUAL_CHANNEL_MINOR_NUM_HIGH_BYTE_OFFSET                        39
#define MSD3393_VIRTUAL_CHANNEL_MINOR_NUM_LOW_BYTE_OFFSET                         38
#define MSD3393_VIRTUAL_CHANNEL_SERVICE_TYPE_BYTE_OFFSET                          47

#define MSD3393_CHANNEL_SETTING_VIRTUAL_CHANNEL_INFO_BYTE_OFFSET                  588
#define MSD3393_CHANNEL_SETTING_VIRTUAL_CHANNEL_INFO_BYTE_SIZE                    2
#define MSD3393_VIRTUAL_CHANNEL_INFO_IS_LOCK_CH_BYTE_OFFSET						  1
#define MSD3393_VIRTUAL_CHANNEL_INFO_IS_LOCK_CH_BYTE_BIT						  0X10

#define MSD3393_TV_CHANNEL_INFO_START_STR						"TVChannel"
#define MSD3393_TV_LAST_CHANNEL_INFO_START_STR					"TVLASTChannel"


//BOARD
#define BOARD_T_MSD30X_B55TA_STR								"T_MSD30X_B55TA"
#define BOARD_MSD3393_CVT_55A_STR								"BD_CVT_55A"

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
	unsigned int sourceDataLen;
	unsigned int tvSecureChannelMajor;
	unsigned int tvSecureChannelMinor;
	unsigned char* pSourceData;
	unsigned char* pDBSaveData;
	unsigned char* pTvCloneData;
	BYTE tvSecureChannelEnable;
	BYTE tvDtvTypeCountry;
	BYTE tvRadioTypeCountry;
	BYTE tvDataTypeCountry;
	//30X
	unsigned int atv309ChannelDataOffset;
	unsigned int atv309ChannelDataSize;
	unsigned int dtv309ChannelDataOffset;
	unsigned int dtv309ChannelDataSize;

	unsigned char* p309AtvChannelData;
	unsigned char* p309DtvChannelData;
	unsigned int atv309Count;
	unsigned int dtv309Count;

	//3393 add
	unsigned char* p3393TvChannelData;
	unsigned char* p3393TvChannelVirtualChannelData;
	unsigned char* p3393TvChannelChannelSettingData;
	unsigned char* p3393TvChannelVirtualChannelDataLen;
	unsigned char* p3393TvChannelChannelSettingDataLen;
	unsigned char* p3393TvLastChannelData;
	unsigned int tv3393ChannelDataOffset;
	unsigned int tv3393ChannelDataSize;
	unsigned int tv3393ChannelVirtualChannelDataOffset;
	unsigned int tv3393ChannelVirtualChannelDataSize;
	unsigned int tv3393ChannelChannelSettingDataOffset;
	unsigned int tv3393ChannelChannelSettingDataSize;
	unsigned int tv3393LastChannelDataOffset;
	unsigned int tv3393LastChannelDataSize;

	unsigned int tv3393MainListSrvNum;
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
	unsigned int channelOldPos;
	unsigned int channelType;
	unsigned int dbChannelItemDataOffset;
	unsigned int dbChannelItemDataSize;

	//3393
	unsigned int tvPhysicalChIdx;
	unsigned int tvVirChInfoStartIdx;
	unsigned int tvVirChInfoIdx;
	unsigned int tvVirPos;
	unsigned int tvVirChIdx;
	unsigned int tvOldPhysicalChIdx;
	unsigned int tvOldVirChInfoStartIdx;
	unsigned int tvOldVirChInfoIdx;
	unsigned int tvOldVirPos;
	unsigned int tvOldVirChIdx;
	unsigned int tvMajorNum;
	unsigned int tvMinorNum;
	stChannelInfo():isSkip(false), isLock(false),isSelect(false), indexForAtv(DEF_INVALID_VALUE), indexForDtv(DEF_INVALID_VALUE),
		atvChannelNo(DEF_INVALID_VALUE), channelPos(DEF_INVALID_VALUE), channelOldPos(DEF_INVALID_VALUE), channelType(DEF_INVALID_VALUE), dbChannelItemDataOffset(DEF_INVALID_VALUE), 
		dbChannelItemDataSize(0), tvPhysicalChIdx(DEF_INVALID_VALUE), tvVirChInfoStartIdx(DEF_INVALID_VALUE), tvVirChInfoIdx(DEF_INVALID_VALUE), tvVirPos(DEF_INVALID_VALUE), tvVirChIdx(DEF_INVALID_VALUE),
		tvOldPhysicalChIdx(DEF_INVALID_VALUE), tvOldVirChInfoStartIdx(DEF_INVALID_VALUE), tvOldVirChInfoIdx(DEF_INVALID_VALUE), tvOldVirPos(DEF_INVALID_VALUE), tvOldVirChIdx(DEF_INVALID_VALUE),
		tvMajorNum(DEF_INVALID_VALUE), tvMinorNum(DEF_INVALID_VALUE){};
}ChannelInfo;


typedef struct stMSD3393VirtualChannelInfo{
	unsigned char serviceName[8];
	unsigned int majorNum;
	unsigned int minorNum;
	TV_TYPE serviceType;
}MSD3393VirtualChannelInfo;
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
		const int GetTvNameByteSize(TV_TYPE tvType);
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
		BOOL MSD309ParseATVData();
		BOOL MSD309ParseDTVData();
		BOOL MSD309SaveDataToDb();
		BOOL MSD309ParseRAWData();
		BOOL MSD3393SaveDataToDb();
		BOOL MSD3393ParseRAWData();
		BOOL MSD3393GetChannelIndoByVirChIdx(unsigned int index, MSD3393VirtualChannelInfo & info);
		unsigned int TVCalCheckSum(BYTE *pBuf, DWORD wBufLen);
		unsigned int DataCalCheckSum(BYTE *pBuf, DWORD wBufLen);
};

