// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// Copyright (c) E. A. Pristavka, 2017
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <common/memory/allocate.h>
#include "vector.h"

#define VECTOR_INITIAL_CAPACITY 8

static CVector *CVector_CheckAndGrow(CVector *pVector) {
    if (!pVector)
        return NULL;

    if (pVector->nCount >= pVector->nCapacity) {
        pVector->nCapacity *= 2;

        CObject **ppElements = REALLOCATE(pVector->ppItems, SIZE(pVector->nCapacity) * sizeof(CObject *));

        if (!ppElements)
            return NULL;

        pVector->ppItems = ppElements;
    }

    return pVector;
}

CVector *CVector_Create(int32_t capacity, CDestructorFunc pfnDestructor) {
    CVector *pVector = ALLOCATE(1, sizeof(CVector));

    if (!pVector)
        return NULL;

    pVector->nCapacity = capacity < VECTOR_INITIAL_CAPACITY ? VECTOR_INITIAL_CAPACITY : capacity;
    pVector->ppItems = ALLOCATE(SIZE(pVector->nCapacity), sizeof(CObject *));

    if (!pVector->ppItems) {
        FREE(pVector);
        return NULL;
    }

    pVector->pfnDestructor = pfnDestructor;
    pVector->nCount = 0;

    return pVector;
}

CError CVector_Set(CVector *pVector, int32_t index, void *pSource, int32_t nSize) {
    CObject *pObject = CObject_New(pSource, nSize);

    if (!pObject)
        return CERROR_FAIL;

    pVector->ppItems[SIZE(index)] = pObject;
    pVector->nCount++;

    return CERROR_OK;
}

CError CVector_PushBack(CVector *pVector, void *pSource, int32_t nSize) {
    if (!pVector)
        return CERROR_FAIL;

    CVector_CheckAndGrow(pVector);

    CError code = CVector_Set(pVector, pVector->nCount, pSource, nSize);

    return code;
}

CError CVector_PopBack(CVector *pVector, void **ppDestination) {
    CError code;
    code = CVector_Get(pVector, pVector->nCount, ppDestination);

    if (code != CERROR_OK)
        return code;

    code = CVector_Remove(pVector, pVector->nCount);

    return code;
}

CError CVector_PopFront(CVector *pVector, void **ppDestination) {
    CError code;
    code = CVector_Get(pVector, 0, ppDestination);

    if (code != CERROR_OK)
        return code;

    code = CVector_Remove(pVector, 0);

    return code;
}

CError CVector_Get(CVector *pVector, int32_t index, void **ppDestination) {
    if (!pVector)
        return CERROR_FAIL;

    if (index >= pVector->nCount)
        return CERROR_FAIL;

    CObject_Get(pVector->ppItems[SIZE(index)], ppDestination);

    return CERROR_OK;
}

int32_t CVector_Count(CVector *pVector) {
    if (!pVector)
        return 0;

    return pVector->nCount;
}

int32_t CVector_Capacity(CVector *pVector) {
    if (!pVector)
        return 0;

    return pVector->nCapacity;
}

CBool CVector_Empty(CVector *pVector) {
    if (!pVector)
        return 0;

    return pVector->nCount == 0;
}

CError CVector_Insert(CVector *pVector, int32_t index, void *pSource, int32_t nSize) {
    if (!pVector)
        return CERROR_FAIL;

    if (CVector_CheckAndGrow(pVector) == NULL)
        return CERROR_FAIL;

    MEMMOVE(&pVector->ppItems[SIZE(index) + 1], &pVector->ppItems[SIZE(index)], SIZE((pVector->nCount - index)) * sizeof(CObject *));

    CError code = CVector_Set(pVector, index, pSource, nSize);

    return code;
}

CError CVector_Remove(CVector *pVector, int32_t index) {
    if (!pVector)
        return CERROR_FAIL;

	int32_t count = pVector->nCount;

    if (index >= count)
        return CERROR_FAIL;

    if (pVector->pfnDestructor) {
        void *p;

        CError code = CVector_Get(pVector, index, &p);

        if (code == CERROR_OK)
            pVector->pfnDestructor(p);
        else
            return code;
    }

    CObject_Delete(pVector->ppItems[SIZE(index)]);

    if (index != count - 1)
        if (NULL == MEMMOVE(&pVector->ppItems[SIZE(index)],
                            &pVector->ppItems[SIZE(index + 1)],
                            SIZE((count - index)) * sizeof(CObject *)))
            return CERROR_FAIL;

    pVector->nCount--;

    return CERROR_OK;
}

CError CVector_Destroy(CVector *pVector) {
    if (!pVector)
        return CERROR_FAIL;

	int32_t i = 0;
	int32_t count = pVector->nCount;

    if (pVector->pfnDestructor)
        for (i = 0; i < count; i++) {
            void *p;

            CError code = CVector_Get(pVector, i, &p);

            if (code == CERROR_OK)
                pVector->pfnDestructor(p);
            else
                return code;
        }

    for (i = 0; i < count; i++)
        CObject_Delete(pVector->ppItems[SIZE(i)]);

    FREE(pVector->ppItems);
    FREE(pVector);

    return CERROR_OK;
}

CError CVector_Each(CVector *pVector, CCallbackFunc pfnCallback) {
    if (!pVector)
        return CERROR_FAIL;

	int32_t count = pVector->nCount;

    void *p = NULL;

    for (int32_t i = 0; i < count; i++) {
        CError code = CVector_Get(pVector, i, &p);

        if (code != CERROR_OK)
            return code;

        pfnCallback(p);
        FREE(p);
    }

    return CERROR_OK;
}

CError CVector_ReverseEach(CVector *pVector, CCallbackFunc pfnCallback) {
    if (!pVector)
        return CERROR_FAIL;

    int32_t count = pVector->nCount;

    void *p = NULL;

    for (int32_t i = count - 1; i >= 0; i--) {
        CError code = CVector_Get(pVector, i, &p);

        if (code != CERROR_OK)
            return code;

        pfnCallback(p);
        //DEALLOCATE(pUserData);
    }

    return CERROR_OK;
}

void CVector_DefaultDestructor(void *p) {
    FREE(p);
}

CError CVector_Clear(CVector *pVector) {
    if (!pVector)
        return CERROR_FAIL;

    int32_t i;
    int32_t count = pVector->nCount;

    if (pVector->pfnDestructor)
        for (i = 0; i < count; i++) {
            void *p;

            CError code = CVector_Get(pVector, i, &p);

            if (code == CERROR_OK)
                pVector->pfnDestructor(p);
            else
                return code;
        }

    for (i = 0; i < count; i++)
        CObject_Delete(pVector->ppItems[SIZE(i)]);

    return 0;
}
