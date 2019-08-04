// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// (c) E. A. Pristavka, 2017
//
//

#include <string.h>
#include <common/debug.h>
#include "application.h"
#include "api/application.h"
#include "api/platform.h"
#include "configuration/configuration.h"

#if defined(_WIN32)
#include <windows.h>
#endif

// Command line arguments

typedef struct ACommandArguments {
    CString pConfigurationPath;
} ACommandArguments;

static ACommandArguments AApplication_CommandArguments = {
        .pConfigurationPath = "Sandbox.ini"
};

static void AApplication_ParseCommandArguments(int argc, char **argv);

// State

static CBool AApplication_IsRunning = 1;

// Time

static f64 AApplication_StartTime;
static f64 AApplication_FrameStartTime;
static f64 AApplication_FrameEndTime;
static f64 AApplication_LastFrameTime;
static f64 AApplication_DeltaTime;

// Modules

typedef struct AModuleInfo {
    AModule arrModules[MODULE_MAX_COUNT];
    u32 nCount;
} AModuleInfo;

static AModuleInfo AApplication_Modules = {0};

static void AApplication_ModuleReadyCallback(void *pModule);

static void AApplication_ModuleUpdateCallback(void *pModule);

static void AApplication_ModuleDestroyCallback(void *pModule);

/// Definitions.

void AApplication_Run(int argc, char *argv[]) {

    AApplication_StartTime = APlatform_GetTime();

    AApplication_ParseCommandArguments(argc, argv);

    AApplication_OnCreate();

    AApplication_OnReady();

    while (AApplication_IsRunning) {
        AApplication_OnUpdate();

        AApplication_Quit(0);
    }

    AApplication_OnDestroy();
}

static void AApplication_ParseCommandArguments(i32 argc, CStringPtr argv) {
    for (i32 i = 1; i < argc; i++) {
        if (i + 1 < argc) {
            if (strcmp(argv[i], "--config") == 0)
                AApplication_CommandArguments.pConfigurationPath = argv[i + 1];
            i++;
        }
    }
}

void AApplication_OnCreate() {

    u32 nProcessorCount = APlatform_GetProcessorCount();
    DEBUG_PRINT("Processor count is %d.\n", nProcessorCount);

    f32 fStartupTime = APlatform_GetTime();
    DEBUG_PRINT("Startup time is %.6f.\n", fStartupTime);

    AModuleNameInfo moduleNameInfo = {0};
    CError nCode = AApplication_LoadConfigurationFromIniFile(AApplication_CommandArguments.pConfigurationPath, &moduleNameInfo);

    assert(nCode == CERROR_OK);

    // Load modules.
    for (u32 i = 0; i < moduleNameInfo.nCount; i++) {
        CString pModuleName = moduleNameInfo.arrModuleNames[i];

        DEBUG_PRINT("Loading module %s.\n", pModuleName);

        nCode = APlatform_LoadModule(pModuleName, &AApplication_Modules.arrModules[i]);

        assert(nCode == CERROR_OK && "There was problem with module loading");

        AApplication_Modules.nCount++;

        AApplication_Modules.arrModules[i].pfnOnCreate();
    }
}

void AApplication_OnReady() {
    for (u32 i = 0; i < AApplication_Modules.nCount; i++) {
        AApplication_Modules.arrModules[i].pfnOnReady();
    }
}

void AApplication_OnUpdate() {

    AApplication_FrameStartTime = APlatform_GetTime();

    for (u32 i = 0; i < AApplication_Modules.nCount; i++)
        AApplication_Modules.arrModules[i].pfnOnUpdate();

    AApplication_DeltaTime = AApplication_FrameStartTime - AApplication_LastFrameTime;
    AApplication_LastFrameTime = AApplication_FrameStartTime - AApplication_StartTime;

    AApplication_FrameEndTime = APlatform_GetTime();

}

void AApplication_OnDestroy() {

    ATaskManager_OnDestroy();

    for (i32 i = AApplication_Modules.nCount - 1; i >= 0; i--) {
        AApplication_Modules.arrModules[i].pfnOnDestroy();
        APlatform_UnloadModule(&AApplication_Modules.arrModules[i]);
    }

    f32 fShutdownTime = APlatform_GetTime();
    DEBUG_PRINT("Shutdown time is %.6f.\n", fShutdownTime);
}

void AApplication_Quit(i32 nCode) {
    DEBUG_PRINT("Exit code: %d", nCode);
    AApplication_IsRunning = 0;
}
