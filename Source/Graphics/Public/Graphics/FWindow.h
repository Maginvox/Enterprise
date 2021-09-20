/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FWINDOW_H__
#define __FWINDOW_H__

#include "Core/FTypes.h"
#include "Graphics/FGraphicsContext.h"

typedef struct FWindow FWindow;

typedef enum FWindowType
{
    FWindowType_Default, /* All normal buttons and styling for the system */
    FWindowType_Minimal, /* No buttons or styling but a surface */
} FWindowType;

typedef struct FWindowCreateInfo
{
    FInt32 width;
    FInt32 height;
    const char* pTitle;
} FWindowCreateInfo;

/* Many platforms only support one surface for the application for example Android, Consoles and more.
   On platforms that do support more than one window or surface you can create as many as you like. */
FWindow* FWindowCreate(FGraphicsContext* pContext, const FWindowCreateInfo* pInfo);
void FWindowDestroy(FGraphicsContext* pContext, FWindow** ppWindow);

void FWindowSetTitle(FGraphicsContext* pContext, FWindow* pWindow, const char* pTitle);
void FWindowResize(FGraphicsContext* pContext, FWindow* pWindow, FInt32 width, FInt32 height);

#endif