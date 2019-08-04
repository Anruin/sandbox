//
// Created by egor on 04.08.2019.
//

#ifndef SANDBOX_PEER_H
#define SANDBOX_PEER_H

#include <common/common.h>

typedef struct APeer {
    char address[64];
    u8 bConnected;
} CPeer;

#endif //SANDBOX_PEER_H
