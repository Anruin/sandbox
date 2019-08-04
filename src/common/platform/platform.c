// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// Created by Egor Pristavka on 13.09.17.
//

#include "common/common.h"
#include "api/platform.h"
#include "common/task.h"
#include "thread.h"
#include "common/debug.h"
#include <assert.h>

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
    pModule->hLibrary = LoadLibraryA(pszModuleName);
#else
    char pszModulePath[MODULE_PATH_MAX_LENGTH] = {0};
    size_t nLength = MODULE_PREFIX_LENGTH + strlen(pszModuleName) + MODULE_SUFFIX_LENGTH;

    assert(nLength <= MODULE_NAME_MAX_LENGTH && "Module name is too long");

    snprintf(pszModulePath, nLength, "./lib%s.so", pszModuleName);
    pModule->hLibrary = dlopen(pszModulePath, RTLD_NOW);
#endif

    if (pModule->hLibrary == NULL) {
#ifdef CPLATFORM_WINDOWS
        DEBUG_PRINT("GetLastError() code: %i\n", (int)GetLastError());
#else
        DEBUG_PRINT("dlerror: %s\n", dlerror());
#endif
        return CERROR_FAIL;
    }

    // Load symbols

#ifdef CPLATFORM_WINDOWS
    pModule->pszName = *(CString *)GetProcAddress(pModule->hLibrary, SYMBOL_TO_PSZ(AModule_Name));
#else
    pModule->pszName = *(CString *) dlsym(pModule->hLibrary, SYMBOL_TO_PSZ(AModule_Name));
#endif

    if (pModule->pszName == NULL) {
        DEBUG_PRINT("Unable to find symbol %s in loaded module!\n", SYMBOL_TO_PSZ(AModule_Name));
        APlatform_UnloadModule(pModule);
        return CERROR_FAIL;
    }

#ifdef CPLATFORM_WINDOWS
    pModule->pfnOnCreate = GetProcAddress(pModule->hLibrary, SYMBOL_TO_PSZ(AModule_OnCreate));
#else
    pModule->pfnOnCreate = dlsym(pModule->hLibrary, SYMBOL_TO_PSZ(AModule_OnCreate));
#endif

    if (pModule->pfnOnCreate == NULL) {
        DEBUG_PRINT("Unable to find symbol %s in loaded module!\n", SYMBOL_TO_PSZ(AModule_OnCreate));
        APlatform_UnloadModule(pModule);
        return CERROR_FAIL;
    }


#ifdef CPLATFORM_WINDOWS
    pModule->pfnOnReady = GetProcAddress(pModule->hLibrary, SYMBOL_TO_PSZ(AModule_OnReady));
#else
    pModule->pfnOnReady = dlsym(pModule->hLibrary, SYMBOL_TO_PSZ(AModule_OnReady));
#endif

    if (pModule->pfnOnReady == NULL) {
        DEBUG_PRINT("Unable to find symbol %s in loaded module!\n", SYMBOL_TO_PSZ(AModule_OnReady));
        APlatform_UnloadModule(pModule);
        return CERROR_FAIL;
    }

#ifdef CPLATFORM_WINDOWS
    pModule->pfnOnUpdate = GetProcAddress(pModule->hLibrary, SYMBOL_TO_PSZ(AModule_OnUpdate));
#else
    pModule->pfnOnUpdate = dlsym(pModule->hLibrary, SYMBOL_TO_PSZ(AModule_OnUpdate));
#endif

    if (pModule->pfnOnUpdate == NULL) {
        DEBUG_PRINT("Unable to find symbol %s in loaded module!\n", SYMBOL_TO_PSZ(AModule_OnUpdate));
        APlatform_UnloadModule(pModule);
        return CERROR_FAIL;
    }

#ifdef CPLATFORM_WINDOWS
    pModule->pfnOnDestroy = GetProcAddress(pModule->hLibrary, SYMBOL_TO_PSZ(AModule_OnDestroy));
#else
    pModule->pfnOnDestroy = dlsym(pModule->hLibrary, SYMBOL_TO_PSZ(AModule_OnDestroy));
#endif

    if (pModule->pfnOnDestroy == NULL) {
        DEBUG_PRINT("Unable to find symbol %s in loaded module!\n", SYMBOL_TO_PSZ(AModule_OnDestroy));
        APlatform_UnloadModule(pModule);
        return CERROR_FAIL;
    }

    return CERROR_OK;
}

void APlatform_UnloadModule(AModule *pModule) {
    if (pModule->hLibrary) {
#ifdef CPLATFORM_WINDOWS
        i32 code = FreeLibrary(pModule->hLibrary);
        assert(code == TRUE);
#else
        i32 code = dlclose(pModule->hLibrary);
        assert(code == 0);
#endif
    }
}

f64 APlatform_GetTime() {
#ifdef CPLATFORM_WINDOWS
    static LARGE_INTEGER nFrequency;
    if (frequency.QuadPart == 0)
        ::QueryPerformanceFrequency(&nFrequency);
    LARGE_INTEGER nNow;
        ::QueryPerformanceCounter(&nNow);
    return nNow.QuadPart / f64(nFrequency.QuadPart);
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec / 1000000000.0;
#endif
}

u32 APlatform_GetProcessorCount() {
#ifdef CPLATFORM_WINDOWS
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    return (u32) systemInfo.dwNumberOfProcessors;
#else
    return (u32) sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

