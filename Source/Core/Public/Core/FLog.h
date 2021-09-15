#ifndef __LOG_H__
#define __LOG_H__

#include "Core/FExport.h"
#include "Core/FTime.h"
#include "Core/FString.h"

FEXPORT bool FLogToFile();
FEXPORT void FLogShutdown();

FEXPORT void FLog(const char* pBasis, const char* pContext, const char* pMessage);
FEXPORT void FLogInfo(const char* message);
FEXPORT void FLogWarning(const char* message);
FEXPORT void FLogError(const char* message);
FEXPORT void FLogDebug(const char* message);

#endif