// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "common/string/string.h"
#include "configuration.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define COMMENT_PREFIX ";"
#define MAX_LINE 1024
#define MAX_SECTION 255
#define MAX_KEY 255

// Declarations

typedef CString(*AConfiguration_IniFileReader)(CString pString, i32 nNumber, void *pStream);

static i32 AConfiguration_ParseFileStream(FILE *pFile, AConfiguration_IniFileLineParseCallback pfnCallback, void *pUserData);

static i32 AConfiguration_ParseReaderStream(AConfiguration_IniFileReader pfnReader, void *pStream, AConfiguration_IniFileLineParseCallback pfnCallback, void *pUserData);

static CString AConfiguration_FindCharsOrComment(CString pString, CStringConst pChars);

// Definitions

CError AConfiguration_LoadIniFile(CStringConst pFileName, AConfiguration_IniFileLineParseCallback pfnCallback, void *pUserData) {
    FILE *pFile;
    i32 nErrorLine;

    pFile = fopen(pFileName, "r");

    if (!pFile)
        return -1;

    nErrorLine = AConfiguration_ParseFileStream(pFile, pfnCallback, pUserData);

    fclose(pFile);

    assert(nErrorLine == 0);

    return nErrorLine > 0;
}

static i32 AConfiguration_ParseFileStream(FILE *pFile, AConfiguration_IniFileLineParseCallback pfnCallback, void *pUserData) {
    return AConfiguration_ParseReaderStream((AConfiguration_IniFileReader) fgets, pFile, pfnCallback, pUserData);
}

static i32 AConfiguration_ParseReaderStream(AConfiguration_IniFileReader pfnReader, void *pStream, AConfiguration_IniFileLineParseCallback pfnCallback, void *pUserData) {
    char arrLine[MAX_LINE];
    char arrSection[MAX_SECTION] = "";
    char arrPreviousKey[MAX_KEY] = "";

    CString pStart;
    CString pEnd;
    CString pKey;
    CString pValue;
    i32 nLine = 0;
    i32 nError = 0;

    while (pfnReader(arrLine, MAX_LINE, pStream) != NULL) {
        nLine++;

        pStart = arrLine;

        // Skip BOM
        if (nLine == 1 && (u8) pStart[0] == 0xEF &&
            (u8) pStart[1] == 0xBB &&
            (u8) pStart[2] == 0xBF) {
            pStart += 3;
        }

        pStart = CString_SkipLeft(CString_StripRight(pStart));

        if (*pStart == ';' || *pStart == '#' || *pStart == '\0') {
            // Comment or empty line
        } else if (*arrPreviousKey && *pStart && pStart > arrLine) {
            // Previous line or value continuation.
            if (!pfnCallback(pUserData, arrSection, arrPreviousKey, pStart, nLine) && !nError)
                nError = nLine;
        } else if (*pStart == '[') {
            // Section line
            pEnd = AConfiguration_FindCharsOrComment(pStart + 1, "]");

            if (*pEnd == ']') {
                *pEnd = '\0';
                CString_CopyZero(arrSection, pStart + 1, sizeof(arrSection));
                *arrPreviousKey = '\0';
            } else if (!nError) {
                // No ']' found.
                nError = nLine;
            }
        } else if (*pStart) {
            // Not a comment nor section
            pEnd = AConfiguration_FindCharsOrComment(pStart, "=:");
            if (*pEnd == '=' || *pEnd == ':') {
                *pEnd = '\0';

                pKey = CString_StripRight(pStart);
                pValue = pEnd + 1;

                // Check for inline comments
                pEnd = AConfiguration_FindCharsOrComment(pValue, NULL);
                if (*pEnd)
                    *pEnd = '\0';

                pValue = CString_StripRight(CString_SkipLeft(pValue));

                CString_CopyZero(arrPreviousKey, pKey, sizeof(arrPreviousKey));

                if (CERROR_OK != pfnCallback(pUserData, arrSection, pKey, pValue, nLine) && !nError)
                    nError = nLine;
            }
        } else if (!nError) {
            // No "=:" found on key-value line.
            nError = nLine;
        }
    }

    return nError;
}

static CString AConfiguration_FindCharsOrComment(CString pString, CStringConst pChars) {
    i32 nWasSpace = 0;

    while (*pString
           && (!pChars || !strchr(pChars, *pString))
           && !(nWasSpace && strchr(COMMENT_PREFIX, *pString))) {
        nWasSpace = isspace((u8) (*pString));
        pString++;
    }

    return pString;
}
