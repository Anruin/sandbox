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

CStringConst AModule_Name = "SocketServer";

CPeer* peers;
i32 pSocket;
i16 socketPort = 8080;

void AModule_OnCreate() {
    peers = NULL;

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Starting server on port %d.\n", socketPort);

    pSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (pSocket < 0) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Could not create a socket.\n");
        assert(0);
    } else {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Socket created.\n");
    }

    struct sockaddr_in serverAddress = {0}, client = {0};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(socketPort);

    if ((bind(pSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress))) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Socket bind failed.\n");
        assert(0);
    } else {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Socket bound.\n");
    }

    if ((listen(pSocket, 5)) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Socked listen failed.\n");
        assert(0);
    } else {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Socket listening.\n");
    }

    i32 len = sizeof(client);

    i32 pConnection = accept(pSocket, (struct sockaddr*)&client, &len);

    if (pConnection < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Socket accept failed.\n");
        assert(0);
    } else {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Socket accepted connection.\n");
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
