// Sandbox Module API Declarations
// (c) Egor Pristavka 2017-2018

#pragma once

#include <common/common.h>
#include <common/string/string.h>
#include <common/containers/vector.h>

#ifdef __cplusplus
extern "C" { // Do not allow C++ compiler to mangle API functions
#endif

// All the public symbols must be defined in the module.

PUBLIC_SYMBOL CStringConst AModule_Name;
PUBLIC_SYMBOL u32 AModule_Version;

// Called by the application during module load stage.
PUBLIC_SYMBOL void AModule_OnCreate();

// Called by the application after all modules were loaded.
PUBLIC_SYMBOL void AModule_OnReady();

// Called by the application during its update.
PUBLIC_SYMBOL void AModule_OnUpdate();

// Called by the application when it is unloading module.
PUBLIC_SYMBOL void AModule_OnDestroy();

// Module definition.
typedef struct AModule {

    // Module function library handle.
    void *hLibrary;

    // Module name literal loaded from DLL.
    CString pszName;
	
    // API callback functions.
    void (*pfnOnCreate)();
    void (*pfnOnReady)();
    void (*pfnOnUpdate)();
    void (*pfnOnDestroy)();

} AModule;

#ifdef __cplusplus
}; // extern "C"
#endif
