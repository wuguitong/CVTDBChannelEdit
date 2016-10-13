#include "stdafx.h"
typedef enum
{
	ISO_8859_01,
	ISO_8859_02,
	ISO_8859_03,
	ISO_8859_04,
	ISO_8859_05,
	ISO_8859_06,
	ISO_8859_07,
	ISO_8859_08,
	ISO_8859_09,
	ISO_8859_10,
	ISO_8859_11,
	ISO_8859_13,
	ISO_8859_14,
	ISO_8859_15,
	UNSUPPORT_CHAR_TABLE = 0xFF
} EN_CHARACTER_CODE_TABLE;
BOOL MApp_TranslateUnicodeToCharTable(BYTE *ChannelNameBuffer, WORD *NameArray, BYTE BufferMaxLen);
void MApp_TranslateCharTableToUnicode(BYTE *ChannelNameBuffer, WORD *NameArray, BYTE BufferMaxLen);