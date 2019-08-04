//
// Created by egor on 01.08.2019.
//

#ifndef SANDBOX_ALLOCATE_H
#define SANDBOX_ALLOCATE_H

#define ALLOCATE(length, size) calloc(length, size)
#define REALLOCATE(p, size) realloc(p, size)
#define DEALLOCATE(p) free(p)

// Check and free pointer.
#define FREE(v) if ((v) != NULL) { DEALLOCATE(v); (v) = NULL; }

#define MEMCOPY(destination, source, size) memcpy(destination, source, size)
#define MEMMOVE(destination, source, size) memmove(destination, source, size)

#endif //SANDBOX_ALLOCATE_H
