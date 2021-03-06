/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <Windows.h>

#include "Core/enMemory.h"
#include "Core/enMath.h"
#include "Core/enLog.h"

#include "Graphics/enWindow.h"

enWindowWindows window_win;

bool enWindowInitialize(const enWindowInfo* pInfo)
{
    if (pInfo->pTitle == NULL /* || !FMathIsBetween(style, 0, FWindowStyle_Max) */)
    {
        return false;
    }

    /* Register the window class */
    WNDCLASSEX defaultWindowClass =
    {
        .cbSize = sizeof(WNDCLASSEX),
        .style = 0,
        .lpfnWndProc = enWindowProc,
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

    /* Get the style
    DWORD winStyle = 0;
    switch(style)
    {
        case FWindowStyle_Default:
            winStyle = WS_OVERLAPPEDWINDOW;
        break;
        case FWindowStyle_Minimal:
            winStyle = WS_POPUPWINDOW;
        break;
    } */

    /* Create the window */
    window_win.hWnd = CreateWindowEx(0, "Enterprise_DefaultWindow", pInfo->pTitle, WS_OVERLAPPEDWINDOW, 0, 0, pInfo->width, pInfo->height, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (window_win.hWnd == NULL)
    {
        enLogError("Could not create a new window!");
        return false;
    }

    ShowWindow(window_win.hWnd, SHOW_OPENWINDOW);

    return true;
}

void enWindowShutdown()
{
    if (window_win.hWnd != NULL)
    {
        CloseWindow(window_win.hWnd);
    }
}

void enWindowGetSize(uint32* pWidth, uint32* pHeight)
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

HWND enWindowGetHandle()
{
    return window_win.hWnd;
}

bool enWindowShouldClose()
{
    return window_win.shouldClose;
}