/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __ENTERPRISE_THREAD_H__
#define __ENTERPRISE_THREAD_H__

#include "Core/enTypes.h"

typedef void* FMutex;

FMutex* enMutexCreate();
void enMutexDestroy(FMutex** mutex);
void enMutexLock(FMutex* mutex);
void enMutexUnlock(FMutex* mutex);

typedef void (*enThreadFunction)(void* pParameter);
typedef struct enThread 
{
    enThreadFunction pFunction;
    void* pParameter;
    void* pThread;
} enThread;

enThread* enThreadCreate(enThreadFunction pFunction, void* pParameter);
void enThreadJoin(enThread** ppThread);
void enThreadSleep(uint64 milliseconds); /* Sleeps current thread */

#endif