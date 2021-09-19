/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FWINDOW_H__
#define __FWINDOW_H__

#include "Core/FTypes.h"

typedef struct FWindow FWindow;

typedef struct FWindowCreateInfo
{
    FInt32 width;
    FInt32 height;
    const char* pTitle;
} FWindowCreateInfo;

FWindow FWindowCreate(FGraphicsContext* pContext, const FWindowCreateInfo* pInfo);
void FWindowDestroy(FGraphicsContext* pContext, FWindow** ppWindow);

void FWindowSetTitle(FGraphicsContext* pContext, FWindow* pWindow, const char* pTitle);
void FWindowResize(FGraphicsContext* pContext, FWindow* pWindow, FInt32 width, FInt32 height);

#endif