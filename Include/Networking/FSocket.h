#ifndef __FSOCKET_H__
#define __FSOCKET_H__

#include "Core/FTypes.h"

typedef struct FSocket FSocket;

bool FSocketInit();
void FSocketShutdown();

bool FSocketOpen(FSocket* socket, const char* host, int port);
void FSocketClose(FSocket* socket);

bool FSocketSend(FSocket* socket, const char* data, int length);

void FSocketConnect(FSocket* socket, const char* host, int port);

#endif