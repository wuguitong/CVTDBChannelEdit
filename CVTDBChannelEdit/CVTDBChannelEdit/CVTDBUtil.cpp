#include "stdafx.h"
#include "CVTDBUtil.h"


using namespace std;

bool SortByAtvChannelNo(const ChannelInfo& first, const ChannelInfo& second)
{
	return first.atvChannelNo < second.atvChannelNo;
}
bool SortByPos(const ChannelInfo& first, const ChannelInfo& second)
{
	return first.channelPos < second.channelPos;
}
bool SortByOldPos(const ChannelInfo& first, const ChannelInfo& second)
{
	return first.channelOldPos < second.channelOldPos;
}
bool SortByOldVirChIdx(const ChannelInfo& first, const ChannelInfo& second)
{
	return first.tvOldVirChIdx < second.tvOldVirChIdx;
}
bool SortByOldPhysicalChIdx(const ChannelInfo& first, const ChannelInfo& second)
{
	return first.tvOldPhysicalChIdx < second.tvOldPhysicalChIdx;
}
bool SortByAtvIndex(const ChannelInfo& first, const ChannelInfo& second)
{
	return first.indexForAtv < second.indexForAtv;
}
bool SortByDtvIndex(const ChannelInfo& first, const ChannelInfo& second)
{
	return first.indexForDtv < second.indexForDtv;
}
char* UnicodeToUtf8(const wchar_t* buf)
{
	char* result;

	int textlen = 0;

	textlen = WideCharToMultiByte(CP_UTF8, 0, buf, -1, NULL, 0, NULL, NULL);

	result = (char *)malloc((textlen + 1)*sizeof(char));

	memset(result, 0, sizeof(char)* (textlen + 1));

	WideCharToMultiByte(CP_UTF8, 0, buf, -1, result, textlen, NULL, NULL);

	return result;
}

