//
// Created by egor on 8/10/18.
//

#include <SDL_filesystem.h>
#include <stdio.h>
#include <limits.h>
#include <common/string/string.h>

void ADebug_PrintCurrentDirectory() {
	CStringConst pszPath = SDL_GetBasePath();

    fprintf(stdout, "Current working dir: %s\n", pszPath);
}
