/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __LOG_H__
#define __LOG_H__

#include "Core/FExport.h"
#include "Core/FTime.h"
#include "Core/FString.h"

bool FLogToFile();
void FLogShutdown();

void FLog(const char* pBasis, const char* pContext, const char* pMessage);
void FLogInfo(const char* message);
void FLogWarning(const char* message);
void FLogError(const char* message);
void FLogDebug(const char* message);

#endif