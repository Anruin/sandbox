// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "string.h"
#include <ctype.h>
#include <string.h>
#include <common/memory/allocate.h>

AString *AString_New(CStringConst pSource) {
	AString *pString = ALLOCATE(1, sizeof(AString));

	if (!pString)
		return NULL;

	pString->nLength = (int32_t)strlen(pSource);
	pString->pString = ALLOCATE(1, ASTRING_LENGTH(pString));

	if (!pString->pString) {
		FREE(pString);
		return NULL;
	}

    MEMCOPY(pString->pString, pSource, ASTRING_LENGTH(pString));

	return pString;
}

AString *AString_Append(AString *pDestination, CStringConst pSource) {
	size_t nSourceLength = SIZE(strlen(pSource));
	size_t nNewSize = ASTRING_LENGTH(pDestination) + nSourceLength;
	CString pNewString = REALLOCATE(pDestination->pString, nNewSize);

	if (!pNewString)
		return NULL;

	pDestination->pString = pNewString;

    MEMCOPY(pNewString + SIZE(pDestination->nLength), pSource, SIZE(nSourceLength) + 1);

	return pDestination;
}

CString CString_CopyZero(CString pszDestination, CStringConst pszSource, size_t nCharsLeft) {
	strncpy(pszDestination, pszSource, nCharsLeft);
	pszDestination[nCharsLeft - 1] = '\0';
	return pszDestination;
}

CString CString_StripRight(CString pszString) {
	CString p = pszString + SIZE(strlen(pszString));

	while (p > pszString && isspace((unsigned char)(*--p)))
		*p = '\0';

	return pszString;
}

CString CString_SkipLeft(CString pszString) {
	while (*pszString && isspace((unsigned char)(*pszString)))
		pszString++;
	return pszString;
}

void AString_Delete(AString *pString) {
	if (pString) {
		FREE(pString->pString);
		FREE(pString);
	}
}