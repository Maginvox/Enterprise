#include "Graphics/FWindow.h"


/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <Windows.h>

#include "Core/FMemory.h"
#include "Core/FMath.h"
#include "Core/FLog.h"
#include "FInputWindows.h"
#include "Window/FWindow.h"
#include "FWindowWindows.h"

static bool systemInitialized = false;

bool FWindowInitialize(const FWindowInfo* pInfo)
{
    if (pTitle == NULL || width <= 0 || height <= 0 || !FMathIsBetween(style, 0, FWindowStyle_Max))
    {
        return NULL;
    }

    /* Register the window class */
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
    window_win. = CreateWindowEx(style, "Enterprise_DefaultWindow", pTitle, winStyle, 0, 0, width, height, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (window_win.hWnd == NULL)
    {
        FDeallocate(pWindow);
        FLogError("Could not create a new window!");
        return NULL;
    }

    ShowWindow(window_win.hWnd, SHOW_OPENWINDOW);

    /* Set the windows user pointer to the pWindow */
    SetWindowLongPtr(window_win.hWnd, GWLP_USERDATA, (LONG_PTR)pWindow);

    return pWindow;
}

void FWindowShutdown()
{

    if (window_win.hWnd != NULL)
    {
        CloseWindow(window_win.hWnd);
    }
    
    FDeallocate(pWindow);
}

void FWindowGetSize(FUInt32* pWidth, FUInt32* pHeight)
{
    if (pWidth == NULL || pHeight == NULL)
    {
        return;
    }

    RECT rect = {0};
    GetClientRect(window_win.hWnd, &rect);

    *pWidth = rect.right - rect.left;
    *pHeight = rect.bottom - rect.top;
}

HWND FWindowGetHandle()
{
    return window_win.hWnd;
}

bool FWindowShouldClose()
{
    return window_win.shouldClose;
}