//
// Copyright (c) E. A. Pristavka, 2017
//

#pragma once

#include "common/common.h"

#ifdef __cplusplus
extern "C" { // Do not allow C++ compiler to mangle API functions
#endif

LOCAL_SYMBOL void AApplication_OnCreate();

LOCAL_SYMBOL void AApplication_OnReady();

LOCAL_SYMBOL void AApplication_OnUpdate();

LOCAL_SYMBOL void AApplication_OnDestroy();

LOCAL_SYMBOL void AApplication_Run(int argc, char *argv[]);

LOCAL_SYMBOL void AApplication_Quit(i32 nCode);

LOCAL_SYMBOL u64 AApplication_GetDeltaTime();

LOCAL_SYMBOL u64 AApplication_GetTotalTime();

#ifdef __cplusplus
}; // extern "C"
#endif
