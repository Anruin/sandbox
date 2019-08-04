// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// Copyright (c) E. A. Pristavka, 2017
//

#include <api/application.h>
#include "api/module.h"
#include "common/common.h"

CStringConst AModule_Name = "Example";

void AModule_OnCreate() {
    DEBUG_PRINT("Example module created!\n");
}

void AModule_OnReady() {
    DEBUG_PRINT("Example module ready!\n");
}

void AModule_OnUpdate() {
}

void AModule_OnDestroy() {
    DEBUG_PRINT("Example module unloaded!\n");
}

typedef struct MExample_TaskData {
    int nFirst;
    int nSecond;
    int *pResult;
} MExample_TaskData;

void MExample_Task(void *pTaskData) {
    MExample_TaskData *pData = (MExample_TaskData *) pTaskData;

    *pData->pResult = pData->nFirst + pData->nSecond;
}