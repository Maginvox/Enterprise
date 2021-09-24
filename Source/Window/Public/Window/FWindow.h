#ifndef __FWINDOW_H__
#define __FWINDOW_H__

#include "Core/FTypes.h"


typedef enum FWindowStyle
{
    FWindowStyle_Default, /* All normal buttons and styling for the system */
    FWindowStyle_Minimal, /* No buttons or styling, only a surface */
} FWindowStyle;

typedef struct FWindowCreateInfo
{
    const char* pTitle;
    FInt32 width;
    FInt32 height;
    FWindowStyle style;
} FWindowCreateInfo;

typedef struct FWindow FWindow;

FWindow* FWindowCreate(const FWindowCreateInfo* pInfo);
void  FWindowDestroy(FWindow* pWindow);
void* FWindowGetSystemHandle(const FWindow* pWindow);
void  FWindowSetUserData(FWindow* pWindow, void* pData);
void* FWindowGetUserData(const FWindow* pWindow);

#include "FInput.h"

void FWindowSetInputCloseCallback

#endif