// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Copyright (c) E. A. Pristavka, 2017
//

#include "object.h"
#include <memory.h>
#include <common/memory/allocate.h>

static int32_t AObject_LastId = 0;

CObject *CObject_New(void *pSource, int32_t size) {
    CObject *pObject = ALLOCATE(1, sizeof(CObject));

    if (!pObject)
        return NULL;

    pObject->size = size;
    pObject->pData = ALLOCATE(1, SIZE(size));
	pObject->id = AObject_LastId++;

    if (!pObject->pData) {
        FREE(pObject);
        return NULL;
    }

    MEMCOPY(pObject->pData, pSource, SIZE(size));

    return pObject;
}

CError CObject_Get(CObject *pObject, void **ppDestination) {
    *ppDestination = ALLOCATE(1, SIZE(pObject->size));

    if (!*ppDestination)
        return CERROR_FAIL;

    MEMCOPY(*ppDestination, pObject->pData, SIZE(pObject->size));

    return CERROR_OK;
}

void CObject_Delete(CObject *pObject) {
    if (pObject) {
        FREE(pObject->pData);
        FREE(pObject);
    }
}
