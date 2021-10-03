/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/FMemory.h"
#include "Core/FThread.h"

#ifdef ENTERPRISE_WINDOWS

#include <windows.h>

FMutex* FMutexCreate()
{
    HANDLE hMutex = CreateMutex(NULL, FALSE, NULL);
    if (hMutex == NULL)
    {
        return NULL;
    }

    return (FMutex*)hMutex;
}

void FMutexDestroy(FMutex** ppMutex)
{
    if (*ppMutex == NULL)
    {
        return;
    }

    HANDLE hMutex = (HANDLE)(*ppMutex);

    CloseHandle(hMutex);
    *ppMutex = NULL;
}

void FMutexLock(FMutex* pMutex)
{
    if (pMutex == NULL)
    {
        return;
    }

    WaitForSingleObject(pMutex, INFINITE);
}

void FMutexUnlock(FMutex* pMutex)
{
    if (pMutex == NULL)
    {
        return;
    }

    ReleaseMutex(pMutex);
}

DWORD WINAPI FThreadWindows(LPVOID lpParam)
{
    FThread* pThread = (FThread*)lpParam;

    pThread->pFunction(pThread->pParameter);

    return 0;
}

FThread* FThreadCreate(FThreadFunction pFunction, void* pParameter)
{

    FThread* pThread = FAllocateZero(1, sizeof(FThread));
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
        FThreadJoin(&pThread);
        return NULL;
    }
	
	pThread->pThread = (void*)hThread;

    return pThread;
}

void FThreadJoin(FThread** ppThread)
{
    if (ppThread == NULL || *ppThread == NULL)
    {
        return;
    }

    FThread* pThread = *ppThread;
    HANDLE hThread = (HANDLE)pThread->pThread;

    FDeallocate(pThread);
    *ppThread = NULL;

    WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
}

void FThreadSleep(int64 milliseconds)
{
    Sleep((DWORD)milliseconds);
}

#elif ENTERPRISE_LINUX

#include <pthread.h>
#include <unistd.h> /* For sleep() */

/* ====================================================== */
FMutex* FMutexCreate()
{
    pthread_mutex_t* pMutex = FAllocateZero(1, sizeof(pthread_mutex_t));
    if (pMutex == NULL)
    {
        return NULL;
    }

    if (pthread_mutex_init(pMutex, NULL) != 0)
    {
        FDeallocate(pMutex);
        return NULL;
    }

    return (FMutex*)pMutex;
}

/* ====================================================== */
void FMutexDestroy(FMutex** ppMutex)
{
    if (ppMutex == NULL || *ppMutex == NULL)
    {
        return;
    }

    /* Destroy the mutex */
    pthread_mutex_t* pMutex = (pthread_mutex_t*)(*ppMutex);
    pthread_mutex_destroy(pMutex);

    FDeallocate(pMutex);
    *ppMutex = NULL;
}

/* ====================================================== */
void FMutexLock(FMutex* pMutex)
{
    if (pMutex == NULL)
    {
        return;
    }

    pthread_mutex_lock((pthread_mutex_t*)pMutex);
}

/* ====================================================== */
void FMutexUnlock(FMutex* pMutex)
{
    if (pMutex == NULL)
    {
        return;
    }

    pthread_mutex_unlock((pthread_mutex_t*)pMutex);
}

void* FThreadUnix(void* pParameter)
{
    FThread* pThread = (FThread*)pParameter;
    pThread->pFunction(pThread->pParameter);
    return NULL;
}

/* ====================================================== */
FThread* FThreadCreate(FThreadFunction pFunction, void* pParameter)
{
    if (pFunction == NULL)
    {
        return NULL;
    }
    
    FThread* pThread = FAllocateZero(1, sizeof(FThread));
    if (pThread == NULL)
    {
        return NULL;
    }

    pThread->pThread = FAllocateZero(1, sizeof(pthread_t));
    if (pThread->pThread == NULL)
    {
        FThreadJoin(&pThread);
        return NULL;
    } 

    if (pthread_create(pThread->pThread, NULL, FThreadUnix, pParameter) != 0)
    {
        FThreadJoin(&pThread);
        return NULL;
    }

    return pThread;
}

/* ====================================================== */
void FThreadJoin(FThread** ppThread)
{
    if (ppThread == NULL || *ppThread == NULL)
    {
        return;
    }

    FThread* pThread = *ppThread;

    if (pThread->pThread != NULL)
    {
        pthread_t threadId = *(pthread_t*)pThread->pThread;

        FDeallocate(pThread->pThread);
        FDeallocate(pThread);
        *ppThread = NULL;    

        pthread_join(threadId, NULL);
    }
    else    
    {
        FDeallocate(pThread);
        *ppThread = NULL;
    }
}

/* ====================================================== */
void FThreadSleep(uint64 milliseconds)
{
    sleep(milliseconds * 1000);
}

#endif