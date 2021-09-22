#ifndef __FWINDOW_H__
#define __FWINDOW_H__

#include "Core/FTypes.h"

typedef struct FWindowCreateInfo
{
    const char* pTitle;
    FInt32 width;
    FInt32 height;
} FWindowCreateInfo;

typedef struct FWindow FWindow;

bool FWindowSystemInitialize();
void FWindowSystemShutdown();
bool FWindowSystemIsInitialized();

FWindow* FWindowCreate(FWindowCreateInfo* pInfo);
void FWindowDestroy(FWindow** ppWindow);
void* FWindowGetHandle(FWindow* pWindow);

#endif