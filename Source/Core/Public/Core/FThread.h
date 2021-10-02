/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FTHREAD_H__
#define __FTHREAD_H__

#include "FTypes.h"

typedef struct FMutex FMutex;

FMutex* FMutexCreate();
void FMutexDestroy(FMutex** mutex);
void FMutexLock(FMutex* mutex);
void FMutexUnlock(FMutex* mutex);

typedef void (*FThreadFunction)(void* pParameter);
typedef struct FThread 
{
    FThreadFunction pFunction;
    void* pParameter;
    void* pThread;
} FThread;

FThread* FThreadCreate(FThreadFunction pFunction, void* pParameter);
void FThreadJoin(FThread** ppThread);
void FThreadSleep(int64 milliseconds); /* Sleeps current thread */

#endif