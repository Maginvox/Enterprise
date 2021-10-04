/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/FMemory.h"
#include "Core/FThread.h"

#ifdef ENTERPRISE_WINDOWS

#include <windows.h>

FMutex* enMutexCreate()
{
    HANDLE hMutex = CreateMutex(NULL, FALSE, NULL);
    if (hMutex == NULL)
    {
        return NULL;
    }

    return (FMutex*)hMutex;
}

void enMutexDestroy(FMutex** ppMutex)
{
    if (*ppMutex == NULL)
    {
        return;
    }

    HANDLE hMutex = (HANDLE)(*ppMutex);

    CloseHandle(hMutex);
    *ppMutex = NULL;
}

void enMutexLock(FMutex* pMutex)
{
    if (pMutex == NULL)
    {
        return;
    }

    WaitForSingleObject(pMutex, INFINITE);
}

void enMutexUnlock(FMutex* pMutex)
{
    if (pMutex == NULL)
    {
        return;
    }

    ReleaseMutex(pMutex);
}

DWORD WINAPI FThreadWindows(LPVOID lpParam)
{
    enThread* pThread = (enThread*)lpParam;

    pThread->pFunction(pThread->pParameter);

    return 0;
}

enThread* enThreadCreate(enThreadFunction pFunction, void* pParameter)
{

    enThread* pThread = enAllocateZero(1, sizeof(enThread));
    if (pThread == NULL)
    {
        return NULL;
    }

    pThread->pFunction = pFunction;
    pThread->pParameter = pParameter;

    /* Create the windows thread */
    HANDLE hThread = CreateThread(NULL, 0, FThreadWindows, pThread, 0, NULL);
    if (hThread == NULL)
    {
        enThreadJoin(&pThread);
        return NULL;
    }
	
	pThread->pThread = (void*)hThread;

    return pThread;
}

void enThreadJoin(enThread** ppThread)
{
    if (ppThread == NULL || *ppThread == NULL)
    {
        return;
    }

    enThread* pThread = *ppThread;
    HANDLE hThread = (HANDLE)pThread->pThread;

    enDeallocate(pThread);
    *ppThread = NULL;

    WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
}

void enThreadSleep(int64 milliseconds)
{
    Sleep((DWORD)milliseconds);
}

#elif ENTERPRISE_LINUX

#include <pthread.h>
#include <unistd.h> /* For sleep() */

/* ====================================================== */
FMutex* enMutexCreate()
{
    pthread_mutex_t* pMutex = enAllocateZero(1, sizeof(pthread_mutex_t));
    if (pMutex == NULL)
    {
        return NULL;
    }

    if (pthread_mutex_init(pMutex, NULL) != 0)
    {
        enDeallocate(pMutex);
        return NULL;
    }

    return (FMutex*)pMutex;
}

/* ====================================================== */
void enMutexDestroy(FMutex** ppMutex)
{
    if (ppMutex == NULL || *ppMutex == NULL)
    {
        return;
    }

    /* Destroy the mutex */
    pthread_mutex_t* pMutex = (pthread_mutex_t*)(*ppMutex);
    pthread_mutex_destroy(pMutex);

    enDeallocate(pMutex);
    *ppMutex = NULL;
}

/* ====================================================== */
void enMutexLock(FMutex* pMutex)
{
    if (pMutex == NULL)
    {
        return;
    }

    pthread_mutex_lock((pthread_mutex_t*)pMutex);
}

/* ====================================================== */
void enMutexUnlock(FMutex* pMutex)
{
    if (pMutex == NULL)
    {
        return;
    }

    pthread_mutex_unlock((pthread_mutex_t*)pMutex);
}

void* FThreadUnix(void* pParameter)
{
    enThread* pThread = (enThread*)pParameter;
    pThread->pFunction(pThread->pParameter);
    return NULL;
}

/* ====================================================== */
enThread* enThreadCreate(enThreadFunction pFunction, void* pParameter)
{
    if (pFunction == NULL)
    {
        return NULL;
    }
    
    enThread* pThread = enAllocateZero(1, sizeof(enThread));
    if (pThread == NULL)
    {
        return NULL;
    }

    pThread->pThread = enAllocateZero(1, sizeof(pthread_t));
    if (pThread->pThread == NULL)
    {
        enThreadJoin(&pThread);
        return NULL;
    } 

    if (pthread_create(pThread->pThread, NULL, FThreadUnix, pParameter) != 0)
    {
        enThreadJoin(&pThread);
        return NULL;
    }

    return pThread;
}

/* ====================================================== */
void enThreadJoin(enThread** ppThread)
{
    if (ppThread == NULL || *ppThread == NULL)
    {
        return;
    }

    enThread* pThread = *ppThread;

    if (pThread->pThread != NULL)
    {
        pthread_t threadId = *(pthread_t*)pThread->pThread;

        enDeallocate(pThread->pThread);
        enDeallocate(pThread);
        *ppThread = NULL;    

        pthread_join(threadId, NULL);
    }
    else    
    {
        enDeallocate(pThread);
        *ppThread = NULL;
    }
}

/* ====================================================== */
void enThreadSleep(uint64 milliseconds)
{
    sleep(milliseconds * 1000);
}

#endif