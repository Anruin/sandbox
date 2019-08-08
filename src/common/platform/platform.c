// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// Created by Egor Pristavka on 13.09.17.
//

#include "common/common.h"
#include "api/platform.h"
#include "common/debug.h"
#include <assert.h>

#include <SDL_loadso.h>
#include <SDL_timer.h>
#include <SDL_cpuinfo.h>

#ifdef CPLATFORM_WINDOWS
#include <windows.h>
#include <process.h>
#else

#include <dlfcn.h>
#include <unistd.h>
#include <string.h>

#define MODULE_PREFIX_LENGTH 2 // (strlen("./"))
#define MODULE_SUFFIX_LENGTH 8 // (strlen("lib.so") + 1)
#define MODULE_PATH_MAX_LENGTH 1024
#define MODULE_NAME_MAX_LENGTH 1024
#endif

CError APlatform_LoadModule(CStringConst pszModuleName, AModule *pModule) {

    assert(pModule != NULL && "Module is NULL");

    // Load library
    ADebug_PrintCurrentDirectory();

#ifdef CPLATFORM_WINDOWS
    pModule->hLibrary = SDL_LoadObject(pszModuleName);
#else
    char pszModulePath[MODULE_PATH_MAX_LENGTH] = {0};
    size_t nLength = MODULE_PREFIX_LENGTH + strlen(pszModuleName) + MODULE_SUFFIX_LENGTH;

    assert(nLength <= MODULE_NAME_MAX_LENGTH && "Module name is too long");

    snprintf(pszModulePath, nLength, "./lib%s.so", pszModuleName);
    pModule->hLibrary = SDL_LoadObject(pszModulePath);
#endif

    if (pModule->hLibrary == NULL) {
        DEBUG_PRINT("SDL_GetError: %s\n", SDL_GetError());
        return CERROR_FAIL;
    }

    // Load symbols

    // Module name
    CStringConst pszModuleNameSym = SYMBOL_TO_CSTRING(AModule_Name);
    pModule->pszName = *(CStringPtr)SDL_LoadFunction(pModule->hLibrary, pszModuleNameSym);
    if (pModule->pszName == NULL) {
        DEBUG_PRINT("Unable to find symbol %s in loaded module!\n", pszModuleNameSym);
        APlatform_UnloadModule(pModule);
        return CERROR_FAIL;
    }

    // OnCreate callback
    CStringConst pszModuleOnCreateSym = SYMBOL_TO_CSTRING(AModule_OnCreate);
    pModule->pfnOnCreate = (void (*)())SDL_LoadFunction(pModule->hLibrary, pszModuleOnCreateSym);
    if (pModule->pfnOnCreate == NULL) {
        DEBUG_PRINT("Unable to find symbol %s in loaded module!\n", pszModuleOnCreateSym);
        APlatform_UnloadModule(pModule);
        return CERROR_FAIL;
    }

    // OnReady callback
    CStringConst pszModuleOnReadySym = SYMBOL_TO_CSTRING(AModule_OnReady);
    pModule->pfnOnReady = (void (*)())SDL_LoadFunction(pModule->hLibrary, pszModuleOnReadySym);
    if (pModule->pfnOnReady == NULL) {
        DEBUG_PRINT("Unable to find symbol %s in loaded module!\n", pszModuleOnReadySym);
        APlatform_UnloadModule(pModule);
        return CERROR_FAIL;
    }

    // OnUpdate callback
    CStringConst pszModuleOnUpdateSym = SYMBOL_TO_CSTRING(AModule_OnUpdate);
    pModule->pfnOnUpdate = (void (*)())SDL_LoadFunction(pModule->hLibrary, pszModuleOnUpdateSym);
    if (pModule->pfnOnUpdate == NULL) {
        DEBUG_PRINT("Unable to find symbol %s in loaded module!\n", pszModuleOnUpdateSym);
        APlatform_UnloadModule(pModule);
        return CERROR_FAIL;
    }

    // OnDestroy callback
    CStringConst pszModuleOnDestroySym = SYMBOL_TO_CSTRING(AModule_OnDestroy);
    pModule->pfnOnDestroy = (void (*)())SDL_LoadFunction(pModule->hLibrary, pszModuleOnDestroySym);
    if (pModule->pfnOnDestroy == NULL) {
        DEBUG_PRINT("Unable to find symbol %s in loaded module!\n", SYMBOL_TO_CSTRING(AModule_OnDestroy));
        APlatform_UnloadModule(pModule);
        return CERROR_FAIL;
    }

    return CERROR_OK;
}

void APlatform_UnloadModule(AModule *pModule) {
    if (pModule->hLibrary) {
        SDL_UnloadObject(pModule->hLibrary);
    }
}

f64 APlatform_GetTime() {
    return SDL_GetPerformanceCounter() * 1000.0 / SDL_GetPerformanceFrequency();
}

u32 APlatform_GetProcessorCount() {
    return (u32)SDL_GetCPUCount();
}

u32 APlatform_GetSystemRAM() {
    return (u32)SDL_GetSystemRAM();
}
