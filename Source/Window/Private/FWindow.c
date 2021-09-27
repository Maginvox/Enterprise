#include "FWindow_Impl.h"

FWindowCreateCallback pCreateCallback;
FWindowDestroyCallback pDestroyCallback;


void FWindowSetUserData(FWindow* pWindow, void* pUserData)
{
    if (pWindow == NULL)
    {
        return;
    }

    pWindow->pUserData = pUserData;
}

void* FWindowGetUserData(const FWindow* pWindow)
{
    if (pWindow == NULL)
    {
        return NULL;
    }

    return pWindow->pUserData;
}
