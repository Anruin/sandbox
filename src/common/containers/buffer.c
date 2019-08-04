//
// Created by egor on 01.08.2019.
//

#include <common/memory/allocate.h>
#include "buffer.h"

CBuffer *CBuffer_Create(i8 *pData, i32 nLength, i32 nCapacity) {
    CBuffer* pBuffer = ALLOCATE(1, sizeof(CBuffer));

    if (!pBuffer)
        return NULL;

    pBuffer->capacity = nCapacity;
    pBuffer->length = nLength;

    return pBuffer;
}

void CBuffer_Destroy(CBuffer* pBuffer) {
    if (pBuffer == NULL) {
        return;
    }

    if (pBuffer->bHasData) {
        FREE(pBuffer->data);
    }

    FREE(pBuffer);
}