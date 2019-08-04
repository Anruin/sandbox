//
// Created by egor on 8/10/18.
//

#include <unistd.h>
#include <stdio.h>
#include <limits.h>

void ADebug_PrintCurrentDirectory() {
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		fprintf(stdout, "Current working dir: %s\n", cwd);
	} else {
		fprintf(stderr, "getcwd() error");
	}
}
