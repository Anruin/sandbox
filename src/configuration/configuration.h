#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common.h"

typedef CError (*AConfiguration_IniFileLineParseCallback)(void *pUserData, CStringConst pSection, CStringConst pKey, CStringConst pValue, i32 nLine);

CError AConfiguration_LoadIniFile(CStringConst pFileName, AConfiguration_IniFileLineParseCallback pfnCallback, void *pUserData);

#ifdef __cplusplus 
}
#endif