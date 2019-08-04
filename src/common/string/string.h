#pragma once

#include "common/common.h"

// C string
typedef char *CString;
typedef char **CStringPtr;
// Const C string
typedef const char *CStringConst;

// String.
typedef struct AString {
    CString pString;
    int32_t nLength;
} AString;

// Return size of a string including zero terminator.
//-V:ASTRING_LENGTH:201
#define ASTRING_LENGTH(s) ((size_t)(s)->nLength + 1)

// Creates wrapper object.
extern AString *AString_New(CStringConst pSource);

// Destroys wrapper object with its data.
extern void AString_Delete(AString *pString);

extern AString *AString_Append(AString *pDestination, CStringConst pSource);

extern CString CString_CopyZero(CString pszDestination, CStringConst pszSource, size_t nSize);

extern CString CString_StripRight(CString pszString);

extern CString CString_SkipLeft(CString pszString);