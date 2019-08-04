/**
* @file vector.h
* @ingroup Common
* @brief Sandbox dynamic array.
* @author Egor Pristavka
* @copyright Egor Pristavka 2017
*/

#pragma once

#include <stddef.h>
#include "common/common.h"
#include "common/generic/object.h"

/**
 * @struct AVector
 * @brief Dynamic container.
 * @property AObject **ppItems Inner data memory, containing AObject.
 * @property ADestructorFunc pfnDestructor Function used to free objects, by default just frees memory used by pointer.
 * @property nCount Actual count of elements in the container.
 * @property nCapacity Maximum available count of elements in the container.
 * @remark Using 4 byte integers for size to save up some memory.
 * @remark When requesting additional capacity from OS it shouldn't allocate it right away, and will actually allocate
 * 		   it when memory will be written.
 */
typedef struct AVector {
	CObject **ppItems;
	CDestructorFunc pfnDestructor;
	i32 nCount;
	i32 nCapacity;
} CVector;

// Create new dynamic array.
CVector *CVector_Create(i32 capacity, CDestructorFunc pfnDestructor);

// Sets value in array by index. Be careful with index.
CError CVector_Set(CVector *pVector, i32 index, void *pSource, i32 nSize);

// Inserts object into array at specified index.
CError CVector_Insert(CVector *pVector, i32 index, void *pSource, i32 nSize);

// Appends object to the end of array.
CError CVector_PushBack(CVector *pVector, void *pSource, i32 nSize);

// Retrieves object from the array by its index. Be careful about retrieved object type cast.
CError CVector_Get(CVector *pVector, i32 index, void **ppDestination);

// Retrieves and removes object from end of the array.
CError CVector_PopBack(CVector *pVector, void **ppDestination);

// Retrieves and removes object from beginning of the array.
CError CVector_PopFront(CVector *pVector, void **ppDestination);

// Checks if array is empty.
CBool CVector_Empty(CVector *pVector);

// Returns count of items in array.
i32 CVector_Count(CVector *pVector);

// Returns capacity of the array.
i32 CVector_Capacity(CVector *pVector);

// Remove item from array at specified index.
CError CVector_Remove(CVector *pVector, i32 index);

// Frees and removes all elements from vector.
CError CVector_Clear(CVector *pVector);

// Destroys array.
CError CVector_Destroy(CVector *pVector);

// For each loop implementation.
CError CVector_Each(CVector *pVector, CCallbackFunc pfnCallback);

// Reverse for each loop implementation.
CError CVector_ReverseEach(CVector *pVector, CCallbackFunc pfnCallback);

void CVector_DefaultDestructor(void *p);

#define CVector_DefaultCapacity 8

#define CVector_NewDefault() CVector_New(CVector_DefaultCapacity, CVector_DefaultDestructor);

/**
 * @def AVector_PushBackCString(pVector, pSource)
 * @brief Append string to the array.
 * @remarks Does not duplicate string.
 * -V:AVector_PushBackCString:202
 */
#define CVector_PushBackCString(pVector, pSource) { CVector_PushBack(pVector, pSource, (i32)strlen(pSource) + 1); }
/**
 * @def AVector_PushBackPrimitive(pVector, type, value)
 * @brief Push value of primitive type to array.
 * -V:AVector_PushBackCString:202
 */
#define CVector_PushBackPrimitive(pVector, type, value) { type v = value; CVector_PushBack(pVector, &v, (i32)sizeof(type)); }
/**
 * @def AVector_GetPrimitive(pVector, nIndex, type, value)
 * @brief Get value of primitive type.
 * -V:AVector_GetPrimitive:206
 */
#define CVector_GetPrimitive(pVector, nIndex, type, value) { void *__p; CVector_Get(pVector, nIndex, &__p); (value) = *(type *)__p; FREE(__p); }
/**
 * @def AVector_GetPointer(pVector, nIndex, p)
 * @brief Get value of pointer type.
 * @remarks You should not add indirection level to pointer as with AVector_Get function.
 */
#define CVector_GetPointer(pVector, nIndex, p) { CVector_Get(pVector, nIndex, (void **)&(p)); }
