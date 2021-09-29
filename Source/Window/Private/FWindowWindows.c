/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <Windows.h>

#include "Core/FMemory.h"
#include "Core/FMath.h"
#include "Core/FLog.h"
#include "FInputWindows.h"
#include "Window/FWindow.h"
#include "FWindowWindows.h"

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

FWindow FWindowCreate(const char* pTitle, FInt32 width, FInt32 height, FWindowStyle style)
{
    if (pTitle == NULL || width <= 0 || height <= 0 || !FMathIsBetween(style, 0, FWindowStyle_Max))
    {
        return NULL;
    }

    /* Allocate the window */
    FWindowWindows* pWindow = FAllocateZero(1, sizeof(FWindowWindows));
    if (pWindow == NULL)
    {
        return NULL;
    }

    /* Get the style */
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

    /* Create the window */
    pWindow->hWnd = CreateWindowEx(style, "Enterprise_DefaultWindow", pTitle, winStyle, 0, 0, width, height, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (pWindow->hWnd == NULL)
    {
        FDeallocate(pWindow);
        FLogError("Could not create a new window!");
        return NULL;
    }

    ShowWindow(pWindow->hWnd, SHOW_OPENWINDOW);

    /* Set the windows user pointer to the pWindow */
    SetWindowLongPtr(pWindow->hWnd, GWLP_USERDATA, (LONG_PTR)pWindow);

    return pWindow;
}

void FWindowDestroy(FWindow window)
{
    if (window == NULL)
    {
        return;
    }

    FWindowWindows* pWindow = window;

    CloseWindow(pWindow->hWnd);
    FDeallocate(pWindow);
}

void FWindowGetSize(FWindow window, FUInt32* pWidth, FUInt32* pHeight)
{
    if (window == NULL || pWidth == NULL || pHeight == NULL)
    {
        return;
    }

    FWindowWindows* pWindow = window;

    RECT rect = {0};
    GetClientRect(pWindow->hWnd, &rect);

    *pWidth = rect.right - rect.left;
    *pHeight = rect.bottom - rect.top;
}

void* FWindowGetHandle(FWindow window)
{
    if (window == NULL)
    {
        return NULL;
    }

    FWindowWindows* pWindow = window;

    return pWindow->hWnd;
}

void FWindowSetUserData(FWindow window, void* pUserData)
{
    if (window == NULL)
    {
        return;
    }

    FWindowWindows* pWindow = window;

    pWindow->pUserData = pUserData;
}

void* FWindowGetUserData(FWindow window)
{
    if (window == NULL)
    {
        return NULL;
    }

    FWindowWindows* pWindow = window;

    return pWindow->pUserData;
}

bool FWindowShouldClose(FWindow window)
{
    if (window == NULL)
    {
        return true;
    }

    FWindowWindows* pWindow = window;

    return pWindow->shouldClose;
}