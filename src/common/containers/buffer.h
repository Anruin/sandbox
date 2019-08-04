//
// Created by egor on 01.08.2019.
//

#ifndef SANDBOX_BUFFER_H
#define SANDBOX_BUFFER_H

#include <common/common.h>

typedef struct ABuffer {
    u8 *data;
    u8 *position;
    i32 length;
    i32 capacity;
    i8 bHasData;
} CBuffer;

#endif //SANDBOX_BUFFER_H
