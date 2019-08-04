#pragma once

#include "api/module.h"
#include "common/common.h"
#include "common/string/string.h"
#include "common/containers/vector.h"
#include "common/task.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Modules

LOCAL_SYMBOL CError APlatform_LoadModule(CStringConst pName, AModule *pModule);

LOCAL_SYMBOL void APlatform_UnloadModule(AModule *pModule);

LOCAL_SYMBOL f64 APlatform_GetTime();

LOCAL_SYMBOL u32 APlatform_GetProcessorCount();

// Platform

// Threading

// LOCAL_SYMBOL extern void APlatform_StartTaskWorkerThreads();
// Schedule system work. Should be called from main thread.
// LOCAL_SYMBOL extern void APlatform_ScheduleTasks(CVector *pTasks, float fDeltaTime);
// Add a task.
// LOCAL_SYMBOL extern void APlatform_AddTask(ATask *pTask);
// Wait for all tasks to finish.
// LOCAL_SYMBOL extern void APlatform_WaitForAllTasks();

#ifdef __cplusplus
}
#endif // __cplusplus
