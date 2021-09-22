/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <Windows.h>

#include "Core/FLog.h"
#include "Graphics/FWindow.h"
#include "Window/FInputWindows.h"


typedef struct FWindowWindows {
    HWND hWnd;
} FWindowWindows;

static bool registeredClasses = false; 

FWindow* FWindowCreate(FGraphicsContext* pContext, const FWindowCreateInfo* pInfo)
{
    if (pInfo == NULL)
    {
        return NULL;
    }

    if (pInfo->pTitle == NULL || pInfo->width <= 0 || pInfo->height <= 0)
    {
        return NULL;
    }

    if (!registeredClasses)
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
            .lpszClassName = "DefaultWindow",
            .hIconSm = NULL
        };

        RegisterClassEx(&defaultWindowClass);
        registeredClasses = true;
    }

    DWORD style = WS_EX_OVERLAPPEDWINDOW;
    HWND window = CreateWindowEx(style, "DefaultWindow", pInfo->pTitle, style, 0, 0, pInfo->width, pInfo->height, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (window == NULL)
    {
        FLogError("Could not create a new window!");
        return NULL;
    }

    ShowWindow(window, SHOW_OPENWINDOW);

    return (FWindow*)window;
}