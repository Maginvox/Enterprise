#ifndef __LOG_H__
#define __LOG_H__

#include "Core/FTime.h"
#include "Core/FString.h"

bool FLogInit();
void FLogShutdown();

void FLog(const char* pBasis, const char* pContext, const char* pMessage);
void FLogInfo(const char* message);
void FLogWarning(const char* message);
void FLogError(const char* message);
void FLogDebug(const char* message);

#endif