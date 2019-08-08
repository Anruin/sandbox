// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// Copyright (c) E. A. Pristavka, 2017
//

#include <api/module.h>
#include <common/network/peer.h>
#include <common/common.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <string.h>
#include <SDL_log.h>
#include <SDL_net.h>

CStringConst AModule_Name = "SocketServer";

CPeer *peers;
i32 pSocket;
i16 nSocketPort = 8080;

void AModule_OnCreate() {
    peers = NULL;

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Starting server on port %d.\n", nSocketPort);

    // create a listening TCP socket on port 9999 (server)
    IPaddress ipAddress;
    TCPsocket tcpSocket;

    if (SDLNet_ResolveHost(&ipAddress, NULL, nSocketPort) == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(1);
    }

    tcpSocket = SDLNet_TCP_Open(&ipAddress);
    if (!tcpSocket) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(2);
    }

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "SocketServer module created!\n");
}

void AModule_OnReady() {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "SocketServer module ready!\n");
}

void AModule_OnUpdate() {
    char pBuffer[255];
    int n;

    bzero(pBuffer, 255);

    read(pSocket, pBuffer, sizeof(pBuffer));

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Received: %s", pBuffer);

    bzero(pBuffer, 255);

    n = 0;

    while ((pBuffer[n++] = getchar()) != '\n');

    write(pSocket, pBuffer, sizeof(pBuffer));

    if (strncmp("exit", pBuffer, 4) == 0) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Exit command received.\n");

        // AApplication_Quit(0);
    }
}

void AModule_OnDestroy() {
    close(pSocket);

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "SocketServer module unloaded!\n");
}