CVTDBUtil* CVTDBUtil::pCVTDBUtil = NULL;
CVTDBUtil::CVTDBUtil()
{
	pDbFilePath = NULL;
	dataBlockInfo.pSourceData = NULL;
	dataBlockInfo.pDBSaveData = NULL;
	dataBlockInfo.pTvCloneData = NULL;
	dataBlockInfo.sourceDataLen = 0;
	dataBlockInfo.tvCloneDataOffset = 0;
	dataBlockInfo.tvCloneDataSize = 0;
	dataBlockInfo.tvDtvTypeCountry = 0;
	dataBlockInfo.tvRadioTypeCountry = 0;
	dataBlockInfo.tvDataTypeCountry = 0;
	dataBlockInfo.tvSecureChannelMajor = 0;
	dataBlockInfo.tvSecureChannelMinor = 0;
	dataBlockInfo.tvSecureChannelEnable = 0;
	pChannelDataDB = NULL;
	allChannelVector.clear();
	deleteChannelVector.clear();

	//30X
	dataBlockInfo.p309AtvChannelData = NULL;
	dataBlockInfo.p309DtvChannelData = NULL;
	dataBlockInfo.atv309ChannelDataOffset = 0;
	dataBlockInfo.dtv309ChannelDataOffset = 0;
	dataBlockInfo.atv309ChannelDataSize = 0;
	dataBlockInfo.dtv309ChannelDataSize = 0;
	dataBlockInfo.atv309Count = 0;
	dataBlockInfo.dtv309Count = 0;


	//3393
	dataBlockInfo.p3393TvChannelData = NULL;
	dataBlockInfo.p3393TvLastChannelData = NULL;
	dataBlockInfo.p3393TvChannelVirtualChannelData = NULL;
	dataBlockInfo.p3393TvChannelChannelSettingData = NULL;
	dataBlockInfo.p3393TvChannelVirtualChannelDataLen = NULL;
	dataBlockInfo.p3393TvChannelChannelSettingDataLen = NULL;
	dataBlockInfo.tv3393ChannelDataOffset = 0;
	dataBlockInfo.tv3393ChannelDataSize = 0;
	dataBlockInfo.tv3393ChannelVirtualChannelDataOffset = 0;
	dataBlockInfo.tv3393ChannelVirtualChannelDataSize = 0;
	dataBlockInfo.tv3393ChannelChannelSettingDataOffset = 0;
	dataBlockInfo.tv3393ChannelChannelSettingDataSize = 0;
	dataBlockInfo.tv3393LastChannelDataOffset = 0;
	dataBlockInfo.tv3393LastChannelDataSize = 0;

	dataBlockInfo.tv3393MainListSrvNum = 0;

	dataBlockInfo.boardType = BOARD_TYPE_NONE;
}
CVTDBUtil::~CVTDBUtil()
{
	if (pDbFilePath != NULL)
	{
		free(pDbFilePath);
		pDbFilePath = NULL;
	}
	if (dataBlockInfo.pSourceData != NULL)
	{
		free(dataBlockInfo.pSourceData);
		dataBlockInfo.pSourceData = NULL;
	}
	if (dataBlockInfo.pDBSaveData != NULL)
	{
		free(dataBlockInfo.pDBSaveData);
		dataBlockInfo.pDBSaveData = NULL;
	}
	dataBlockInfo.pTvCloneData = NULL;
	dataBlockInfo.sourceDataLen = 0;
	dataBlockInfo.tvCloneDataOffset = 0;
	dataBlockInfo.tvCloneDataSize = 0;
	dataBlockInfo.tvDtvTypeCountry = 0;
	dataBlockInfo.tvRadioTypeCountry = 0;
	dataBlockInfo.tvDataTypeCountry = 0;
	dataBlockInfo.tvSecureChannelMajor = 0;
	dataBlockInfo.tvSecureChannelMinor = 0;
	dataBlockInfo.tvSecureChannelEnable = 0;
	pChannelDataDB = NULL;
	allChannelVector.clear();
	deleteChannelVector.clear();

	//30X
	dataBlockInfo.p309AtvChannelData = NULL;
	dataBlockInfo.p309DtvChannelData = NULL;
	dataBlockInfo.atv309ChannelDataOffset = 0;
	dataBlockInfo.dtv309ChannelDataOffset = 0;
	dataBlockInfo.atv309ChannelDataSize = 0;
	dataBlockInfo.dtv309ChannelDataSize = 0;
	dataBlockInfo.atv309Count = 0;
	dataBlockInfo.dtv309Count = 0;

	//3393
	dataBlockInfo.p3393TvChannelData = NULL;
	dataBlockInfo.p3393TvLastChannelData = NULL;
	dataBlockInfo.p3393TvChannelVirtualChannelData = NULL;
	dataBlockInfo.p3393TvChannelChannelSettingData = NULL;
	dataBlockInfo.p3393TvChannelVirtualChannelDataLen = NULL;
	dataBlockInfo.p3393TvChannelChannelSettingDataLen = NULL;
	dataBlockInfo.tv3393ChannelDataOffset = 0;
	dataBlockInfo.tv3393ChannelDataSize = 0;
	dataBlockInfo.tv3393ChannelVirtualChannelDataOffset = 0;
	dataBlockInfo.tv3393ChannelVirtualChannelDataSize = 0;
	dataBlockInfo.tv3393ChannelChannelSettingDataOffset = 0;
	dataBlockInfo.tv3393ChannelChannelSettingDataSize = 0;
	dataBlockInfo.tv3393LastChannelDataOffset = 0;
	dataBlockInfo.tv3393LastChannelDataSize = 0;

	dataBlockInfo.tv3393MainListSrvNum = 0;

	dataBlockInfo.boardType = BOARD_TYPE_NONE;
}
void CVTDBUtil::InitData()
{
	if (dataBlockInfo.pSourceData != NULL)
	{
		free(dataBlockInfo.pSourceData);
		dataBlockInfo.pSourceData = NULL;
	}
	if (dataBlockInfo.pDBSaveData != NULL)
	{
		free(dataBlockInfo.pDBSaveData);
		dataBlockInfo.pDBSaveData = NULL;
	}
	dataBlockInfo.pTvCloneData = NULL;
	dataBlockInfo.sourceDataLen = 0;
	dataBlockInfo.tvCloneDataOffset = 0;
	dataBlockInfo.tvCloneDataSize = 0;
	dataBlockInfo.tvDtvTypeCountry = 0;
	dataBlockInfo.tvRadioTypeCountry = 0;
	dataBlockInfo.tvDataTypeCountry = 0;
	dataBlockInfo.tvSecureChannelMajor = 0;
	dataBlockInfo.tvSecureChannelMinor = 0;
	dataBlockInfo.tvSecureChannelEnable = 0;
	allChannelVector.clear();
	deleteChannelVector.clear();

	//30X
	dataBlockInfo.p309AtvChannelData = NULL;
	dataBlockInfo.p309DtvChannelData = NULL;
	dataBlockInfo.atv309ChannelDataOffset = 0;
	dataBlockInfo.dtv309ChannelDataOffset = 0;
	dataBlockInfo.atv309ChannelDataSize = 0;
	dataBlockInfo.dtv309ChannelDataSize = 0;
	dataBlockInfo.atv309Count = 0;
	dataBlockInfo.dtv309Count = 0;

	//3393
	dataBlockInfo.p3393TvChannelData = NULL;
	dataBlockInfo.p3393TvLastChannelData = NULL;
	dataBlockInfo.p3393TvChannelVirtualChannelData = NULL;
	dataBlockInfo.p3393TvChannelChannelSettingData = NULL;
	dataBlockInfo.p3393TvChannelVirtualChannelDataLen = NULL;
	dataBlockInfo.p3393TvChannelChannelSettingDataLen = NULL;
	dataBlockInfo.tv3393ChannelDataOffset = 0;
	dataBlockInfo.tv3393ChannelDataSize = 0;
	dataBlockInfo.tv3393ChannelVirtualChannelDataOffset = 0;
	dataBlockInfo.tv3393ChannelVirtualChannelDataSize = 0;
	dataBlockInfo.tv3393ChannelChannelSettingDataOffset = 0;
	dataBlockInfo.tv3393ChannelChannelSettingDataSize = 0;
	dataBlockInfo.tv3393LastChannelDataOffset = 0;
	dataBlockInfo.tv3393LastChannelDataSize = 0;

	dataBlockInfo.tv3393MainListSrvNum = 0;

	dataBlockInfo.boardType = BOARD_TYPE_NONE;
}
CVTDBUtil* CVTDBUtil::GetInstance()
{
	if (pCVTDBUtil == NULL)
	{
		pCVTDBUtil = new CVTDBUtil();
	}
	return pCVTDBUtil;
}
void CVTDBUtil::DeleteInstance()
{
	delete pCVTDBUtil;
	pCVTDBUtil = NULL;
}
void CVTDBUtil::SetDBFilePath(CString path)
{
	if (pDbFilePath != NULL)
	{
		free(pDbFilePath);
		pDbFilePath = NULL;
	}
	pDbFilePath = UnicodeToUtf8(path);	
}
char* CVTDBUtil::GetDBFilePath()
{
	return pDbFilePath;
}
BOOL CVTDBUtil::OpenDb()
{
	USES_CONVERSION;
	if (pDbFilePath == NULL){
		InitData();
		return FALSE;
	}
	int rc = sqlite3_open(pDbFilePath, &pChannelDataDB);
	if (rc == SQLITE_OK)
	{
		return TRUE;
	}
	InitData();
	return FALSE;
}
void CVTDBUtil::CloseDb()
{
	if (pChannelDataDB != NULL)
	{
		sqlite3_close(pChannelDataDB);
		pChannelDataDB = NULL;
	}
}
BOOL CVTDBUtil::ParseRAWData()
{
	USES_CONVERSION;
	sqlite3_stmt *stat = 0;
	InitData();
	if (sqlite3_prepare(pChannelDataDB, "select DATA from SETTINGS where FIELDNAME='TunerRawData'", -1, &stat, 0) == SQLITE_OK){
		if (sqlite3_step(stat) == SQLITE_ROW)
		{
			const void * pFileContent = sqlite3_column_blob(stat, 0);
			const int size = sqlite3_column_bytes(stat, 0);
			unsigned int tmpOffset;
			if (MemFindDataFirstOffset((unsigned char *)pFileContent, size, (unsigned char *)BOARD_T_MSD30X_B55TA_STR, strlen(BOARD_T_MSD30X_B55TA_STR), tmpOffset))
			{
				dataBlockInfo.boardType = BOARD_T_MSD30X_B55TA_TYPE;
			}
			else if (MemFindDataFirstOffset((unsigned char *)pFileContent, size, (unsigned char *)BOARD_MSD3393_CVT_55A_STR, strlen(BOARD_MSD3393_CVT_55A_STR), tmpOffset))
			{
				dataBlockInfo.boardType = BOARD_MSD3393_CVT_55A_TYPE;
			}
		}
	}
	else{
		sqlite3_finalize(stat);
		return FALSE;
	}
	sqlite3_finalize(stat);
	switch (dataBlockInfo.boardType)
	{
	case BOARD_T_MSD30X_B55TA_TYPE:
		return MSD309ParseRAWData();
		break;
	case BOARD_MSD3393_CVT_55A_TYPE:
		return MSD3393ParseRAWData();
		break;
	default:
		break;
	}
	return FALSE;
}
BOOL CVTDBUtil::MSD309ParseRAWData()
{
	USES_CONVERSION;
	sqlite3_stmt *stat = 0;
	if (sqlite3_prepare(pChannelDataDB, "select DATA from SETTINGS where FIELDNAME='SecureChannelEnabled'", -1, &stat, 0) == SQLITE_OK){
		if (sqlite3_step(stat) == SQLITE_ROW)
		{
			const void * pFileContent = sqlite3_column_blob(stat, 0);
			const int size = sqlite3_column_bytes(stat, 0);
			dataBlockInfo.tvSecureChannelEnable = ((unsigned char*)pFileContent)[0];
		}
	}
	sqlite3_finalize(stat);
	if (sqlite3_prepare(pChannelDataDB, "select DATA from SETTINGS where FIELDNAME='SecureChannelMajor'", -1, &stat, 0) == SQLITE_OK){
		if (sqlite3_step(stat) == SQLITE_ROW)
		{
			const void * pFileContent = sqlite3_column_blob(stat, 0);
			const int size = sqlite3_column_bytes(stat, 0);
			dataBlockInfo.tvSecureChannelMajor = ((unsigned char*)pFileContent)[0] * 256 + ((unsigned char*)pFileContent)[1];
		}
	}
	sqlite3_finalize(stat);
	if (sqlite3_prepare(pChannelDataDB, "select DATA from SETTINGS where FIELDNAME='SecureChannelMinor'", -1, &stat, 0) == SQLITE_OK){
		if (sqlite3_step(stat) == SQLITE_ROW)
		{
			const void * pFileContent = sqlite3_column_blob(stat, 0);
			const int size = sqlite3_column_bytes(stat, 0);
			dataBlockInfo.tvSecureChannelMinor = ((unsigned char*)pFileContent)[0] * 256 + ((unsigned char*)pFileContent)[1];
		}
	}
	sqlite3_finalize(stat);
	if (sqlite3_prepare(pChannelDataDB, "select DATA from SETTINGS where FIELDNAME='TunerRawData'", -1, &stat, 0) == SQLITE_OK){
		if (sqlite3_step(stat) == SQLITE_ROW)
		{
			const void * pFileContent = sqlite3_column_blob(stat, 0);
			const int size = sqlite3_column_bytes(stat, 0);
			dataBlockInfo.pSourceData = (unsigned char *)malloc(size);
			dataBlockInfo.sourceDataLen = size;
			memcpy((unsigned char *)dataBlockInfo.pSourceData, (unsigned char *)pFileContent, size);
		}
	}
	else{
		sqlite3_finalize(stat);
		return FALSE;
	}
	sqlite3_finalize(stat);
	dataBlockInfo.tvCloneDataOffset = (dataBlockInfo.pSourceData[MSD309_FM_CHANNEL_COUNT_HIGH_BYTE_OFFSET] * 256 + dataBlockInfo.pSourceData[MSD309_FM_CHANNEL_COUNT_LOW_BYTE_OFFSET]) * MSD309_FM_CHANNEL_INFO_BYTE_SIZE +
		(dataBlockInfo.pSourceData[MSD309_TV_CHANNEL_COUNT_HIGH_BYTE_OFFSET] * 256 + dataBlockInfo.pSourceData[MSD309_TV_CHANNEL_COUNT_LOW_BYTE_OFFSET]) * MSD309_TV_CHANNEL_INFO_BYTE_SIZE +
		MSD309_DATABASE_HEAD_BYTE_SIZE + MSD309_FM_CHANNEL_COUNT_BYTE_SIZE + MSD309_TV_CHANNEL_COUNT_BYTE_SIZE;
	dataBlockInfo.pTvCloneData = &dataBlockInfo.pSourceData[dataBlockInfo.tvCloneDataOffset];
	dataBlockInfo.tvCloneDataSize = dataBlockInfo.sourceDataLen - dataBlockInfo.tvCloneDataOffset - MSD309_DATABASE_CHECKSUM_BYTE_SIZE;
	unsigned int tmpSize = dataBlockInfo.sourceDataLen - dataBlockInfo.tvCloneDataOffset;
	unsigned int tmpOffset;
	if (MemFindDataFirstOffset(dataBlockInfo.pTvCloneData, tmpSize, (unsigned char *)MSD309_ATV_INFO_START_STR, strlen(MSD309_ATV_INFO_START_STR), tmpOffset))
	{
		dataBlockInfo.p309AtvChannelData = &dataBlockInfo.pTvCloneData[tmpOffset];
		dataBlockInfo.atv309ChannelDataOffset = tmpOffset + dataBlockInfo.tvCloneDataOffset;
	}
	else
	{
		return FALSE;
	}
	if (MemFindDataFirstOffset(dataBlockInfo.pTvCloneData, tmpSize, (unsigned char *)MSD309_DTV_INFO_START_STR, strlen(MSD309_DTV_INFO_START_STR), tmpOffset))
	{
		dataBlockInfo.p309DtvChannelData = &dataBlockInfo.pTvCloneData[tmpOffset];
		dataBlockInfo.dtv309ChannelDataOffset = tmpOffset + dataBlockInfo.tvCloneDataOffset;
	}
	else
	{
		return FALSE;
	}
	if (!MSD309ParseATVData())
	{
		return FALSE;
	}
	if (!MSD309ParseDTVData())
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CVTDBUtil::MSD309ParseATVData()
{
	if (dataBlockInfo.p309AtvChannelData == NULL)
	{
		return FALSE;
	}
	unsigned char * pTmpData;
	unsigned int startOffset, tmpSize, tmpOffset, atvStartStrLen, atvEndStrLen, channelIndex = 0;
	pTmpData = dataBlockInfo.p309AtvChannelData;
	tmpSize = dataBlockInfo.sourceDataLen - dataBlockInfo.atv309ChannelDataOffset;
	atvStartStrLen = strlen(MSD309_ATV_ITEM_INFO_START_STR);
	atvEndStrLen = strlen(MSD309_ATV_ITEM_INFO_END_STR);
	while (MemFindDataFirstOffset(pTmpData, tmpSize, (unsigned char *)MSD309_ATV_ITEM_INFO_START_STR, atvStartStrLen, startOffset))
	{
		if (!MemFindDataFirstOffset(&pTmpData[startOffset + MSD309_ATV_CHANNEL_DATA_BYTE_SIZE + 5], atvEndStrLen, (unsigned char *)MSD309_ATV_ITEM_INFO_END_STR, atvEndStrLen, tmpOffset))
		{
			pTmpData = &pTmpData[startOffset + MSD309_ATV_CHANNEL_DATA_BYTE_SIZE + atvEndStrLen + atvStartStrLen];
			tmpSize = tmpSize - MSD309_ATV_CHANNEL_DATA_BYTE_SIZE - startOffset - atvEndStrLen - atvStartStrLen;
			continue;
		}
		ChannelInfo channelInfo;
		channelInfo.dbChannelItemDataOffset = &pTmpData[startOffset] - dataBlockInfo.pSourceData;
		channelInfo.dbChannelItemDataSize = MSD309_ATV_CHANNEL_DATA_BYTE_SIZE + atvEndStrLen + atvStartStrLen;
		channelInfo.channelPos = pTmpData[startOffset + atvStartStrLen + MSD309_ATV_CHANNEL_POS_HIGH_BYTE_OFFSET] * 256 + pTmpData[startOffset + atvStartStrLen + MSD309_ATV_CHANNEL_POS_LOW_BYTE_OFFSET];
		channelInfo.channelOldPos = channelInfo.channelPos;
		channelInfo.channelType = TV_ATV_TYPE;
		channelInfo.isLock = ((pTmpData[startOffset + atvStartStrLen + MSD309_ATV_CHANNEL_LOCK_BYTE_OFFSET] & MSD309_ATV_CHANNEL_IS_LOCK) != 0);
		channelInfo.isSkip = ((pTmpData[startOffset + atvStartStrLen + MSD309_ATV_CHANNEL_SKIP_BYTE_OFFSET] & MSD309_ATV_CHANNEL_IS_SKIP) != 0);
		memcpy(channelInfo.name, &pTmpData[startOffset + atvStartStrLen + MSD309_ATV_CHANNEL_NAME_BYTE_OFFSET], MSD309_ATV_CHANNEL_NAME_BYTE_SIZE);
		channelInfo.atvChannelNo = pTmpData[startOffset + atvStartStrLen + MSD309_ATV_CHANNEL_NO_BYTE_OFFSET];
		channelInfo.indexForAtv = channelIndex;
		allChannelVector.push_back(channelInfo);
		//printf("ATV channelInfo.dbDataOffset=%d,channelInfo.channelPos=%d,channelInfo.channelType=%d,channelInfo.isLock=%d,channelInfo.isSkip=%d,channelInfo.atvChannelNo=%d,channelInfo.indexForAtv=%d\n", \
			channelInfo.dbDataOffset, channelInfo.channelPos, channelInfo.channelType, channelInfo.isLock, channelInfo.isSkip, channelInfo.atvChannelNo, channelInfo.indexForAtv);
		pTmpData = &pTmpData[startOffset + MSD309_ATV_CHANNEL_DATA_BYTE_SIZE + atvEndStrLen + atvStartStrLen];
		tmpSize = tmpSize - MSD309_ATV_CHANNEL_DATA_BYTE_SIZE - startOffset - atvEndStrLen - atvStartStrLen;
		channelIndex++;
	}
	dataBlockInfo.atv309Count = channelIndex;
	if (channelIndex > 0)
	{
		dataBlockInfo.atv309ChannelDataSize = (pTmpData - dataBlockInfo.pSourceData) - dataBlockInfo.atv309ChannelDataOffset;
	}
	else
	{
		dataBlockInfo.atv309ChannelDataSize = MSD309_ATV_ALL_DATA_START_BYTE_SIZE + MSD309_ATV_CHANNEL_COUNT_BYTE_SIZE + MSD309_ATV_CHECKSUM_BYTE_SIZE;
	}
	return TRUE;
}
BOOL CVTDBUtil::MSD309ParseDTVData()
{
	USES_CONVERSION;
	if (dataBlockInfo.p309DtvChannelData == NULL)
	{
		return FALSE;
	}
	unsigned char * pTmpData;
	unsigned int startOffset, tmpSize, tmpOffset, dtvStartStrLen, dtvEndStrLen,channelIndex = 0;
	pTmpData = dataBlockInfo.p309DtvChannelData;
	tmpSize = dataBlockInfo.sourceDataLen - dataBlockInfo.dtv309ChannelDataOffset;
	dtvStartStrLen = strlen(MSD309_DTV_ITEM_INFO_START_STR);
	dtvEndStrLen = strlen(MSD309_DTV_ITEM_INFO_END_STR);
	while (MemFindDataFirstOffset(pTmpData, tmpSize, (unsigned char *)MSD309_DTV_ITEM_INFO_START_STR, dtvStartStrLen, startOffset))
	{
		if (!MemFindDataFirstOffset(&pTmpData[startOffset + MSD309_DTV_CHANNEL_DATA_BYTE_SIZE + 5], dtvEndStrLen, (unsigned char *)MSD309_DTV_ITEM_INFO_END_STR, dtvEndStrLen, tmpOffset))
		{
			pTmpData = &pTmpData[startOffset + MSD309_DTV_CHANNEL_DATA_BYTE_SIZE + dtvStartStrLen + dtvEndStrLen];
			tmpSize = tmpSize - MSD309_DTV_CHANNEL_DATA_BYTE_SIZE - startOffset - dtvStartStrLen - dtvEndStrLen;
			continue;
		}
		ChannelInfo channelInfo;
		channelInfo.dbChannelItemDataOffset = &pTmpData[startOffset] - dataBlockInfo.pSourceData;
		channelInfo.dbChannelItemDataSize = MSD309_DTV_CHANNEL_DATA_BYTE_SIZE + dtvStartStrLen + dtvEndStrLen;
		channelInfo.channelPos = pTmpData[startOffset + dtvStartStrLen + MSD309_DTV_CHANNEL_POS_HIGH_BYTE_OFFSET] * 256 + pTmpData[startOffset + dtvStartStrLen + MSD309_DTV_CHANNEL_POS_LOW_BYTE_OFFSET];
		channelInfo.channelOldPos = channelInfo.channelPos;
		
		switch (pTmpData[startOffset + dtvStartStrLen + MSD309_DTV_CHANNEL_SERVICE_TYPE_BYTE_OFFSET])
		{
		case MSD309_TV_DTV_TYPE:
			channelInfo.channelType = TV_DTV_TYPE;
			break;
		case MSD309_TV_RADIO_TYPE:
			channelInfo.channelType = TV_RADIO_TYPE;
			break;
		case MSD309_TV_DATA_TYPE:
			channelInfo.channelType = TV_DATA_TYPE;
			break;
		default:
			break;
		}
		channelInfo.isLock = ((pTmpData[startOffset + dtvStartStrLen + MSD309_DTV_CHANNEL_LOCK_BYTE_OFFSET] & MSD309_DTV_CHANNEL_IS_LOCK) != 0);
		channelInfo.isSkip = ((pTmpData[startOffset + dtvStartStrLen + MSD309_DTV_CHANNEL_SKIP_BYTE_OFFSET] & MSD309_DTV_CHANNEL_IS_SKIP) != 0);
		memcpy(channelInfo.name, &pTmpData[startOffset + dtvStartStrLen + MSD309_DTV_CHANNEL_NAME_BYTE_OFFSET], MSD309_DTV_CHANNEL_NAME_BYTE_SIZE);
		channelInfo.indexForDtv = channelIndex;
		allChannelVector.push_back(channelInfo);
		//printf("DTV channelInfo.dbDataOffset=%d,channelInfo.channelPos=%d,channelInfo.channelType=%d,channelInfo.isLock=%d,channelInfo.isSkip=%d,channelInfo.indexForDtv=%d\n",\
					channelInfo.dbDataOffset, channelInfo.channelPos, channelInfo.channelType, channelInfo.isLock, channelInfo.isSkip, channelInfo.indexForDtv);
		pTmpData = &pTmpData[startOffset + MSD309_DTV_CHANNEL_DATA_BYTE_SIZE + dtvStartStrLen + dtvEndStrLen];
		tmpSize = tmpSize - MSD309_DTV_CHANNEL_DATA_BYTE_SIZE - startOffset - dtvStartStrLen - dtvEndStrLen;
		channelIndex++;
	}
	dataBlockInfo.dtv309Count = channelIndex;
	if (channelIndex > 0)
	{
		dataBlockInfo.dtv309ChannelDataSize = (pTmpData - dataBlockInfo.pSourceData) - dataBlockInfo.dtv309ChannelDataOffset;
	}
	else
	{
		dataBlockInfo.dtv309ChannelDataSize = MSD309_DTV_ALL_DATA_START_BYTE_SIZE + MSD309_DTV_CHANNEL_COUNT_BYTE_SIZE + MSD309_DTV_CHECKSUM_BYTE_SIZE;
	}
	//get country
	unsigned int i = 0, dtvCountryOffset = 0;
	int tvType = TV_DTV_TYPE;
	dtvCountryOffset = MSD309_DTV_ALL_DATA_START_BYTE_SIZE + MSD309_DTV_CHANNEL_COUNT_BYTE_SIZE + MSD309_DTV_CHECKSUM_BYTE_SIZE;
	for (tvType = TV_DTV_TYPE; tvType < TV_TYPE_END; tvType++)
	{
		for (i = 0; i < allChannelVector.size(); i++)
		{
			if (allChannelVector[i].channelType == tvType)
			{
				dtvCountryOffset += allChannelVector[i].dbChannelItemDataSize;
			}
		}
		switch (tvType)
		{
		case TV_DTV_TYPE:
			dataBlockInfo.tvDtvTypeCountry = dataBlockInfo.p309DtvChannelData[dtvCountryOffset];
			break;
		case TV_RADIO_TYPE:
			dataBlockInfo.tvRadioTypeCountry = dataBlockInfo.p309DtvChannelData[dtvCountryOffset];
			break;
		case TV_DATA_TYPE:
			dataBlockInfo.tvDataTypeCountry = dataBlockInfo.p309DtvChannelData[dtvCountryOffset];
			break;
		default:
			break;
		}
		dtvCountryOffset++;
	}
	return TRUE;
}
BOOL CVTDBUtil::SaveDataToDb()
{
	switch (dataBlockInfo.boardType)
	{
	case BOARD_T_MSD30X_B55TA_TYPE:
		return MSD309SaveDataToDb();
		break;
	case BOARD_MSD3393_CVT_55A_TYPE:
		return MSD3393SaveDataToDb();
		break;
	default:
		break;
	}
	return FALSE;
}
BOOL CVTDBUtil::MSD309SaveDataToDb()
{
	USES_CONVERSION;
	BOOL result = TRUE;
	unsigned int i = 0;
	unsigned char * pTmpData;
	unsigned int dataCheckSum = 0,nowSaveDataOffset = 0,deleteChannelSize = 0;
	if (pChannelDataDB == NULL)
	{
		return FALSE;
	}
	else
	{
		if (pDbFilePath == NULL){
			return FALSE;
		}
		for (i = 0; i < deleteChannelVector.size(); i++)
		{
			deleteChannelSize += deleteChannelVector[i].dbChannelItemDataSize;
			deleteChannelSize += MSD309_TV_CHANNEL_INFO_BYTE_SIZE;//每个TV占用40个字节
		}
		if (dataBlockInfo.pDBSaveData != NULL)
		{
			free(dataBlockInfo.pDBSaveData);
			dataBlockInfo.pDBSaveData = NULL;
		}
		dataBlockInfo.pDBSaveData = (unsigned char *)malloc(dataBlockInfo.sourceDataLen - deleteChannelSize);
		/////////copy before clone data
		//copy database head
		//copy total count of fm channel
		//copy total count of tv channel
		//copy fm data
		unsigned int totalFmCount, totalTvCount;
		totalFmCount = dataBlockInfo.pSourceData[MSD309_FM_CHANNEL_COUNT_HIGH_BYTE_OFFSET] * 256 + dataBlockInfo.pSourceData[MSD309_FM_CHANNEL_COUNT_LOW_BYTE_OFFSET];
		memcpy(dataBlockInfo.pDBSaveData, dataBlockInfo.pSourceData, MSD309_DATABASE_HEAD_BYTE_SIZE + MSD309_FM_CHANNEL_COUNT_BYTE_SIZE + MSD309_TV_CHANNEL_COUNT_BYTE_SIZE + totalFmCount*MSD309_FM_CHANNEL_INFO_BYTE_SIZE);
		//set tv count
		totalTvCount = allChannelVector.size();
		dataBlockInfo.pDBSaveData[MSD309_TV_CHANNEL_COUNT_HIGH_BYTE_OFFSET] = totalTvCount / 256;
		dataBlockInfo.pDBSaveData[MSD309_TV_CHANNEL_COUNT_LOW_BYTE_OFFSET] = totalTvCount % 256;
		nowSaveDataOffset += MSD309_DATABASE_HEAD_BYTE_SIZE + MSD309_FM_CHANNEL_COUNT_BYTE_SIZE + MSD309_TV_CHANNEL_COUNT_BYTE_SIZE + totalFmCount * MSD309_FM_CHANNEL_INFO_BYTE_SIZE;
		//copy tv data
		memset(&dataBlockInfo.pDBSaveData[nowSaveDataOffset], 0, totalTvCount*MSD309_TV_CHANNEL_INFO_BYTE_SIZE);
		sort(allChannelVector.begin(), allChannelVector.end(), SortByPos);
		for (i = 0; i < totalTvCount;i++)
		{
			unsigned int serverType = MSD309_TV_ATV_TYPE;
			switch (allChannelVector[i].channelType)
			{
			case TV_DTV_TYPE:
				serverType = MSD309_TV_DTV_TYPE;
				break;
			case TV_RADIO_TYPE:
				serverType = MSD309_TV_RADIO_TYPE;
				break;
			case TV_DATA_TYPE:
				serverType = MSD309_TV_DATA_TYPE;
				break;
			default:
				serverType = MSD309_TV_ATV_TYPE;
				break;
			}
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD309_TV_CHANNEL_INFO_BYTE_SIZE + MSD309_TV_CHANNEL_INFO_SERVICE_TYPE_BYTE_OFFSET] = serverType;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD309_TV_CHANNEL_INFO_BYTE_SIZE + MSD309_TV_CHANNEL_INFO_MAJOR_CHANNO_HIGH_BYTE_OFFSET] = (allChannelVector[i].channelPos + 1) / 256;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD309_TV_CHANNEL_INFO_BYTE_SIZE + MSD309_TV_CHANNEL_INFO_MAJOR_CHANNO_LOW_BYTE_OFFSET] = (allChannelVector[i].channelPos + 1) % 256;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD309_TV_CHANNEL_INFO_BYTE_SIZE + MSD309_TV_CHANNEL_INFO_MINOR_CHANNO_HIGH_BYTE_OFFSET] = MSD309_TV_CHANNEL_INFO_DEFAULT_MINOR_CHANNO / 256;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD309_TV_CHANNEL_INFO_BYTE_SIZE + MSD309_TV_CHANNEL_INFO_MINOR_CHANNO_LOW_BYTE_OFFSET] = MSD309_TV_CHANNEL_INFO_DEFAULT_MINOR_CHANNO % 256;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD309_TV_CHANNEL_INFO_BYTE_SIZE + MSD309_TV_CHANNEL_INFO_IS_SECURE_BYTE_OFFSET] = allChannelVector[i].isLock ? MSD309_TV_CHANNEL_INFO_IS_SECURE_DATA : MSD309_TV_CHANNEL_INFO_NOT_SECURE_DATA;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD309_TV_CHANNEL_INFO_BYTE_SIZE + MSD309_TV_CHANNEL_INFO_IS_PROME_BYTE_OFFSET] = MSD309_TV_CHANNEL_INFO_DEFAULT_IS_PROME;
			if (allChannelVector[i].channelType == TV_ATV_TYPE)
			{
				memcpy(&dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD309_TV_CHANNEL_INFO_BYTE_SIZE + MSD309_TV_CHANNEL_INFO_CHNAME_BYTE_OFFSET], allChannelVector[i].name, MSD309_ATV_CHANNEL_NAME_BYTE_SIZE);
			}
			else
			{
				WORD unicodeName[MSD309_DTV_CHANNEL_NAME_BYTE_SIZE];
				MApp_TranslateCharTableToUnicode((BYTE *)allChannelVector[i].name, unicodeName, MSD309_DTV_CHANNEL_NAME_BYTE_SIZE);
				CString unicodeStr;
				unicodeStr.Format(L"%s", unicodeName);
				char * utf8name = UnicodeToUtf8(unicodeStr);
				memcpy(&dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD309_TV_CHANNEL_INFO_BYTE_SIZE + MSD309_TV_CHANNEL_INFO_CHNAME_BYTE_OFFSET], utf8name, strlen(utf8name) + 1);
			}
		}
		nowSaveDataOffset += totalTvCount*MSD309_TV_CHANNEL_INFO_BYTE_SIZE;
		//copy before atv data
		memcpy(&(dataBlockInfo.pDBSaveData[nowSaveDataOffset]), &(dataBlockInfo.pSourceData[dataBlockInfo.tvCloneDataOffset]), dataBlockInfo.atv309ChannelDataOffset - dataBlockInfo.tvCloneDataOffset);
		nowSaveDataOffset += dataBlockInfo.atv309ChannelDataOffset - dataBlockInfo.tvCloneDataOffset;
		//copy atv
		sort(allChannelVector.begin(), allChannelVector.end(), SortByAtvIndex);
		unsigned int atvStartStrLen, atvEndStrLen, atvDataStartOffset = 0, atvByteCountOffset = 0, atvByteCheckSumOffset = 0, atvByteCount = 0, atvCheckSum = 0;
		atvStartStrLen = strlen(MSD309_ATV_ITEM_INFO_START_STR);
		atvEndStrLen = strlen(MSD309_ATV_ITEM_INFO_END_STR);
		memcpy(&(dataBlockInfo.pDBSaveData[nowSaveDataOffset]), &(dataBlockInfo.pSourceData[dataBlockInfo.atv309ChannelDataOffset]), MSD309_ATV_ALL_DATA_START_BYTE_SIZE);
		nowSaveDataOffset += MSD309_ATV_ALL_DATA_START_BYTE_SIZE + MSD309_ATV_CHANNEL_COUNT_BYTE_SIZE + MSD309_ATV_CHECKSUM_BYTE_SIZE;
		atvByteCountOffset = nowSaveDataOffset - MSD309_ATV_CHECKSUM_BYTE_SIZE - MSD309_ATV_CHANNEL_COUNT_BYTE_SIZE;
		atvByteCheckSumOffset = nowSaveDataOffset - MSD309_ATV_CHECKSUM_BYTE_SIZE;
		atvDataStartOffset = nowSaveDataOffset;
		for (i = 0; i < allChannelVector.size(); i++)
		{
			if (allChannelVector[i].channelType != TV_ATV_TYPE)
			{
				break;
			}
			pTmpData = (unsigned char *)malloc(allChannelVector[i].dbChannelItemDataSize);
			memcpy(pTmpData, &(dataBlockInfo.pSourceData[allChannelVector[i].dbChannelItemDataOffset]), allChannelVector[i].dbChannelItemDataSize);
			memcpy(&pTmpData[atvStartStrLen + MSD309_ATV_CHANNEL_NAME_BYTE_OFFSET], allChannelVector[i].name, MSD309_ATV_CHANNEL_NAME_BYTE_SIZE);
			if (allChannelVector[i].isLock)
			{
				pTmpData[atvStartStrLen + MSD309_ATV_CHANNEL_LOCK_BYTE_OFFSET] |= MSD309_ATV_CHANNEL_IS_LOCK;
			}
			else
			{
				pTmpData[atvStartStrLen + MSD309_ATV_CHANNEL_LOCK_BYTE_OFFSET] &= ~MSD309_ATV_CHANNEL_IS_LOCK;
			}
			if (allChannelVector[i].isSkip)
			{
				pTmpData[atvStartStrLen + MSD309_ATV_CHANNEL_SKIP_BYTE_OFFSET] |= MSD309_ATV_CHANNEL_IS_SKIP;
			}
			else
			{
				pTmpData[atvStartStrLen + MSD309_ATV_CHANNEL_SKIP_BYTE_OFFSET] &= ~MSD309_ATV_CHANNEL_IS_SKIP;
			}
			pTmpData[atvStartStrLen + MSD309_ATV_CHANNEL_POS_HIGH_BYTE_OFFSET] = allChannelVector[i].channelPos / 256;
			pTmpData[atvStartStrLen + MSD309_ATV_CHANNEL_POS_LOW_BYTE_OFFSET] = allChannelVector[i].channelPos % 256;

			pTmpData[atvStartStrLen + MSD309_ATV_CHANNEL_NO_BYTE_OFFSET] = allChannelVector[i].atvChannelNo;

			memcpy(&(dataBlockInfo.pDBSaveData[nowSaveDataOffset]), pTmpData, allChannelVector[i].dbChannelItemDataSize);
			nowSaveDataOffset += allChannelVector[i].dbChannelItemDataSize;
			atvByteCount += allChannelVector[i].dbChannelItemDataSize;
			free(pTmpData);

		}
		atvCheckSum = TVCalCheckSum(&(dataBlockInfo.pDBSaveData[atvDataStartOffset]), atvByteCount);
		for (i = 1; i <= MSD309_ATV_CHECKSUM_BYTE_SIZE; i++)
		{
			dataBlockInfo.pDBSaveData[atvByteCheckSumOffset + MSD309_ATV_CHECKSUM_BYTE_SIZE - i] = atvCheckSum % 256;
			atvCheckSum /= 256;
		}
		for (i = 1; i <= MSD309_ATV_CHANNEL_COUNT_BYTE_SIZE; i++)
		{
			dataBlockInfo.pDBSaveData[atvByteCountOffset + MSD309_ATV_CHANNEL_COUNT_BYTE_SIZE - i] = atvByteCount % 256;
			atvByteCount /= 256;
		}
		//copy between 
		memcpy(&(dataBlockInfo.pDBSaveData[nowSaveDataOffset]), &(dataBlockInfo.pSourceData[dataBlockInfo.atv309ChannelDataOffset + dataBlockInfo.atv309ChannelDataSize]), dataBlockInfo.dtv309ChannelDataOffset - dataBlockInfo.atv309ChannelDataOffset - dataBlockInfo.atv309ChannelDataSize);
		nowSaveDataOffset += dataBlockInfo.dtv309ChannelDataOffset - dataBlockInfo.atv309ChannelDataOffset - dataBlockInfo.atv309ChannelDataSize;
		//copy dtv
		sort(allChannelVector.begin(), allChannelVector.end(), SortByDtvIndex);
		unsigned int dtvStartStrLen, dtvEndStrLen, dtvDataStartOffset = 0, dtvByteCountOffset = 0, dtvByteCheckSumOffset = 0, dtvByteCount = 0, dtvCheckSum = 0;
		dtvStartStrLen = strlen(MSD309_DTV_ITEM_INFO_START_STR);
		dtvEndStrLen = strlen(MSD309_DTV_ITEM_INFO_END_STR);
		memcpy(&(dataBlockInfo.pDBSaveData[nowSaveDataOffset]), &(dataBlockInfo.pSourceData[dataBlockInfo.dtv309ChannelDataOffset]), MSD309_DTV_ALL_DATA_START_BYTE_SIZE);
		nowSaveDataOffset += MSD309_DTV_ALL_DATA_START_BYTE_SIZE + MSD309_DTV_CHANNEL_COUNT_BYTE_SIZE + MSD309_DTV_CHECKSUM_BYTE_SIZE;
		dtvByteCountOffset = nowSaveDataOffset - MSD309_DTV_CHANNEL_COUNT_BYTE_SIZE - MSD309_DTV_CHECKSUM_BYTE_SIZE;
		dtvByteCheckSumOffset = nowSaveDataOffset - MSD309_DTV_CHECKSUM_BYTE_SIZE;
		dtvDataStartOffset = nowSaveDataOffset;
		int tvType = TV_DTV_TYPE;
		for (tvType = TV_DTV_TYPE; tvType < TV_TYPE_END; tvType++)
		{
			for (i = 0; i < allChannelVector.size(); i++)
			{
				if (allChannelVector[i].channelType == TV_ATV_TYPE)
				{
					break;
				}
				if (allChannelVector[i].channelType == tvType)
				{
					pTmpData = (unsigned char *)malloc(allChannelVector[i].dbChannelItemDataSize);
					memcpy(pTmpData, &(dataBlockInfo.pSourceData[allChannelVector[i].dbChannelItemDataOffset]), allChannelVector[i].dbChannelItemDataSize);
					memcpy(&pTmpData[dtvStartStrLen + MSD309_DTV_CHANNEL_NAME_BYTE_OFFSET], allChannelVector[i].name, MSD309_DTV_CHANNEL_NAME_BYTE_SIZE);
					if (allChannelVector[i].isLock)
					{
						pTmpData[dtvStartStrLen + MSD309_DTV_CHANNEL_LOCK_BYTE_OFFSET] |= MSD309_DTV_CHANNEL_IS_LOCK;
					}
					else
					{
						pTmpData[dtvStartStrLen + MSD309_DTV_CHANNEL_LOCK_BYTE_OFFSET] &= ~MSD309_DTV_CHANNEL_IS_LOCK;
					}
					if (allChannelVector[i].isSkip)
					{
						pTmpData[dtvStartStrLen + MSD309_DTV_CHANNEL_SKIP_BYTE_OFFSET] |= MSD309_DTV_CHANNEL_IS_SKIP;
					}
					else
					{
						pTmpData[dtvStartStrLen + MSD309_DTV_CHANNEL_SKIP_BYTE_OFFSET] &= ~MSD309_DTV_CHANNEL_IS_SKIP;
					}
					pTmpData[dtvStartStrLen + MSD309_DTV_CHANNEL_POS_HIGH_BYTE_OFFSET] = allChannelVector[i].channelPos / 256;
					pTmpData[dtvStartStrLen + MSD309_DTV_CHANNEL_POS_LOW_BYTE_OFFSET] = allChannelVector[i].channelPos % 256;
					memcpy(&(dataBlockInfo.pDBSaveData[nowSaveDataOffset]), pTmpData, allChannelVector[i].dbChannelItemDataSize);
					nowSaveDataOffset += allChannelVector[i].dbChannelItemDataSize;
					dtvByteCount += allChannelVector[i].dbChannelItemDataSize;
					free(pTmpData);
				}
			}
			switch (tvType)
			{
			case TV_DTV_TYPE:
				dataBlockInfo.pDBSaveData[nowSaveDataOffset] = dataBlockInfo.tvDtvTypeCountry;
				nowSaveDataOffset++;
				dtvByteCount++;
				break;
			case TV_RADIO_TYPE:
				dataBlockInfo.pDBSaveData[nowSaveDataOffset] = dataBlockInfo.tvRadioTypeCountry;
				nowSaveDataOffset++;
				dtvByteCount++;
				break;
			case TV_DATA_TYPE:
				dataBlockInfo.pDBSaveData[nowSaveDataOffset] = dataBlockInfo.tvDataTypeCountry;
				nowSaveDataOffset++;
				dtvByteCount++;
				break;
			default:
				break;
			}
		}
		dtvCheckSum = TVCalCheckSum(&(dataBlockInfo.pDBSaveData[dtvDataStartOffset]), dtvByteCount);
		for (i = 1; i <= MSD309_DTV_CHECKSUM_BYTE_SIZE; i++)
		{
			dataBlockInfo.pDBSaveData[dtvByteCheckSumOffset + MSD309_DTV_CHECKSUM_BYTE_SIZE - i] = dtvCheckSum % 256;
			dtvCheckSum /= 256;
		}
		dtvByteCount -= 2;//country 只算了一个 所以减去2
		for (i = 1; i <= MSD309_DTV_CHANNEL_COUNT_BYTE_SIZE; i++)
		{
			dataBlockInfo.pDBSaveData[dtvByteCountOffset + MSD309_DTV_CHANNEL_COUNT_BYTE_SIZE - i] = dtvByteCount % 256;
			dtvByteCount /= 256;
		}
		//copy checksum
		dataCheckSum = DataCalCheckSum(dataBlockInfo.pDBSaveData, dataBlockInfo.sourceDataLen - deleteChannelSize - MSD309_DATABASE_CHECKSUM_BYTE_SIZE);
		for (i = 1; i <= MSD309_DATABASE_CHECKSUM_BYTE_SIZE; i++)
		{
			dataBlockInfo.pDBSaveData[dataBlockInfo.sourceDataLen - deleteChannelSize - i] = dataCheckSum % 256;
			dataCheckSum /= 256;
		}

		//save to db
		sqlite3_stmt *stat = 0;
		if (sqlite3_prepare(pChannelDataDB, "update SETTINGS SET DATA = ? where FIELDNAME='TunerRawData'", -1, &stat, 0) == SQLITE_OK){
			if (sqlite3_bind_blob(stat, 1, dataBlockInfo.pDBSaveData, dataBlockInfo.sourceDataLen - deleteChannelSize, NULL) == SQLITE_OK)
			{
				if (sqlite3_step(stat) != SQLITE_DONE)
				{
					result = FALSE;
				}
			}
			else
			{
				result = FALSE;
			}
		}
		else{
			result = FALSE;
		}
		sqlite3_finalize(stat);
		if (dataBlockInfo.tvSecureChannelMajor != 0)
		{
			unsigned int tmpSecureChannelMajor = 0;
			unsigned int tmpSecureChannelMinor = 0;
			BYTE tmpSecureChannelEnable = 0;
			for (i = 0; i < allChannelVector.size(); i++)
			{
				if (allChannelVector[i].channelOldPos == dataBlockInfo.tvSecureChannelMajor - 1)
				{
					tmpSecureChannelMajor = allChannelVector[i].channelPos + 1;
					tmpSecureChannelEnable = allChannelVector[i].isLock ? 0x01: 0x00;
				}
			}
			if (sqlite3_prepare(pChannelDataDB, "update SETTINGS SET DATA = ? where FIELDNAME='SecureChannelMajor'", -1, &stat, 0) == SQLITE_OK){
				pTmpData = (unsigned char *)malloc(2);
				pTmpData[0] = tmpSecureChannelMajor / 256;
				pTmpData[1] = tmpSecureChannelMajor % 256;
				if (sqlite3_bind_blob(stat, 1, pTmpData, 2, NULL) == SQLITE_OK)
				{
					if (sqlite3_step(stat) != SQLITE_DONE)
					{
						result = FALSE;
					}
				}
				else
				{
					result = FALSE;
				}
				free(pTmpData);
			}
			else{
				result = FALSE;
			}
			sqlite3_finalize(stat);
			if (sqlite3_prepare(pChannelDataDB, "update SETTINGS SET DATA = ? where FIELDNAME='SecureChannelEnabled'", -1, &stat, 0) == SQLITE_OK){
				pTmpData = (unsigned char *)malloc(1);
				pTmpData[0] = tmpSecureChannelEnable;
				if (sqlite3_bind_blob(stat, 1, pTmpData, 1, NULL) == SQLITE_OK)
				{
					if (sqlite3_step(stat) != SQLITE_DONE)
					{
						result = FALSE;
					}
				}
				else
				{
					result = FALSE;
				}
				free(pTmpData);
			}
			else{
				result = FALSE;
			}
			sqlite3_finalize(stat);
			if (sqlite3_prepare(pChannelDataDB, "update SETTINGS SET DATA = ? where FIELDNAME='SecureChannelMinor'", -1, &stat, 0) == SQLITE_OK){
				pTmpData = (unsigned char *)malloc(2);
				pTmpData[0] = tmpSecureChannelMinor / 256;
				pTmpData[1] = tmpSecureChannelMinor % 256;
				if (sqlite3_bind_blob(stat, 1, pTmpData, 2, NULL) == SQLITE_OK)
				{
					if (sqlite3_step(stat) != SQLITE_DONE)
					{
						result = FALSE;
					}
				}
				else
				{
					result = FALSE;
				}
				free(pTmpData);
			}
			else{
				result = FALSE;
			}
			sqlite3_finalize(stat);
		}
		//free
		if (dataBlockInfo.pDBSaveData != NULL)
		{
			free(dataBlockInfo.pDBSaveData);
			dataBlockInfo.pDBSaveData = NULL;
		}
		sort(allChannelVector.begin(), allChannelVector.end(), SortByPos);
	}
	return result;
}
BOOL CVTDBUtil::MSD3393SaveDataToDb()
{
	USES_CONVERSION;
	BOOL result = TRUE;
	unsigned char * pTmpData;
	unsigned char * pTmpCheckSum;
	unsigned int i,j,count, dataCheckSum = 0, nowSaveDataOffset = 0, deleteChannelSize = 0;
	if (pChannelDataDB == NULL)
	{
		return FALSE;
	}
	else
	{
		if (pDbFilePath == NULL){
			return FALSE;
		}
		for (i = 0; i < deleteChannelVector.size(); i++)
		{
			deleteChannelSize += deleteChannelVector[i].dbChannelItemDataSize;
			deleteChannelSize += MSD3393_TV_CHANNEL_INFO_BYTE_SIZE;//每个TV占用40个字节
		}
		if (dataBlockInfo.pDBSaveData != NULL)
		{
			free(dataBlockInfo.pDBSaveData);
			dataBlockInfo.pDBSaveData = NULL;
		}
		dataBlockInfo.pDBSaveData = (unsigned char *)malloc(dataBlockInfo.sourceDataLen - deleteChannelSize);
		memset(dataBlockInfo.pDBSaveData, 0, dataBlockInfo.sourceDataLen - deleteChannelSize);
		//copy before tvclone data contain FM TV 
		//copy fm data
		unsigned int totalFmCount, totalTvCount;
		totalFmCount = dataBlockInfo.pSourceData[MSD3393_FM_CHANNEL_COUNT_HIGH_BYTE_OFFSET] * 256 + dataBlockInfo.pSourceData[MSD3393_FM_CHANNEL_COUNT_LOW_BYTE_OFFSET];
		memcpy(dataBlockInfo.pDBSaveData, dataBlockInfo.pSourceData, MSD3393_DATABASE_HEAD_BYTE_SIZE + MSD3393_FM_CHANNEL_COUNT_BYTE_SIZE + MSD3393_TV_CHANNEL_COUNT_BYTE_SIZE + totalFmCount*MSD3393_FM_CHANNEL_INFO_BYTE_SIZE);
		//set tv count
		totalTvCount = allChannelVector.size();
		dataBlockInfo.pDBSaveData[MSD3393_TV_CHANNEL_COUNT_HIGH_BYTE_OFFSET] = totalTvCount / 256;
		dataBlockInfo.pDBSaveData[MSD3393_TV_CHANNEL_COUNT_LOW_BYTE_OFFSET] = totalTvCount % 256;
		nowSaveDataOffset += MSD3393_DATABASE_HEAD_BYTE_SIZE + MSD3393_FM_CHANNEL_COUNT_BYTE_SIZE + MSD3393_TV_CHANNEL_COUNT_BYTE_SIZE + totalFmCount*MSD3393_FM_CHANNEL_INFO_BYTE_SIZE;
		//copy tv data
		memset(&dataBlockInfo.pDBSaveData[nowSaveDataOffset], 0, totalTvCount*MSD3393_TV_CHANNEL_INFO_BYTE_SIZE);
		sort(allChannelVector.begin(), allChannelVector.end(), SortByPos);
		for (i = 0; i < totalTvCount; i++)
		{
			unsigned int serverType = MSD3393_TV_ATV_TYPE;
			switch (allChannelVector[i].channelType)
			{
			case TV_DTV_TYPE:
				serverType = MSD3393_TV_DTV_TYPE;
				break;
			case TV_RADIO_TYPE:
				serverType = MSD3393_TV_RADIO_TYPE;
				break;
			case TV_DATA_TYPE:
				serverType = MSD3393_TV_DATA_TYPE;
				break;
			default:
				serverType = MSD3393_TV_ATV_TYPE;
				break;
			}
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD3393_TV_CHANNEL_INFO_BYTE_SIZE + MSD3393_TV_CHANNEL_INFO_SERVICE_TYPE_BYTE_OFFSET] = serverType;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD3393_TV_CHANNEL_INFO_BYTE_SIZE + MSD3393_TV_CHANNEL_INFO_MAJOR_CHANNO_HIGH_BYTE_OFFSET] = allChannelVector [i].tvMajorNum/ 256;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD3393_TV_CHANNEL_INFO_BYTE_SIZE + MSD3393_TV_CHANNEL_INFO_MAJOR_CHANNO_LOW_BYTE_OFFSET] = allChannelVector[i].tvMajorNum % 256;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD3393_TV_CHANNEL_INFO_BYTE_SIZE + MSD3393_TV_CHANNEL_INFO_MINOR_CHANNO_HIGH_BYTE_OFFSET] = allChannelVector[i].tvMinorNum / 256;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD3393_TV_CHANNEL_INFO_BYTE_SIZE + MSD3393_TV_CHANNEL_INFO_MINOR_CHANNO_LOW_BYTE_OFFSET] = allChannelVector[i].tvMinorNum % 256;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD3393_TV_CHANNEL_INFO_BYTE_SIZE + MSD3393_TV_CHANNEL_INFO_IS_SECURE_BYTE_OFFSET] = allChannelVector[i].isLock ? MSD3393_TV_CHANNEL_INFO_IS_SECURE_DATA : MSD3393_TV_CHANNEL_INFO_NOT_SECURE_DATA;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD3393_TV_CHANNEL_INFO_BYTE_SIZE + MSD3393_TV_CHANNEL_INFO_IS_PROME_BYTE_OFFSET] = MSD3393_TV_CHANNEL_INFO_DEFAULT_IS_PROME;
			memcpy(&dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*MSD3393_TV_CHANNEL_INFO_BYTE_SIZE + MSD3393_TV_CHANNEL_INFO_CHNAME_BYTE_OFFSET], allChannelVector[i].name, MSD3393_TV_CHANNEL_NAME_BYTE_SIZE);
		}
		nowSaveDataOffset += totalTvCount*MSD3393_TV_CHANNEL_INFO_BYTE_SIZE;
		/*memcpy(dataBlockInfo.pDBSaveData, dataBlockInfo.pSourceData, MSD3393_DATABASE_HEAD_BYTE_SIZE + MSD3393_FM_CHANNEL_COUNT_BYTE_SIZE + MSD3393_TV_CHANNEL_COUNT_BYTE_SIZE + totalFmCount*MSD3393_FM_CHANNEL_INFO_BYTE_SIZE);
		//set tv count
		dataBlockInfo.pDBSaveData[MSD3393_TV_CHANNEL_COUNT_HIGH_BYTE_OFFSET] = totalTvCount / 256;
		dataBlockInfo.pDBSaveData[MSD3393_TV_CHANNEL_COUNT_LOW_BYTE_OFFSET] = totalTvCount % 256;
		nowSaveDataOffset += MSD3393_DATABASE_HEAD_BYTE_SIZE + MSD3393_FM_CHANNEL_COUNT_BYTE_SIZE + MSD3393_TV_CHANNEL_COUNT_BYTE_SIZE + totalFmCount*MSD3393_FM_CHANNEL_INFO_BYTE_SIZE;
		//copy tv data
		for (i = 0; i < allChannelVector.size(); i++)
		{
			unsigned int tmpoffset = 0;
			tmpoffset = MSD3393_DATABASE_HEAD_BYTE_SIZE + MSD3393_FM_CHANNEL_COUNT_BYTE_SIZE + MSD3393_TV_CHANNEL_COUNT_BYTE_SIZE + totalFmCount*MSD3393_FM_CHANNEL_INFO_BYTE_SIZE + allChannelVector[i].tvVirChIdx * MSD3393_TV_CHANNEL_INFO_BYTE_SIZE;
			memcpy(&dataBlockInfo.pDBSaveData[nowSaveDataOffset], &dataBlockInfo.pSourceData[tmpoffset], MSD3393_TV_CHANNEL_INFO_BYTE_SIZE);
			//set data
			//
			nowSaveDataOffset += MSD3393_TV_CHANNEL_INFO_BYTE_SIZE;
		}*/
		//memcpy(dataBlockInfo.pDBSaveData, dataBlockInfo.pSourceData, dataBlockInfo.tvCloneDataOffset);
		//nowSaveDataOffset += dataBlockInfo.tvCloneDataOffset;
		//copy clone data
		memcpy(&dataBlockInfo.pDBSaveData[nowSaveDataOffset], dataBlockInfo.pTvCloneData, dataBlockInfo.tv3393ChannelDataOffset - dataBlockInfo.tvCloneDataOffset);
		nowSaveDataOffset += dataBlockInfo.tv3393ChannelDataOffset - dataBlockInfo.tvCloneDataOffset;
		//copy tvchannel
		memcpy(&dataBlockInfo.pDBSaveData[nowSaveDataOffset],dataBlockInfo.p3393TvChannelData , MSD3393_TV_CHANNEL_ALL_DATA_START_BYTE_SIZE + MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_DATA_LEN_BYTE_SIZE + MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_CHECKSUM_BYTE_SIZE);
		unsigned int totalTvByte = (totalTvCount + 1) * MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_ITEM_BYTE_SIZE;
		unsigned int tmpTotalTvByte = totalTvByte;
		for (i = 1; i <= MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_DATA_LEN_BYTE_SIZE; i++)
		{
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + MSD3393_TV_CHANNEL_ALL_DATA_START_BYTE_SIZE + MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_DATA_LEN_BYTE_SIZE - i] = tmpTotalTvByte % 256;
			tmpTotalTvByte /= 256;
		}
		nowSaveDataOffset += MSD3393_TV_CHANNEL_ALL_DATA_START_BYTE_SIZE + MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_DATA_LEN_BYTE_SIZE + MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_CHECKSUM_BYTE_SIZE;
		pTmpData = &(dataBlockInfo.pDBSaveData[nowSaveDataOffset]);
		pTmpCheckSum = &(dataBlockInfo.pDBSaveData[nowSaveDataOffset - MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_CHECKSUM_BYTE_SIZE]);
		memcpy(&dataBlockInfo.pDBSaveData[nowSaveDataOffset], dataBlockInfo.p3393TvChannelVirtualChannelData, MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_ITEM_BYTE_SIZE);
		nowSaveDataOffset += MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_ITEM_BYTE_SIZE;
		sort(allChannelVector.begin(), allChannelVector.end(), SortByOldVirChIdx);
		for (i = 0; i < allChannelVector.size(); i++)
		{
			unsigned int tmpoffset = 0;
			tmpoffset = dataBlockInfo.tv3393ChannelVirtualChannelDataOffset + allChannelVector[i].tvOldVirChIdx * MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_ITEM_BYTE_SIZE;
			memcpy(&dataBlockInfo.pDBSaveData[nowSaveDataOffset], &dataBlockInfo.pSourceData[tmpoffset], MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_ITEM_BYTE_SIZE);
			allChannelVector[i].tvVirChIdx = i + 1;
			memcpy(&dataBlockInfo.pDBSaveData[nowSaveDataOffset + MSD3393_VIRTUAL_CHANNEL_SERVICE_NAME_BYTE_OFFSET], allChannelVector[i].name, MSD3393_TV_CHANNEL_INFO_SERVICE_NAME_BYTE_SIZE);
			nowSaveDataOffset += MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_ITEM_BYTE_SIZE;
		}
		unsigned int virChannelCheckSum = TVCalCheckSum(pTmpData, totalTvByte);
		for (i = 1; i <= MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_CHECKSUM_BYTE_SIZE; i++)
		{
			pTmpCheckSum[MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_CHECKSUM_BYTE_SIZE - i] = virChannelCheckSum % 256;
			virChannelCheckSum /= 256;
		}
		dataBlockInfo.pDBSaveData[nowSaveDataOffset] = 0x00;
		nowSaveDataOffset += 1;
		memcpy(&dataBlockInfo.pDBSaveData[nowSaveDataOffset], dataBlockInfo.p3393TvChannelChannelSettingDataLen, dataBlockInfo.tv3393ChannelChannelSettingDataSize + MSD3393_TVCHANNEL_CHANNEL_SETTING_DATA_LEN_BYTE_SIZE + MSD3393_TVCHANNEL_CHANNEL_SETTING_CHECKSUM_BYTE_SIZE);
		//db 3393TvChannelChannelSettingData
		pTmpData = &dataBlockInfo.pDBSaveData[nowSaveDataOffset + MSD3393_TVCHANNEL_CHANNEL_SETTING_DATA_LEN_BYTE_SIZE + MSD3393_TVCHANNEL_CHANNEL_SETTING_CHECKSUM_BYTE_SIZE];
		pTmpCheckSum = &dataBlockInfo.pDBSaveData[nowSaveDataOffset + MSD3393_TVCHANNEL_CHANNEL_SETTING_DATA_LEN_BYTE_SIZE];
		nowSaveDataOffset += dataBlockInfo.tv3393ChannelChannelSettingDataSize + MSD3393_TVCHANNEL_CHANNEL_SETTING_DATA_LEN_BYTE_SIZE + MSD3393_TVCHANNEL_CHANNEL_SETTING_CHECKSUM_BYTE_SIZE;
		dataBlockInfo.pDBSaveData[nowSaveDataOffset] = 0x00;
		nowSaveDataOffset += 1;
		memset(pTmpData, 0, MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_BYTE_OFFSET + MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_COUNT*MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_STRUCT_BYTE_SIZE);
		//update u16MainListSrvNum
		pTmpData[MSD3393_CHANNEL_SETTING_MAIN_LIST_SRV_NUM_HIGH_BYTE_OFFSET] = totalTvCount / 256;
		pTmpData[MSD3393_CHANNEL_SETTING_MAIN_LIST_SRV_NUM_LOW_BYTE_OFFSET] = totalTvCount % 256;
		//update astMainList
		sort(allChannelVector.begin(), allChannelVector.end(), SortByPos);
		for (i = 0; i < allChannelVector.size(); i++)
		{
			memcpy(&pTmpData[MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_BYTE_OFFSET + ((i+1)*MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_STRUCT_BYTE_SIZE)], 
				&dataBlockInfo.p3393TvChannelChannelSettingData[MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_BYTE_OFFSET + ((allChannelVector[i].channelOldPos + 1) * MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_STRUCT_BYTE_SIZE)], 
				MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_STRUCT_BYTE_SIZE);
			for (j = 0; j < deleteChannelVector.size(); j++)
			{
				if (allChannelVector[i].tvOldPhysicalChIdx == deleteChannelVector[j].tvOldPhysicalChIdx)
				{
					if (deleteChannelVector[j].tvOldVirChInfoIdx < allChannelVector[i].tvOldVirChInfoIdx)
					{
						allChannelVector[i].tvVirChInfoIdx--;
					}
				}
			}
			pTmpData[MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_BYTE_OFFSET + ((i + 1)*MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_STRUCT_BYTE_SIZE) + 1] = allChannelVector[i].tvVirChInfoIdx;
		}
		//update astPhysicalCh
		sort(allChannelVector.begin(), allChannelVector.end(), SortByOldPhysicalChIdx);
		/*for (i = 0; i < allChannelVector.size(); i++)
		{
			unsigned int tmpNumOfVirCh = 0, tmpVirChInfoStartIdx = 1;
			tmpNumOfVirCh = dataBlockInfo.p3393TvChannelChannelSettingData[MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_BYTE_OFFSET + allChannelVector[i].tvOldPhysicalChIdx * MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_STRUCT_BYTE_SIZE];
			for (j = 0; j < deleteChannelVector.size(); j++)
			{
				if (allChannelVector[i].tvOldPhysicalChIdx == deleteChannelVector[j].tvOldPhysicalChIdx)
				{
					tmpNumOfVirCh--;
				}
			}
			for (j = 0; j < allChannelVector.size(); j++)
			{
				if (allChannelVector[j].tvOldPhysicalChIdx < allChannelVector[i].tvOldPhysicalChIdx)
				{
					tmpVirChInfoStartIdx++;
				}
				else
				{
					break;
				}

			}
			pTmpData[MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_BYTE_OFFSET + allChannelVector[i].tvOldPhysicalChIdx * MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_STRUCT_BYTE_SIZE] = tmpNumOfVirCh;
			allChannelVector[i].tvVirChInfoStartIdx = tmpVirChInfoStartIdx;
			pTmpData[MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_BYTE_OFFSET + allChannelVector[i].tvOldPhysicalChIdx * MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_STRUCT_BYTE_SIZE + 2] = tmpVirChInfoStartIdx%256;
			pTmpData[MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_BYTE_OFFSET + allChannelVector[i].tvOldPhysicalChIdx * MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_STRUCT_BYTE_SIZE + 3] = tmpVirChInfoStartIdx / 256;
		}*/
		unsigned int tmpVirChInfoStartIdx = 1;
		for (count = 0; count < MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_COUNT; count++)
		{
			unsigned int tmpNumOfVirCh = 0;
			for (i = 0; i < allChannelVector.size(); i++)
			{
				if (allChannelVector[i].tvOldPhysicalChIdx == count)
				{
					tmpNumOfVirCh++;
				}
			}
			for (i = 0; i < allChannelVector.size(); i++)
			{
				if (allChannelVector[i].tvOldPhysicalChIdx == count)
				{
					allChannelVector[i].tvVirChInfoStartIdx = tmpVirChInfoStartIdx;
				}
			}
			pTmpData[MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_BYTE_OFFSET + count * MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_STRUCT_BYTE_SIZE] = tmpNumOfVirCh;
			pTmpData[MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_BYTE_OFFSET + count * MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_STRUCT_BYTE_SIZE + 2] = tmpVirChInfoStartIdx % 256;
			pTmpData[MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_BYTE_OFFSET + count * MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_STRUCT_BYTE_SIZE + 3] = tmpVirChInfoStartIdx / 256;
			tmpVirChInfoStartIdx += tmpNumOfVirCh;
		}
		//update astVirtualChInfo
		for (i = 0; i < allChannelVector.size(); i++)
		{
			allChannelVector[i].tvVirPos = allChannelVector[i].tvVirChInfoStartIdx + allChannelVector[i].tvVirChInfoIdx;
			memcpy(&pTmpData[MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_BYTE_OFFSET + allChannelVector[i].tvVirPos*MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_STRUCT_BYTE_SIZE],
				&dataBlockInfo.p3393TvChannelChannelSettingData[MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_BYTE_OFFSET + allChannelVector[i].tvOldVirPos*MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_STRUCT_BYTE_SIZE], MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_STRUCT_BYTE_SIZE);
			/*for (j = 0; j < deleteChannelVector.size(); j++)
			{
				if (deleteChannelVector[j].tvOldVirChIdx < allChannelVector[i].tvOldVirChIdx)
				{
					allChannelVector[i].tvVirChIdx--;
				}
			}*/
			pTmpData[MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_BYTE_OFFSET + allChannelVector[i].tvVirPos*MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_STRUCT_BYTE_SIZE] = allChannelVector[i].tvVirChIdx % 256;
			pTmpData[MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_BYTE_OFFSET + allChannelVector[i].tvVirPos*MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_STRUCT_BYTE_SIZE + 1] |= (0X03 & (allChannelVector[i].tvVirChIdx / 256));
			if (allChannelVector[i].isLock)
			{
				pTmpData[MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_BYTE_OFFSET + allChannelVector[i].tvVirPos*MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_STRUCT_BYTE_SIZE + MSD3393_VIRTUAL_CHANNEL_INFO_IS_LOCK_CH_BYTE_OFFSET] |= MSD3393_VIRTUAL_CHANNEL_INFO_IS_LOCK_CH_BYTE_BIT;
			}
			else
			{
				pTmpData[MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_BYTE_OFFSET + allChannelVector[i].tvVirPos*MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_STRUCT_BYTE_SIZE + MSD3393_VIRTUAL_CHANNEL_INFO_IS_LOCK_CH_BYTE_OFFSET] &= ~MSD3393_VIRTUAL_CHANNEL_INFO_IS_LOCK_CH_BYTE_BIT;
			}
		}
		//update au8VirChTbl
		sort(allChannelVector.begin(), allChannelVector.end(), SortByOldVirChIdx);
		unsigned char tmpTbl[MSD3393_CHANNEL_SETTING_VIR_CH_TBL_COUNT];
		unsigned int tmpOffset, tmpCount;
		memset(tmpTbl, 0, MSD3393_CHANNEL_SETTING_VIR_CH_TBL_COUNT);
		for (i = 0; i < allChannelVector.size(); i++)
		{
			tmpOffset = allChannelVector[i].tvVirChIdx;
			tmpCount = 0;
			while ((tmpOffset / 8) != 0)
			{
				tmpOffset /= 8;
				tmpCount++;
			}
			if (allChannelVector[i].isEffective)
			{
				tmpTbl[tmpCount] |= (0x01 << tmpOffset);
			}
			else
			{
				tmpTbl[tmpCount] &= ~(0x01 << tmpOffset);
			}
		}
		memcpy(&pTmpData[MSD3393_CHANNEL_SETTING_VIR_CH_TBL_BYTE_OFFSET], tmpTbl, MSD3393_CHANNEL_SETTING_VIR_CH_TBL_COUNT);
		//check sum
		unsigned int channelSettingCheckSum = TVCalCheckSum(pTmpData, dataBlockInfo.tv3393ChannelChannelSettingDataSize);
		for (i = 1; i <= MSD3393_TVCHANNEL_CHANNEL_SETTING_CHECKSUM_BYTE_SIZE; i++)
		{
			pTmpCheckSum[MSD3393_TVCHANNEL_CHANNEL_SETTING_CHECKSUM_BYTE_SIZE - i] = channelSettingCheckSum % 256;
			channelSettingCheckSum /= 256;
		}
		//copy tvlastchannel
		memcpy(&dataBlockInfo.pDBSaveData[nowSaveDataOffset],dataBlockInfo.p3393TvLastChannelData,dataBlockInfo.tv3393LastChannelDataSize);
		nowSaveDataOffset += dataBlockInfo.tv3393LastChannelDataSize;
		dataBlockInfo.pDBSaveData[nowSaveDataOffset] = 0x00;
		nowSaveDataOffset += 1;
		//checksum
		dataCheckSum = DataCalCheckSum(dataBlockInfo.pDBSaveData,nowSaveDataOffset);
		for (i = 1; i <= MSD3393_DATABASE_CHECKSUM_BYTE_SIZE; i++)
		{
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + MSD3393_DATABASE_CHECKSUM_BYTE_SIZE - i] = dataCheckSum % 256;
			dataCheckSum /= 256;
		}
		printf("%d %d\n", nowSaveDataOffset, dataBlockInfo.sourceDataLen - deleteChannelSize);
		//restore data must do 
		for (i = 0; i < allChannelVector.size(); i++)
		{
			allChannelVector[i].tvPhysicalChIdx = allChannelVector[i].tvOldPhysicalChIdx;
			allChannelVector[i].tvVirChInfoStartIdx = allChannelVector[i].tvOldVirChInfoStartIdx;
			allChannelVector[i].tvVirChInfoIdx = allChannelVector[i].tvOldVirChInfoIdx;
			allChannelVector[i].tvVirPos = allChannelVector[i].tvOldVirPos;
			allChannelVector[i].tvVirChIdx = allChannelVector[i].tvOldVirChIdx;
		}
		sqlite3_stmt *stat = 0;
		if (sqlite3_prepare(pChannelDataDB, "update SETTINGS SET DATA = ? where FIELDNAME='TunerRawData'", -1, &stat, 0) == SQLITE_OK){
			if (sqlite3_bind_blob(stat, 1, dataBlockInfo.pDBSaveData, dataBlockInfo.sourceDataLen - deleteChannelSize, NULL) == SQLITE_OK)
			{
				if (sqlite3_step(stat) != SQLITE_DONE)
				{
					result = FALSE;
				}
			}
			else
			{
				result = FALSE;
			}
		}
		else{
			result = FALSE;
		}
		sqlite3_finalize(stat);
		//free
		if (dataBlockInfo.pDBSaveData != NULL)
		{
			free(dataBlockInfo.pDBSaveData);
			dataBlockInfo.pDBSaveData = NULL;
		}
		sort(allChannelVector.begin(), allChannelVector.end(), SortByPos);
	}
	return result;
}
BOOL CVTDBUtil::MSD3393ParseRAWData()
{
	USES_CONVERSION;
	BOOL result = TRUE;
	sqlite3_stmt *stat = 0;
	unsigned int i = 0;
	if (sqlite3_prepare(pChannelDataDB, "select DATA from SETTINGS where FIELDNAME='SecureChannelEnabled'", -1, &stat, 0) == SQLITE_OK){
		if (sqlite3_step(stat) == SQLITE_ROW)
		{
			const void * pFileContent = sqlite3_column_blob(stat, 0);
			const int size = sqlite3_column_bytes(stat, 0);
			dataBlockInfo.tvSecureChannelEnable = ((unsigned char*)pFileContent)[0];
		}
	}
	sqlite3_finalize(stat);
	if (sqlite3_prepare(pChannelDataDB, "select DATA from SETTINGS where FIELDNAME='SecureChannelMajor'", -1, &stat, 0) == SQLITE_OK){
		if (sqlite3_step(stat) == SQLITE_ROW)
		{
			const void * pFileContent = sqlite3_column_blob(stat, 0);
			const int size = sqlite3_column_bytes(stat, 0);
			dataBlockInfo.tvSecureChannelMajor = ((unsigned char*)pFileContent)[0] * 256 + ((unsigned char*)pFileContent)[1];
		}
	}
	sqlite3_finalize(stat);
	if (sqlite3_prepare(pChannelDataDB, "select DATA from SETTINGS where FIELDNAME='SecureChannelMinor'", -1, &stat, 0) == SQLITE_OK){
		if (sqlite3_step(stat) == SQLITE_ROW)
		{
			const void * pFileContent = sqlite3_column_blob(stat, 0);
			const int size = sqlite3_column_bytes(stat, 0);
			dataBlockInfo.tvSecureChannelMinor = ((unsigned char*)pFileContent)[0] * 256 + ((unsigned char*)pFileContent)[1];
		}
	}
	sqlite3_finalize(stat);
	if (sqlite3_prepare(pChannelDataDB, "select DATA from SETTINGS where FIELDNAME='TunerRawData'", -1, &stat, 0) == SQLITE_OK){
		if (sqlite3_step(stat) == SQLITE_ROW)
		{
			const void * pFileContent = sqlite3_column_blob(stat, 0);
			const int size = sqlite3_column_bytes(stat, 0);
			dataBlockInfo.pSourceData = (unsigned char *)malloc(size);
			dataBlockInfo.sourceDataLen = size;
			memcpy((unsigned char *)dataBlockInfo.pSourceData, (unsigned char *)pFileContent, size);
		}
	}
	else
	{
		sqlite3_finalize(stat);
		return FALSE;
	}
	sqlite3_finalize(stat);
	dataBlockInfo.tvCloneDataOffset = MSD3393_DATABASE_HEAD_BYTE_SIZE + MSD3393_FM_CHANNEL_COUNT_BYTE_SIZE + MSD3393_TV_CHANNEL_COUNT_BYTE_SIZE + (dataBlockInfo.pSourceData[MSD3393_FM_CHANNEL_COUNT_HIGH_BYTE_OFFSET] * 256 +
		dataBlockInfo.pSourceData[MSD3393_FM_CHANNEL_COUNT_LOW_BYTE_OFFSET])*MSD3393_FM_CHANNEL_INFO_BYTE_SIZE + (dataBlockInfo.pSourceData[MSD3393_TV_CHANNEL_COUNT_HIGH_BYTE_OFFSET] * 256 +
		dataBlockInfo.pSourceData[MSD3393_TV_CHANNEL_COUNT_LOW_BYTE_OFFSET])*MSD3393_TV_CHANNEL_INFO_BYTE_SIZE;
	dataBlockInfo.pTvCloneData = &dataBlockInfo.pSourceData[dataBlockInfo.tvCloneDataOffset];
	dataBlockInfo.tvCloneDataSize = dataBlockInfo.sourceDataLen - MSD3393_DATABASE_CHECKSUM_BYTE_SIZE - dataBlockInfo.tvCloneDataOffset;
	unsigned int tmpOffset;
	if (MemFindDataFirstOffset(dataBlockInfo.pTvCloneData, dataBlockInfo.tvCloneDataSize, (unsigned char *)MSD3393_TV_CHANNEL_INFO_START_STR, strlen(MSD3393_TV_CHANNEL_INFO_START_STR), tmpOffset))
	{
		dataBlockInfo.p3393TvChannelData = &dataBlockInfo.pTvCloneData[tmpOffset];
		dataBlockInfo.tv3393ChannelDataOffset = tmpOffset + dataBlockInfo.tvCloneDataOffset;
	}
	else
	{
		return FALSE;
	}
	if (MemFindDataFirstOffset(dataBlockInfo.pTvCloneData, dataBlockInfo.tvCloneDataSize, (unsigned char *)MSD3393_TV_LAST_CHANNEL_INFO_START_STR, strlen(MSD3393_TV_LAST_CHANNEL_INFO_START_STR), tmpOffset))
	{
		dataBlockInfo.p3393TvLastChannelData = &dataBlockInfo.pTvCloneData[tmpOffset];
		dataBlockInfo.tv3393LastChannelDataOffset = tmpOffset + dataBlockInfo.tvCloneDataOffset;
	}
	else
	{
		return FALSE;
	}
	dataBlockInfo.tv3393ChannelDataSize = dataBlockInfo.tv3393LastChannelDataOffset - dataBlockInfo.tv3393ChannelDataOffset;
	dataBlockInfo.tv3393LastChannelDataSize = dataBlockInfo.sourceDataLen - dataBlockInfo.tv3393LastChannelDataOffset - MSD3393_DATABASE_CHECKSUM_BYTE_SIZE - MSD3393_ZERO_END_BYTE_SIZE;
	dataBlockInfo.p3393TvChannelVirtualChannelDataLen = &dataBlockInfo.p3393TvChannelData[MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_HEAD_BYTE_SIZE];

	dataBlockInfo.p3393TvChannelVirtualChannelData = &dataBlockInfo.p3393TvChannelData[MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_HEAD_BYTE_SIZE
		+ MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_DATA_LEN_BYTE_SIZE + MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_CHECKSUM_BYTE_SIZE];

	dataBlockInfo.tv3393ChannelVirtualChannelDataOffset = dataBlockInfo.tv3393ChannelDataOffset + MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_HEAD_BYTE_SIZE
		+ MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_DATA_LEN_BYTE_SIZE + MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_CHECKSUM_BYTE_SIZE;

	dataBlockInfo.tv3393ChannelVirtualChannelDataSize = dataBlockInfo.p3393TvChannelData[MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_HEAD_BYTE_SIZE] * 256 * 256 * 256 
		+ dataBlockInfo.p3393TvChannelData[MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_HEAD_BYTE_SIZE + 1] * 256 * 256
		+ dataBlockInfo.p3393TvChannelData[MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_HEAD_BYTE_SIZE + 2] * 256
		+ dataBlockInfo.p3393TvChannelData[MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_HEAD_BYTE_SIZE + 3];
	dataBlockInfo.p3393TvChannelChannelSettingDataLen = &dataBlockInfo.p3393TvChannelVirtualChannelData[dataBlockInfo.tv3393ChannelVirtualChannelDataSize + MSD3393_ZERO_END_BYTE_SIZE];

	dataBlockInfo.p3393TvChannelChannelSettingData = &dataBlockInfo.p3393TvChannelVirtualChannelData[dataBlockInfo.tv3393ChannelVirtualChannelDataSize 
		+ MSD3393_ZERO_END_BYTE_SIZE + MSD3393_TVCHANNEL_CHANNEL_SETTING_DATA_LEN_BYTE_SIZE + MSD3393_TVCHANNEL_CHANNEL_SETTING_CHECKSUM_BYTE_SIZE];

	dataBlockInfo.tv3393ChannelChannelSettingDataOffset = dataBlockInfo.tv3393ChannelVirtualChannelDataOffset + dataBlockInfo.tv3393ChannelVirtualChannelDataSize 
		+ MSD3393_ZERO_END_BYTE_SIZE + MSD3393_TVCHANNEL_CHANNEL_SETTING_DATA_LEN_BYTE_SIZE + MSD3393_TVCHANNEL_CHANNEL_SETTING_CHECKSUM_BYTE_SIZE;

	dataBlockInfo.tv3393ChannelChannelSettingDataSize = dataBlockInfo.tv3393LastChannelDataOffset - dataBlockInfo.tv3393ChannelChannelSettingDataOffset - MSD3393_ZERO_END_BYTE_SIZE;
	//get u16MainListSrvNum
	dataBlockInfo.tv3393MainListSrvNum = dataBlockInfo.p3393TvChannelChannelSettingData[MSD3393_CHANNEL_SETTING_MAIN_LIST_SRV_NUM_HIGH_BYTE_OFFSET] * 256 
		+ dataBlockInfo.p3393TvChannelChannelSettingData[MSD3393_CHANNEL_SETTING_MAIN_LIST_SRV_NUM_LOW_BYTE_OFFSET];
	for (i = 1; i <= dataBlockInfo.tv3393MainListSrvNum; i++)
	{
		ChannelInfo channelInfo;
		channelInfo.channelOldPos = i - 1;
		channelInfo.channelPos = i - 1;
		channelInfo.tvPhysicalChIdx = dataBlockInfo.p3393TvChannelChannelSettingData[MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_BYTE_OFFSET + i*MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_STRUCT_BYTE_SIZE + 0];
		channelInfo.tvVirChInfoStartIdx = dataBlockInfo.p3393TvChannelChannelSettingData[MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_BYTE_OFFSET + channelInfo.tvPhysicalChIdx*MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_STRUCT_BYTE_SIZE + 2]
			+ (dataBlockInfo.p3393TvChannelChannelSettingData[MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_BYTE_OFFSET + channelInfo.tvPhysicalChIdx*MSD3393_CHANNEL_SETTING_MS_PHYSICAL_CHANNEL_STRUCT_BYTE_SIZE + 3] & 0x03) * 256;
		channelInfo.tvVirChInfoIdx = (dataBlockInfo.p3393TvChannelChannelSettingData[MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_BYTE_OFFSET + i*MSD3393_CHANNEL_SETTING_MS_MAINLIST_A_STRUCT_BYTE_SIZE + 1] & 0x7F);
		channelInfo.tvVirPos = channelInfo.tvVirChInfoIdx + channelInfo.tvVirChInfoStartIdx;
		channelInfo.tvVirChIdx = (dataBlockInfo.p3393TvChannelChannelSettingData[MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_BYTE_OFFSET + channelInfo.tvVirPos*MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_STRUCT_BYTE_SIZE] +
			((dataBlockInfo.p3393TvChannelChannelSettingData[MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_BYTE_OFFSET + channelInfo.tvVirPos*MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_STRUCT_BYTE_SIZE + 1]) & 0x03 * 256));
		//old value
		channelInfo.tvOldPhysicalChIdx = channelInfo.tvPhysicalChIdx;
		channelInfo.tvOldVirChInfoStartIdx = channelInfo.tvVirChInfoStartIdx;
		channelInfo.tvOldVirChInfoIdx = channelInfo.tvVirChInfoIdx;
		channelInfo.tvOldVirPos = channelInfo.tvVirPos;
		channelInfo.tvOldVirChIdx = channelInfo.tvVirChIdx;

		MSD3393VirtualChannelInfo virtualInfo;
		MSD3393GetChannelIndoByVirChIdx(channelInfo.tvVirChIdx, virtualInfo);
		channelInfo.tvMajorNum = virtualInfo.majorNum;
		channelInfo.tvMinorNum = virtualInfo.minorNum;
		memcpy(channelInfo.name, virtualInfo.serviceName, MSD3393_TV_CHANNEL_INFO_SERVICE_NAME_BYTE_SIZE);
		channelInfo.channelType = virtualInfo.serviceType;
		channelInfo.isLock = (dataBlockInfo.p3393TvChannelChannelSettingData[MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_BYTE_OFFSET + channelInfo.tvVirPos*MSD3393_CHANNEL_SETTING_MS_VIRTUAL_CHANNEL_INFO_STRUCT_BYTE_SIZE + MSD3393_VIRTUAL_CHANNEL_INFO_IS_LOCK_CH_BYTE_OFFSET] & MSD3393_VIRTUAL_CHANNEL_INFO_IS_LOCK_CH_BYTE_BIT) ? true : false;

		channelInfo.dbChannelItemDataOffset = dataBlockInfo.tv3393ChannelVirtualChannelDataOffset + i*MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_ITEM_BYTE_SIZE;
		channelInfo.dbChannelItemDataSize = MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_ITEM_BYTE_SIZE;
		//isEffective
		unsigned char tmpTbl[MSD3393_CHANNEL_SETTING_VIR_CH_TBL_COUNT];
		unsigned int tmpOffset = i, tmpCount = 0;
		memcpy(tmpTbl, &dataBlockInfo.p3393TvChannelChannelSettingData[MSD3393_CHANNEL_SETTING_VIR_CH_TBL_BYTE_OFFSET], MSD3393_CHANNEL_SETTING_VIR_CH_TBL_COUNT);
		while ((tmpOffset / 8 )!= 0)
		{
			tmpOffset /= 8;
			tmpCount++;
		}
		channelInfo.isEffective = ((tmpTbl[tmpCount] & (0x01 << tmpOffset)) == 0) ? false : true;
		allChannelVector.push_back(channelInfo);
		printf("pos = %d  tvPhysicalChIdx=%d tvVirChInfoStartIdx=%d tvVirChInfoIdx=%d tvVirPos=%d tvVirChIdx=%d\n", \
			channelInfo.channelPos, channelInfo.tvPhysicalChIdx, channelInfo.tvVirChInfoStartIdx, channelInfo.tvVirChInfoIdx, channelInfo.tvVirPos, channelInfo.tvVirChIdx);
	}
	return result;
}
BOOL CVTDBUtil::MSD3393GetChannelIndoByVirChIdx(unsigned int index, MSD3393VirtualChannelInfo & info)
{
	unsigned int infoCount = dataBlockInfo.tv3393ChannelVirtualChannelDataSize / MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_ITEM_BYTE_SIZE;
	unsigned int indexOffset = index*MSD3393_TVCHANNEL_VIRTUAL_CHANNEL_ITEM_BYTE_SIZE;
	if (index > infoCount)
	{
		return FALSE;
	}

	memcpy(info.serviceName, &dataBlockInfo.p3393TvChannelVirtualChannelData[indexOffset + MSD3393_VIRTUAL_CHANNEL_SERVICE_NAME_BYTE_OFFSET], MSD3393_TV_CHANNEL_INFO_SERVICE_NAME_BYTE_SIZE);
	switch (dataBlockInfo.p3393TvChannelVirtualChannelData[indexOffset + MSD3393_VIRTUAL_CHANNEL_SERVICE_TYPE_BYTE_OFFSET])
	{
	case MSD3393_TV_ATV_TYPE:
		info.serviceType = TV_ATV_TYPE;
		break;
	case MSD3393_TV_DTV_TYPE:
		info.serviceType = TV_DTV_TYPE;
		break;
	case MSD3393_TV_RADIO_TYPE:
		info.serviceType = TV_RADIO_TYPE;
		break;
	case MSD3393_TV_DATA_TYPE:
		info.serviceType = TV_DATA_TYPE;
		break;
	default:
		info.serviceType = TV_ATV_TYPE;
		break;
	}
	info.majorNum = dataBlockInfo.p3393TvChannelVirtualChannelData[indexOffset + MSD3393_VIRTUAL_CHANNEL_MAJOR_NUM_HIGH_BYTE_OFFSET] * 256 + dataBlockInfo.p3393TvChannelVirtualChannelData[indexOffset + MSD3393_VIRTUAL_CHANNEL_MAJOR_NUM_LOW_BYTE_OFFSET];
	info.minorNum = dataBlockInfo.p3393TvChannelVirtualChannelData[indexOffset + MSD3393_VIRTUAL_CHANNEL_MINOR_NUM_HIGH_BYTE_OFFSET] * 256 + dataBlockInfo.p3393TvChannelVirtualChannelData[indexOffset + MSD3393_VIRTUAL_CHANNEL_MINOR_NUM_LOW_BYTE_OFFSET];
	return TRUE;
}
ChannelVector* CVTDBUtil::GetChannelVectorByPos()
{
	sort(allChannelVector.begin(), allChannelVector.end(), SortByPos);
	return &allChannelVector;
}
unsigned int CVTDBUtil::DeleteSelectChannel()
{
	unsigned int count = 0;
	for (vector<ChannelInfo>::iterator it = allChannelVector.begin(); it != allChannelVector.end();)
	{
		if ((*it).isSelect == true)
		{
			deleteChannelVector.push_back(*it);
			it = allChannelVector.erase(it);
			count++;
		}
		else
		{
			it++;
		}
	}
	sort(allChannelVector.begin(), allChannelVector.end(), SortByPos);
	return count;
}
BOOL CVTDBUtil::UpadtePosValueToContinue()
{
	sort(allChannelVector.begin(), allChannelVector.end(), SortByPos);
	for (unsigned int i = 0; i < allChannelVector.size(); i++)
	{
		allChannelVector[i].channelPos = i;
	}
	sort(allChannelVector.begin(), allChannelVector.end(), SortByPos);
	return TRUE;
}

