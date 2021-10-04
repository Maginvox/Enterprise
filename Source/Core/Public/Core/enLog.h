/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __LOG_H__
#define __LOG_H__

#include "Core/enTypes.h"
#include "Core/enTime.h"
#include "Core/enString.h"


bool FLogInitialize(); /* Only initialize and shutdown if you want to log to file. */
void FLogShutdown();

void enLog(const char* pBasis, const char* pContext, const char* pMessage);
void enLogInfo(const char* message);
void enLogWarning(const char* message);
void enLogError(const char* message);
void enLogDebug(const char* message);

#endif