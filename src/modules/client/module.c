// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// Copyright (c) E. A. Pristavka, 2017
//

#include <api/module.h>
#include <common/network/peer.h>
#include <common/common.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // for close
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

CStringConst AModule_Name = "SocketClient";

CPeer* peers;
i32 pSocket;
i16 socketPort = 8080;

void AModule_OnCreate() {
    peers = NULL;

    DEBUG_PRINT("%s: starting server on port %d.\n", AModule_Name, socketPort);

    pSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (pSocket < 0) {
        DEBUG_PRINT("%s: could not create a socket.\n", AModule_Name);
        assert(0);
    } else {
        DEBUG_PRINT("%s: socket created.\n", AModule_Name);
    }

    struct sockaddr_in serverAddress = {0}, client = {0};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(socketPort);

    if ((connect(pSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress))) != 0) {
        DEBUG_PRINT("%s: socket connect failed.\n", AModule_Name);
        assert(0);
    } else {
        DEBUG_PRINT("%s: socket connected.\n", AModule_Name);
    }

    DEBUG_PRINT("%s: module created!\n", AModule_Name);
}

void AModule_OnReady() {
    DEBUG_PRINT("%s: module ready!\n", AModule_Name);
}

void AModule_OnUpdate() {
    char pBuffer[255];
    int n;

    memset(pBuffer, 0, 255);

    n = 0;

    while ((pBuffer[n++] = getchar()) != '\n');

    write(pSocket, pBuffer, sizeof(pBuffer));

    read(pSocket, pBuffer, sizeof(pBuffer));

    DEBUG_PRINT("%s: received: %s", AModule_Name, pBuffer);

    bzero(pBuffer, 255);

    if (strncmp("exit", pBuffer, 4) == 0) {
        DEBUG_PRINT("%s: exit command received.\n", AModule_Name);

        // AApplication_Quit(0);
    }
}

void AModule_OnDestroy() {
    close(pSocket);

    DEBUG_PRINT("%s: module unloaded!\n", AModule_Name);
}