BOOL CVTDBUtil::ChangeChannnelPos(unsigned int from, unsigned int to)
{
	unsigned int i;
	if (from == to)
	{
		return TRUE;
	}
	else if (from < to)
	{
		for (i = 0; i < allChannelVector.size(); i++)
		{
			if (allChannelVector[i].channelPos == from)
			{
				allChannelVector[i].channelPos = to;
			}
			else
			{
				if ((allChannelVector[i].channelPos > from) && (allChannelVector[i].channelPos <= to))
				{
					allChannelVector[i].channelPos--;
				}
			}
		}
	}
	else
	{
		for (i = 0; i < allChannelVector.size(); i++)
		{
			if (allChannelVector[i].channelPos == from)
			{
				allChannelVector[i].channelPos = to;
			}
			else
			{
				if ((allChannelVector[i].channelPos < from) && (allChannelVector[i].channelPos >= to))
				{
					allChannelVector[i].channelPos++;
				}
			}
		}
	}
	sort(allChannelVector.begin(), allChannelVector.end(), SortByPos);
	return TRUE;
}
BOOL CVTDBUtil::UpdateAtvChannelNo()
{
	sort(allChannelVector.begin(), allChannelVector.end(), SortByAtvChannelNo);
	unsigned int i,tmpNo = 0;
	for (i = 0; i < allChannelVector.size(); i++)
	{
		if (allChannelVector[i].channelType == TV_ATV_TYPE)
		{
			allChannelVector[i].atvChannelNo = tmpNo;
			tmpNo++;
		}
	}
	sort(allChannelVector.begin(), allChannelVector.end(), SortByPos);
	return TRUE;
}
BOARD_TYPE CVTDBUtil::GetBoardType()
{
	return dataBlockInfo.boardType;
}
const int CVTDBUtil::GetTvNameByteSize(TV_TYPE tvType)
{
	switch (dataBlockInfo.boardType)
	{
	case BOARD_T_MSD30X_B55TA_TYPE:
		if (tvType == TV_ATV_TYPE)
		{
			return MSD309_ATV_CHANNEL_NAME_BYTE_SIZE;
		}
		else
		{
			return MSD309_DTV_CHANNEL_NAME_BYTE_SIZE;
		}
		break;
	case BOARD_MSD3393_CVT_55A_TYPE:
		return MSD3393_TV_CHANNEL_INFO_SERVICE_NAME_BYTE_SIZE;
		break;
	default:
		break;
	}
	return 0;
}
unsigned int CVTDBUtil::TVCalCheckSum(BYTE *pBuf, DWORD wBufLen)
{
	unsigned int CheckSum;
	CheckSum = 0;

	while (wBufLen > 2)//calculate pBuf[2]~pBuf[End] do not count pBuf[0] pBuf[1],it is checksum
	{
		--wBufLen;
		CheckSum += pBuf[wBufLen];
	}
	//ask to mstar
	return (CheckSum + 0x55);
}
unsigned int CVTDBUtil::DataCalCheckSum(BYTE *pBuf, DWORD wBufLen)
{
	unsigned int CheckSum = 0, i;
	for (i = 0; i < wBufLen; i++)
	{
		CheckSum += pBuf[i];
	}
	return CheckSum;
}
BOOL CVTDBUtil::MemFindDataFirstOffset(unsigned char * sourceData, unsigned int sourceDataLen, unsigned char * findData, unsigned int findDataLen, unsigned int & offset)
{
	unsigned int i, j, count;
	count = sourceDataLen - findDataLen + 1;
	//printf("sourceDataLen = %d", sourceDataLen);
	for (i = 0; i < count; i++)
	{
		for (j = 0; j < findDataLen; j++)
		{
			if (sourceData[i + j] != findData[j])
			{
				break;
			}
			//printf("i=%d : %c\n", i,sourceData[i+j]);
			if (j == findDataLen - 1)
			{
				offset = i;
				return TRUE;
			}
		}
	}
	return FALSE;
}