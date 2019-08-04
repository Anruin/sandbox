//
// Copyright (c) E. A. Pristavka, 2017
//

#pragma once

#include "common/common.h"

// Wrapper object.
typedef struct CObject {
    void *pData;
	i32 size;
	i32 id;
} CObject;

// Creates wrapper object.
extern CObject *CObject_New(void *pSource, i32 size);

// Retrieves data from wrapper object.
extern CError CObject_Get(CObject *pObject, void **ppDestination);

// Destroys wrapper object with its data.
extern void CObject_Delete(CObject *pObject);
