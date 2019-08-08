// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// Copyright (c) E. A. Pristavka, 2017
//

#include <api/module.h>
#include <common/network/peer.h>
#include <common/common.h>
#include <SDL_net.h>

CStringConst AModule_Name = "Tracker";

CPeer* peers;
i32 pSocket;
i16 socketPort = 8080;

void AModule_OnCreate() {
    peers = NULL;

    /*pSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (pSocket < 0) {
        DEBUG_PRINT("Could not create a socket.");
        assert(0);
    }

    struct sockaddr_in sockAddressIn;
    sockAddressIn.sin_family = AF_INET;
    sockAddressIn.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddressIn.sin_port = htons(socketPort);

*/

    DEBUG_PRINT("Tracker module created!\n");
}

void AModule_OnReady() {
    DEBUG_PRINT("Tracker module ready!\n");
}

void AModule_OnUpdate() {

}

void AModule_OnDestroy() {
    DEBUG_PRINT("Tracker module unloaded!\n");
}
