//
// Created by egor on 04.08.2019.
//

#ifndef SANDBOX_PACKET_H
#define SANDBOX_PACKET_H

#include <common/common.h>

typedef struct APacketHeader {
    u8 type;
    u8 error;
    u32 length;
} CPacketHeader;

typedef struct APacket {
    CPacketHeader header;
    u8 payload[1000];
} CPacket;

#endif //SANDBOX_PACKET_H
