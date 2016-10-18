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
	dataBlockInfo.pAtvChannelData = NULL;
	dataBlockInfo.pDtvChannelData = NULL;
	dataBlockInfo.sourceDataLen = 0;
	dataBlockInfo.atvChannelDataOffset = 0;
	dataBlockInfo.dtvChannelDataOffset = 0;
	dataBlockInfo.tvCloneDataOffset = 0;
	dataBlockInfo.atvChannelDataSize = 0;
	dataBlockInfo.dtvChannelDataSize = 0;
	dataBlockInfo.tvCloneDataSize = 0;
	dataBlockInfo.atvCount = 0;
	dataBlockInfo.dtvCount = 0;
	dataBlockInfo.tvDtvTypeCountry = 0;
	dataBlockInfo.tvRadioTypeCountry = 0;
	dataBlockInfo.tvDataTypeCountry = 0;
	dataBlockInfo.tvSecureChannelMajor = 0;
	dataBlockInfo.tvSecureChannelMinor = 0;
	dataBlockInfo.tvSecureChannelEnable = 0;
	pChannelDataDB = NULL;
	allChannelVector.clear();
	deleteChannelVector.clear();
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
	dataBlockInfo.pAtvChannelData = NULL;
	dataBlockInfo.pDtvChannelData = NULL;
	dataBlockInfo.sourceDataLen = 0;
	dataBlockInfo.atvChannelDataOffset = 0;
	dataBlockInfo.dtvChannelDataOffset = 0;
	dataBlockInfo.tvCloneDataOffset = 0;
	dataBlockInfo.atvChannelDataSize = 0;
	dataBlockInfo.dtvChannelDataSize = 0;
	dataBlockInfo.tvCloneDataSize = 0;
	dataBlockInfo.atvCount = 0;
	dataBlockInfo.dtvCount = 0;
	dataBlockInfo.tvDtvTypeCountry = 0;
	dataBlockInfo.tvRadioTypeCountry = 0;
	dataBlockInfo.tvDataTypeCountry = 0;
	dataBlockInfo.tvSecureChannelMajor = 0;
	dataBlockInfo.tvSecureChannelMinor = 0;
	dataBlockInfo.tvSecureChannelEnable = 0;
	pChannelDataDB = NULL;
	allChannelVector.clear();
	deleteChannelVector.clear();
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
	dataBlockInfo.pAtvChannelData = NULL;
	dataBlockInfo.pDtvChannelData = NULL;
	dataBlockInfo.sourceDataLen = 0;
	dataBlockInfo.atvChannelDataOffset = 0;
	dataBlockInfo.dtvChannelDataOffset = 0;
	dataBlockInfo.tvCloneDataOffset = 0;
	dataBlockInfo.atvChannelDataSize = 0;
	dataBlockInfo.dtvChannelDataSize = 0;
	dataBlockInfo.tvCloneDataSize = 0;
	dataBlockInfo.atvCount = 0;
	dataBlockInfo.dtvCount = 0;
	dataBlockInfo.tvDtvTypeCountry = 0;
	dataBlockInfo.tvRadioTypeCountry = 0;
	dataBlockInfo.tvDataTypeCountry = 0;
	dataBlockInfo.tvSecureChannelMajor = 0;
	dataBlockInfo.tvSecureChannelMinor = 0;
	dataBlockInfo.tvSecureChannelEnable = 0;
	allChannelVector.clear();
	deleteChannelVector.clear();
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
	InitData();
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
			unsigned int tmpOffset;
			if (MemFindDataFirstOffset(dataBlockInfo.pSourceData, dataBlockInfo.sourceDataLen, (unsigned char *)BOARD_T_MSD30X_B55TA_STR, strlen(BOARD_T_MSD30X_B55TA_STR), tmpOffset))
			{
				dataBlockInfo.boardType = BOARD_T_MSD30X_B55TA_TYPE;
			}
		}
	}
	else{
		sqlite3_finalize(stat);
		return FALSE;
	}
	sqlite3_finalize(stat);
	if (dataBlockInfo.pSourceData == NULL)
	{
		return FALSE;
	}
	dataBlockInfo.tvCloneDataOffset = (dataBlockInfo.pSourceData[FM_CHANNEL_COUNT_HIGH_BYTE_OFFSET] * 256 + dataBlockInfo.pSourceData[FM_CHANNEL_COUNT_LOW_BYTE_OFFSET]) * FM_CHANNEL_INFO_BYTE_SIZE +
		(dataBlockInfo.pSourceData[TV_CHANNEL_COUNT_HIGH_BYTE_OFFSET] * 256 + dataBlockInfo.pSourceData[TV_CHANNEL_COUNT_LOW_BYTE_OFFSET]) * TV_CHANNEL_INFO_BYTE_SIZE +
		DATABASE_HEAD_BYTE_SIZE + FM_CHANNEL_COUNT_BYTE_SIZE + TV_CHANNEL_COUNT_BYTE_SIZE;
	dataBlockInfo.pTvCloneData = &dataBlockInfo.pSourceData[dataBlockInfo.tvCloneDataOffset];
	dataBlockInfo.tvCloneDataSize = dataBlockInfo.sourceDataLen - dataBlockInfo.tvCloneDataOffset  - DATABASE_CHECKSUM_BYTE_SIZE;
	unsigned int tmpSize = dataBlockInfo.sourceDataLen - dataBlockInfo.tvCloneDataOffset;
	unsigned int tmpOffset;
	if (MemFindDataFirstOffset(dataBlockInfo.pTvCloneData, tmpSize, (unsigned char *)ATV_INFO_START_STR, strlen(ATV_INFO_START_STR), tmpOffset))
	{
		dataBlockInfo.pAtvChannelData = &dataBlockInfo.pTvCloneData[tmpOffset];
		dataBlockInfo.atvChannelDataOffset = tmpOffset + dataBlockInfo.tvCloneDataOffset;
	}
	else
	{
		return FALSE;
	}
	if (MemFindDataFirstOffset(dataBlockInfo.pTvCloneData, tmpSize, (unsigned char *)DTV_INFO_START_STR, strlen(DTV_INFO_START_STR), tmpOffset))
	{
		dataBlockInfo.pDtvChannelData = &dataBlockInfo.pTvCloneData[tmpOffset];
		dataBlockInfo.dtvChannelDataOffset = tmpOffset + dataBlockInfo.tvCloneDataOffset;
	}
	else
	{
		return FALSE;
	}
	if (!ParseATVData())
	{
		return FALSE;
	}
	if (!ParseDTVData())
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CVTDBUtil::ParseATVData()
{
	if (dataBlockInfo.pAtvChannelData == NULL)
	{
		return FALSE;
	}
	unsigned char * pTmpData;
	unsigned int startOffset, tmpSize, tmpOffset, atvStartStrLen, atvEndStrLen, channelIndex = 0;
	pTmpData = dataBlockInfo.pAtvChannelData;
	tmpSize = dataBlockInfo.sourceDataLen - dataBlockInfo.atvChannelDataOffset;
	atvStartStrLen = strlen(ATV_ITEM_INFO_START_STR);
	atvEndStrLen = strlen(ATV_ITEM_INFO_END_STR);
	while (MemFindDataFirstOffset(pTmpData, tmpSize, (unsigned char *)ATV_ITEM_INFO_START_STR, atvStartStrLen, startOffset))
	{
		if (!MemFindDataFirstOffset(&pTmpData[startOffset + ATV_CHANNEL_DATA_BYTE_SIZE + 5], atvEndStrLen, (unsigned char *)ATV_ITEM_INFO_END_STR, atvEndStrLen, tmpOffset))
		{
			pTmpData = &pTmpData[startOffset + ATV_CHANNEL_DATA_BYTE_SIZE + atvEndStrLen + atvStartStrLen];
			tmpSize = tmpSize - ATV_CHANNEL_DATA_BYTE_SIZE - startOffset - atvEndStrLen - atvStartStrLen;
			continue;
		}
		ChannelInfo channelInfo;
		channelInfo.dbChannelItemDataOffset = &pTmpData[startOffset] - dataBlockInfo.pSourceData;
		channelInfo.dbChannelItemDataSize = ATV_CHANNEL_DATA_BYTE_SIZE + atvEndStrLen + atvStartStrLen;
		channelInfo.channelPos = pTmpData[startOffset + atvStartStrLen + ATV_CHANNEL_POS_HIGH_BYTE_OFFSET] * 256 + pTmpData[startOffset + atvStartStrLen + ATV_CHANNEL_POS_LOW_BYTE_OFFSET];
		channelInfo.channelOldPos = channelInfo.channelPos;
		channelInfo.channelType = TV_ATV_TYPE;
		channelInfo.isLock = ((pTmpData[startOffset + atvStartStrLen + ATV_CHANNEL_LOCK_BYTE_OFFSET] & ATV_CHANNEL_IS_LOCK) != 0);
		channelInfo.isSkip = ((pTmpData[startOffset + atvStartStrLen + ATV_CHANNEL_SKIP_BYTE_OFFSET] & ATV_CHANNEL_IS_SKIP) != 0);
		memcpy(channelInfo.name, &pTmpData[startOffset + atvStartStrLen + ATV_CHANNEL_NAME_BYTE_OFFSET], ATV_CHANNEL_NAME_BYTE_SIZE);
		channelInfo.atvChannelNo = pTmpData[startOffset + atvStartStrLen + ATV_CHANNEL_NO_BYTE_OFFSET];
		channelInfo.indexForAtv = channelIndex;
		allChannelVector.push_back(channelInfo);
		//printf("ATV channelInfo.dbDataOffset=%d,channelInfo.channelPos=%d,channelInfo.channelType=%d,channelInfo.isLock=%d,channelInfo.isSkip=%d,channelInfo.atvChannelNo=%d,channelInfo.indexForAtv=%d\n", \
			channelInfo.dbDataOffset, channelInfo.channelPos, channelInfo.channelType, channelInfo.isLock, channelInfo.isSkip, channelInfo.atvChannelNo, channelInfo.indexForAtv);
		pTmpData = &pTmpData[startOffset + ATV_CHANNEL_DATA_BYTE_SIZE + atvEndStrLen + atvStartStrLen];
		tmpSize = tmpSize - ATV_CHANNEL_DATA_BYTE_SIZE - startOffset - atvEndStrLen - atvStartStrLen;
		channelIndex++;
	}
	dataBlockInfo.atvCount = channelIndex;
	if (channelIndex > 0)
	{
		dataBlockInfo.atvChannelDataSize = (pTmpData - dataBlockInfo.pSourceData) - dataBlockInfo.atvChannelDataOffset;
	}
	else
	{
		dataBlockInfo.atvChannelDataSize = ATV_ALL_DATA_START_BYTE_SIZE + ATV_CHANNEL_COUNT_BYTE_SIZE + ATV_CHECKSUM_BYTE_SIZE;
	}
	return TRUE;
}
BOOL CVTDBUtil::ParseDTVData()
{
	USES_CONVERSION;
	if (dataBlockInfo.pDtvChannelData == NULL)
	{
		return FALSE;
	}
	unsigned char * pTmpData;
	unsigned int startOffset, tmpSize, tmpOffset, dtvStartStrLen, dtvEndStrLen,channelIndex = 0;
	pTmpData = dataBlockInfo.pDtvChannelData;
	tmpSize = dataBlockInfo.sourceDataLen - dataBlockInfo.dtvChannelDataOffset;
	dtvStartStrLen = strlen(DTV_ITEM_INFO_START_STR);
	dtvEndStrLen = strlen(DTV_ITEM_INFO_END_STR);
	while (MemFindDataFirstOffset(pTmpData, tmpSize, (unsigned char *)DTV_ITEM_INFO_START_STR, dtvStartStrLen, startOffset))
	{
		if (!MemFindDataFirstOffset(&pTmpData[startOffset + DTV_CHANNEL_DATA_BYTE_SIZE + 5], dtvEndStrLen, (unsigned char *)DTV_ITEM_INFO_END_STR, dtvEndStrLen, tmpOffset))
		{
			pTmpData = &pTmpData[startOffset + DTV_CHANNEL_DATA_BYTE_SIZE + dtvStartStrLen + dtvEndStrLen];
			tmpSize = tmpSize - DTV_CHANNEL_DATA_BYTE_SIZE - startOffset - dtvStartStrLen - dtvEndStrLen;
			continue;
		}
		ChannelInfo channelInfo;
		channelInfo.dbChannelItemDataOffset = &pTmpData[startOffset] - dataBlockInfo.pSourceData;
		channelInfo.dbChannelItemDataSize = DTV_CHANNEL_DATA_BYTE_SIZE + dtvStartStrLen + dtvEndStrLen;
		channelInfo.channelPos = pTmpData[startOffset + dtvStartStrLen + DTV_CHANNEL_POS_HIGH_BYTE_OFFSET] * 256 + pTmpData[startOffset + dtvStartStrLen + DTV_CHANNEL_POS_LOW_BYTE_OFFSET];
		channelInfo.channelOldPos = channelInfo.channelPos;
		channelInfo.channelType = pTmpData[startOffset + dtvStartStrLen + DTV_CHANNEL_SERVICE_TYPE_BYTE_OFFSET];
		channelInfo.isLock = ((pTmpData[startOffset + dtvStartStrLen + DTV_CHANNEL_LOCK_BYTE_OFFSET] & DTV_CHANNEL_IS_LOCK) != 0);
		channelInfo.isSkip = ((pTmpData[startOffset + dtvStartStrLen + DTV_CHANNEL_SKIP_BYTE_OFFSET] & DTV_CHANNEL_IS_SKIP) != 0);
		memcpy(channelInfo.name, &pTmpData[startOffset + dtvStartStrLen + DTV_CHANNEL_NAME_BYTE_OFFSET], DTV_CHANNEL_NAME_BYTE_SIZE);
		channelInfo.indexForDtv = channelIndex;
		allChannelVector.push_back(channelInfo);
		//printf("DTV channelInfo.dbDataOffset=%d,channelInfo.channelPos=%d,channelInfo.channelType=%d,channelInfo.isLock=%d,channelInfo.isSkip=%d,channelInfo.indexForDtv=%d\n",\
					channelInfo.dbDataOffset, channelInfo.channelPos, channelInfo.channelType, channelInfo.isLock, channelInfo.isSkip, channelInfo.indexForDtv);
		pTmpData = &pTmpData[startOffset + DTV_CHANNEL_DATA_BYTE_SIZE + dtvStartStrLen + dtvEndStrLen];
		tmpSize = tmpSize - DTV_CHANNEL_DATA_BYTE_SIZE - startOffset - dtvStartStrLen - dtvEndStrLen;
		channelIndex++;
	}
	dataBlockInfo.dtvCount = channelIndex;
	if (channelIndex > 0)
	{
		dataBlockInfo.dtvChannelDataSize = (pTmpData - dataBlockInfo.pSourceData) - dataBlockInfo.dtvChannelDataOffset;
	}
	else
	{
		dataBlockInfo.dtvChannelDataSize = DTV_ALL_DATA_START_BYTE_SIZE + DTV_CHANNEL_COUNT_BYTE_SIZE + DTV_CHECKSUM_BYTE_SIZE;
	}
	//get country
	unsigned int i = 0, dtvCountryOffset = 0;
	int tvType = TV_DTV_TYPE;
	dtvCountryOffset = DTV_ALL_DATA_START_BYTE_SIZE + DTV_CHANNEL_COUNT_BYTE_SIZE + DTV_CHECKSUM_BYTE_SIZE;
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
			dataBlockInfo.tvDtvTypeCountry = dataBlockInfo.pDtvChannelData[dtvCountryOffset];
			break;
		case TV_RADIO_TYPE:
			dataBlockInfo.tvRadioTypeCountry = dataBlockInfo.pDtvChannelData[dtvCountryOffset];
			break;
		case TV_DATA_TYPE:
			dataBlockInfo.tvDataTypeCountry = dataBlockInfo.pDtvChannelData[dtvCountryOffset];
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
			deleteChannelSize += TV_CHANNEL_INFO_BYTE_SIZE;//每个TV占用40个字节
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
		totalFmCount = dataBlockInfo.pSourceData[FM_CHANNEL_COUNT_HIGH_BYTE_OFFSET] * 256 + dataBlockInfo.pSourceData[FM_CHANNEL_COUNT_LOW_BYTE_OFFSET];
		memcpy(dataBlockInfo.pDBSaveData, dataBlockInfo.pSourceData, DATABASE_HEAD_BYTE_SIZE + FM_CHANNEL_COUNT_BYTE_SIZE + TV_CHANNEL_COUNT_BYTE_SIZE + totalFmCount*FM_CHANNEL_INFO_BYTE_SIZE);
		//set tv count
		totalTvCount = allChannelVector.size();
		dataBlockInfo.pDBSaveData[TV_CHANNEL_COUNT_HIGH_BYTE_OFFSET] = totalTvCount / 256;
		dataBlockInfo.pDBSaveData[TV_CHANNEL_COUNT_LOW_BYTE_OFFSET] = totalTvCount % 256;
		nowSaveDataOffset += DATABASE_HEAD_BYTE_SIZE + FM_CHANNEL_COUNT_BYTE_SIZE + TV_CHANNEL_COUNT_BYTE_SIZE + totalFmCount * FM_CHANNEL_INFO_BYTE_SIZE;
		//copy tv data
		memset(&dataBlockInfo.pDBSaveData[nowSaveDataOffset], 0, totalTvCount*TV_CHANNEL_INFO_BYTE_SIZE);
		sort(allChannelVector.begin(), allChannelVector.end(), SortByPos);
		for (i = 0; i < totalTvCount;i++)
		{
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*TV_CHANNEL_INFO_BYTE_SIZE + TV_CHANNEL_INFO_SERVICE_TYPE_BYTE_OFFSET] = allChannelVector[i].channelType;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*TV_CHANNEL_INFO_BYTE_SIZE + TV_CHANNEL_INFO_MAJOR_CHANNO_HIGH_BYTE_OFFSET] = (allChannelVector[i].channelPos + 1) / 256;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*TV_CHANNEL_INFO_BYTE_SIZE + TV_CHANNEL_INFO_MAJOR_CHANNO_LOW_BYTE_OFFSET] = (allChannelVector[i].channelPos + 1) % 256;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*TV_CHANNEL_INFO_BYTE_SIZE + TV_CHANNEL_INFO_MINOR_CHANNO_HIGH_BYTE_OFFSET] = TV_CHANNEL_INFO_DEFAULT_MINOR_CHANNO / 256;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*TV_CHANNEL_INFO_BYTE_SIZE + TV_CHANNEL_INFO_MINOR_CHANNO_LOW_BYTE_OFFSET] = TV_CHANNEL_INFO_DEFAULT_MINOR_CHANNO % 256;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*TV_CHANNEL_INFO_BYTE_SIZE + TV_CHANNEL_INFO_IS_SECURE_BYTE_OFFSET] = allChannelVector[i].isLock ? TV_CHANNEL_INFO_IS_SECURE_DATA : TV_CHANNEL_INFO_NOT_SECURE_DATA;
			dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*TV_CHANNEL_INFO_BYTE_SIZE + TV_CHANNEL_INFO_IS_PROME_BYTE_OFFSET] = TV_CHANNEL_INFO_DEFAULT_IS_PROME;
			if (allChannelVector[i].channelType == TV_ATV_TYPE)
			{
				memcpy(&dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*TV_CHANNEL_INFO_BYTE_SIZE + TV_CHANNEL_INFO_CHNAME_BYTE_OFFSET], allChannelVector[i].name, ATV_CHANNEL_NAME_BYTE_SIZE);
			}
			else
			{
				WORD unicodeName[DTV_CHANNEL_NAME_BYTE_SIZE];
				MApp_TranslateCharTableToUnicode((BYTE *)allChannelVector[i].name, unicodeName, DTV_CHANNEL_NAME_BYTE_SIZE);
				CString unicodeStr;
				unicodeStr.Format(L"%s", unicodeName);
				char * utf8name = (char *)malloc(DTV_CHANNEL_NAME_BYTE_SIZE);
				memset(utf8name, 0, DTV_CHANNEL_NAME_BYTE_SIZE);
				utf8name = UnicodeToUtf8(unicodeStr);
				memcpy(&dataBlockInfo.pDBSaveData[nowSaveDataOffset + i*TV_CHANNEL_INFO_BYTE_SIZE + TV_CHANNEL_INFO_CHNAME_BYTE_OFFSET], utf8name, DTV_CHANNEL_NAME_BYTE_SIZE);
			}
		}
		nowSaveDataOffset += totalTvCount*TV_CHANNEL_INFO_BYTE_SIZE;
		//copy before atv data
		memcpy(&(dataBlockInfo.pDBSaveData[nowSaveDataOffset]), &(dataBlockInfo.pSourceData[dataBlockInfo.tvCloneDataOffset]), dataBlockInfo.atvChannelDataOffset - dataBlockInfo.tvCloneDataOffset);
		nowSaveDataOffset += dataBlockInfo.atvChannelDataOffset - dataBlockInfo.tvCloneDataOffset;
		//copy atv
		sort(allChannelVector.begin(), allChannelVector.end(), SortByAtvIndex);
		unsigned int atvStartStrLen, atvEndStrLen, atvDataStartOffset = 0, atvByteCountOffset = 0, atvByteCheckSumOffset = 0, atvByteCount = 0, atvCheckSum = 0;
		atvStartStrLen = strlen(ATV_ITEM_INFO_START_STR);
		atvEndStrLen = strlen(ATV_ITEM_INFO_END_STR);
		memcpy(&(dataBlockInfo.pDBSaveData[nowSaveDataOffset]), &(dataBlockInfo.pSourceData[dataBlockInfo.atvChannelDataOffset]), ATV_ALL_DATA_START_BYTE_SIZE);
		nowSaveDataOffset += ATV_ALL_DATA_START_BYTE_SIZE + ATV_CHANNEL_COUNT_BYTE_SIZE + ATV_CHECKSUM_BYTE_SIZE;
		atvByteCountOffset = nowSaveDataOffset - ATV_CHECKSUM_BYTE_SIZE - ATV_CHANNEL_COUNT_BYTE_SIZE;
		atvByteCheckSumOffset = nowSaveDataOffset - ATV_CHECKSUM_BYTE_SIZE;
		atvDataStartOffset = nowSaveDataOffset;
		for (i = 0; i < allChannelVector.size(); i++)
		{
			if (allChannelVector[i].channelType != TV_ATV_TYPE)
			{
				break;
			}
			pTmpData = (unsigned char *)malloc(allChannelVector[i].dbChannelItemDataSize);
			memcpy(pTmpData, &(dataBlockInfo.pSourceData[allChannelVector[i].dbChannelItemDataOffset]), allChannelVector[i].dbChannelItemDataSize);
			memcpy(&pTmpData[atvStartStrLen + ATV_CHANNEL_NAME_BYTE_OFFSET], allChannelVector[i].name, ATV_CHANNEL_NAME_BYTE_SIZE);
			if (allChannelVector[i].isLock)
			{
				pTmpData[atvStartStrLen + ATV_CHANNEL_LOCK_BYTE_OFFSET] |= ATV_CHANNEL_IS_LOCK;
			}
			else
			{
				pTmpData[atvStartStrLen + ATV_CHANNEL_LOCK_BYTE_OFFSET] &= ~ATV_CHANNEL_IS_LOCK;
			}
			if (allChannelVector[i].isSkip)
			{
				pTmpData[atvStartStrLen + ATV_CHANNEL_SKIP_BYTE_OFFSET] |= ATV_CHANNEL_IS_SKIP;
			}
			else
			{
				pTmpData[atvStartStrLen + ATV_CHANNEL_SKIP_BYTE_OFFSET] &= ~ATV_CHANNEL_IS_SKIP;
			}
			pTmpData[atvStartStrLen + ATV_CHANNEL_POS_HIGH_BYTE_OFFSET] = allChannelVector[i].channelPos / 256;
			pTmpData[atvStartStrLen + ATV_CHANNEL_POS_LOW_BYTE_OFFSET] = allChannelVector[i].channelPos % 256;

			pTmpData[atvStartStrLen + ATV_CHANNEL_NO_BYTE_OFFSET] = allChannelVector[i].atvChannelNo;

			memcpy(&(dataBlockInfo.pDBSaveData[nowSaveDataOffset]), pTmpData, allChannelVector[i].dbChannelItemDataSize);
			nowSaveDataOffset += allChannelVector[i].dbChannelItemDataSize;
			atvByteCount += allChannelVector[i].dbChannelItemDataSize;
			free(pTmpData);

		}
		atvCheckSum = TVCalCheckSum(&(dataBlockInfo.pDBSaveData[atvDataStartOffset]), atvByteCount);
		for (i = 1; i <= ATV_CHECKSUM_BYTE_SIZE; i++)
		{
			dataBlockInfo.pDBSaveData[atvByteCheckSumOffset + ATV_CHECKSUM_BYTE_SIZE - i] = atvCheckSum % 256;
			atvCheckSum /= 256;
		}
		for (i = 1; i <= ATV_CHANNEL_COUNT_BYTE_SIZE; i++)
		{
			dataBlockInfo.pDBSaveData[atvByteCountOffset + ATV_CHANNEL_COUNT_BYTE_SIZE - i] = atvByteCount % 256;
			atvByteCount /= 256;
		}
		//copy between 
		memcpy(&(dataBlockInfo.pDBSaveData[nowSaveDataOffset]), &(dataBlockInfo.pSourceData[dataBlockInfo.atvChannelDataOffset + dataBlockInfo.atvChannelDataSize]), dataBlockInfo.dtvChannelDataOffset - dataBlockInfo.atvChannelDataOffset - dataBlockInfo.atvChannelDataSize);
		nowSaveDataOffset += dataBlockInfo.dtvChannelDataOffset - dataBlockInfo.atvChannelDataOffset - dataBlockInfo.atvChannelDataSize;
		//copy dtv
		sort(allChannelVector.begin(), allChannelVector.end(), SortByDtvIndex);
		unsigned int dtvStartStrLen, dtvEndStrLen, dtvDataStartOffset = 0, dtvByteCountOffset = 0, dtvByteCheckSumOffset = 0, dtvByteCount = 0, dtvCheckSum = 0;
		dtvStartStrLen = strlen(DTV_ITEM_INFO_START_STR);
		dtvEndStrLen = strlen(DTV_ITEM_INFO_END_STR);
		memcpy(&(dataBlockInfo.pDBSaveData[nowSaveDataOffset]), &(dataBlockInfo.pSourceData[dataBlockInfo.dtvChannelDataOffset]), DTV_ALL_DATA_START_BYTE_SIZE);
		nowSaveDataOffset += DTV_ALL_DATA_START_BYTE_SIZE + DTV_CHANNEL_COUNT_BYTE_SIZE + DTV_CHECKSUM_BYTE_SIZE;
		dtvByteCountOffset = nowSaveDataOffset - DTV_CHANNEL_COUNT_BYTE_SIZE - DTV_CHECKSUM_BYTE_SIZE;
		dtvByteCheckSumOffset = nowSaveDataOffset - DTV_CHECKSUM_BYTE_SIZE;
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
					memcpy(&pTmpData[dtvStartStrLen + DTV_CHANNEL_NAME_BYTE_OFFSET], allChannelVector[i].name, DTV_CHANNEL_NAME_BYTE_SIZE);
					if (allChannelVector[i].isLock)
					{
						pTmpData[dtvStartStrLen + DTV_CHANNEL_LOCK_BYTE_OFFSET] |= DTV_CHANNEL_IS_LOCK;
					}
					else
					{
						pTmpData[dtvStartStrLen + DTV_CHANNEL_LOCK_BYTE_OFFSET] &= ~DTV_CHANNEL_IS_LOCK;
					}
					if (allChannelVector[i].isSkip)
					{
						pTmpData[dtvStartStrLen + DTV_CHANNEL_SKIP_BYTE_OFFSET] |= DTV_CHANNEL_IS_SKIP;
					}
					else
					{
						pTmpData[dtvStartStrLen + DTV_CHANNEL_SKIP_BYTE_OFFSET] &= ~DTV_CHANNEL_IS_SKIP;
					}
					pTmpData[dtvStartStrLen + DTV_CHANNEL_POS_HIGH_BYTE_OFFSET] = allChannelVector[i].channelPos / 256;
					pTmpData[dtvStartStrLen + DTV_CHANNEL_POS_LOW_BYTE_OFFSET] = allChannelVector[i].channelPos % 256;
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
		for (i = 1; i <= DTV_CHECKSUM_BYTE_SIZE; i++)
		{
			dataBlockInfo.pDBSaveData[dtvByteCheckSumOffset + DTV_CHECKSUM_BYTE_SIZE - i] = dtvCheckSum % 256;
			dtvCheckSum /= 256;
		}
		dtvByteCount -= 2;//country 只算了一个 所以减去2
		for (i = 1; i <= DTV_CHANNEL_COUNT_BYTE_SIZE; i++)
		{
			dataBlockInfo.pDBSaveData[dtvByteCountOffset + DTV_CHANNEL_COUNT_BYTE_SIZE -i] = dtvByteCount % 256;
			dtvByteCount /= 256;
		}
		//copy checksum
		dataCheckSum = DataCalCheckSum(dataBlockInfo.pDBSaveData, dataBlockInfo.sourceDataLen - deleteChannelSize - DATABASE_CHECKSUM_BYTE_SIZE);
		for (i = 1; i <= DATABASE_CHECKSUM_BYTE_SIZE; i++)
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