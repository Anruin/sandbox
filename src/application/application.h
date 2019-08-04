//
// Created by egor on 8/10/18.
//

#ifndef SANDBOX_APPLICATION_H
#define SANDBOX_APPLICATION_H

#include <common/common.h>
#include <common/string/string.h>

// Definitions

#define MODULE_MAX_COUNT 8
#define MODULE_MAX_NAME_LENGTH 64
#define MODULE_INI_SECTION "Modules"

// Configuration module name data.
typedef struct AModuleNameInfo {
	char arrModuleNames[MODULE_MAX_COUNT][MODULE_MAX_NAME_LENGTH];
	u32 nCount;
} AModuleNameInfo;

CError AApplication_LoadConfigurationFromIniFile(CStringConst pPath, AModuleNameInfo *pModuleNameInfo);

#endif //SANDBOX_APPLICATION_H
