/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifdef ENTERPRISE_WINDOWS

#include <WinSock2.h>

#include "Networking/FSocket.h"

bool FSocketInit()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        return false;
    }

    return true;
}

void FSocketShutdown()
{
    WSACleanup();
}

#elif ENTERPRISE_LINUX

#include <sys/socket.h>

#endif