/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <Windows.h>

#include "Core/FMemory.h"
#include "Core/FMath.h"
#include "Core/FLog.h"
#include "FInputWindows.h"
#include "Window/FWindow.h"
#include "FWindow_Impl.h"


static bool systemInitialized = false;

bool FWindowInitialize()
{
    WNDCLASSEX defaultWindowClass =
    {
        .cbSize = sizeof(WNDCLASSEX),
        .style = 0,
        .lpfnWndProc = FWindowProcedure,
        .cbClsExtra = 0,
        .hInstance = GetModuleHandle(NULL),
        .hIcon = NULL,
        .hCursor = NULL,
        .hbrBackground = NULL,
        .lpszMenuName = NULL,
        .lpszClassName = "Enterprise_DefaultWindow",
        .hIconSm = NULL
    };

    RegisterClassEx(&defaultWindowClass);
    systemInitialized = true;

    return true;
}

void FWindowShutdown()
{

}

void* FWindowSystemHandle()
{
    return GetModuleHandle(NULL);
}

FWindow* FWindowCreate(const char* pTitle, FInt32 width, FInt32 height, FWindowStyle style)
{
    if (pTitle == NULL || width <= 0 || height <= 0 || !FMathIsBetween(style, 0, FWindowStyle_Max))
    {
        return NULL;
    }

    DWORD winStyle = 0;
    switch(style)
    {
        case FWindowStyle_Default:
            winStyle = WS_OVERLAPPEDWINDOW;
        break;
        case FWindowStyle_Minimal:
            winStyle = WS_POPUPWINDOW;
        break;
    }

    HWND window = CreateWindowEx(style, "Enterprise_DefaultWindow", pTitle, winStyle, 0, 0, width, height, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (window == NULL)
    {
        FLogError("Could not create a new window!");
        return NULL;
    }

    ShowWindow(window, SHOW_OPENWINDOW);

    FWindow* pWindow = FAllocateZero(1, sizeof(FWindow));
    if (pWindow == NULL)
    {
        CloseWindow(window);
        return NULL;
    }

    pWindow->pHandle = window;
    
    SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)pWindow);

    return pWindow;
}

void FWindowDestroy(FWindow* pWindow)
{
    if (pWindow == NULL)
    {
        return;
    }

    CloseWindow((HWND)pWindow->pHandle);

    FDeallocate(pWindow);
}

void FWindowGetSize(const FWindow* pWindow, FUInt32* pWidth, FUInt32* pHeight)
{
    if (pWindow == NULL || pWindow == NULL || pHeight == NULL)
    {
        return;
    }

    RECT rect = {0};
    GetClientRect((HWND)pWindow->pHandle, &rect);

    *pWidth = rect.right - rect.left;
    *pHeight = rect.bottom - rect.top;
}

void* FWindowGetHandle(const FWindow* pWindow)
{
    return pWindow->pHandle;
}