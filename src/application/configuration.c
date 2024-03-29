// Configuration

#include <common/errors.h>
#include <common/string/string.h>
#include <memory.h>
#include <configuration/configuration.h>
#include <SDL_log.h>
#include "application.h"

static CError AApplication_ConfigurationLineParseCallback(void *pUserData, CStringConst pSection, CStringConst pKey, CStringConst pValue, i32 nLine) {

	if (strcmp(pSection, MODULE_INI_SECTION) == 0) {
		AModuleNameInfo *pModuleInfo = (AModuleNameInfo *) pUserData;

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Adding module [%s] to loading queue, loading it from %s, ini line %d.\n", pKey, pValue, nLine);

		i32 nIndex = pModuleInfo->nCount++;

		assert(nIndex < MODULE_MAX_COUNT && "Consider increasing MODULE_MAX_COUNT");

		// Need to copy string from file to process memory instead of using file mapped pointer.
		memcpy(pModuleInfo->arrModuleNames[nIndex], pValue, MODULE_MAX_NAME_LENGTH);
	} else if (strcmp(pSection, "Nodes") == 0) {

		// Collect configuration for modules
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s\n", (char*)pUserData);

		return CERROR_OK;
	}

	return CERROR_OK;
}

CError AApplication_LoadConfigurationFromIniFile(CStringConst pPath, AModuleNameInfo *pModuleNameInfo) {

	if (AConfiguration_LoadIniFile(pPath, AApplication_ConfigurationLineParseCallback, (void *) pModuleNameInfo) < 0) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Error loading configuration from %s.\n", pPath);
		return CERROR_FAIL;
	}

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Configuration loaded from %s.\n", pPath);

	return CERROR_OK;
}
