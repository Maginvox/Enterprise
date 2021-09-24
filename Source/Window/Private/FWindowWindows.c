/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <Windows.h>

#include "Core/FLog.h"
#include "Core/FMemory.h"
#include "Window/FWindow.h"
#include "FInputWindows.h"


typedef struct FWindowWindows {
    HWND hWnd;
    void* pRenderData;
} FWindowWindows;

static bool systemInitialized = false;

bool FWindowSystemInitialize()
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

void FWindowSystemShutdown()
{

}

FWindow* FWindowCreate(const FWindowCreateInfo* pInfo)
{
    if (pInfo == NULL || pInfo->pTitle == NULL || pInfo->width <= 0 || pInfo->height <= 0)
    {
        return NULL;
    }

    DWORD style = WS_EX_OVERLAPPEDWINDOW;
    HWND window = CreateWindowEx(style, "Enterprise_DefaultWindow", pInfo->pTitle, style, 0, 0, pInfo->width, pInfo->height, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (window == NULL)
    {
        FLogError("Could not create a new window!");
        return NULL;
    }

    ShowWindow(window, SHOW_OPENWINDOW);

    FWindowWindows* pWindowWindows = FAllocateZero(1, sizeof(FWindowWindows));
    if (pWindowWindows == NULL)
    {
        CloseWindow(window);
        return NULL;
    }

    pWindowWindows->hWnd = window;

    return (FWindow*)pWindowWindows;
}

void FWindowDestroy(FWindow* pWindow)
{
    if (pWindow == NULL)
    {
        return;
    }

    FWindowWindows* pWindowWindows = (FWindowWindows*)*pWindow;
    CloseWindow(pWindowWindows->hWnd);

    FDeallocate(pWindowWindows);
}

void FWindowSetRenderData(FWindow* pWindow, void* pRenderData)
{
    if (pWindow == NULL)
    {
        return;
    }

    FWindowWindows* pWindowWindows = (FWindowWindows*)pWindow;

    pWindowWindows->pRenderData = pRenderData;
}

void* FWindowGetRenderData(const FWindow* pWindow)
{
    if (pWindow == NULL)
    {
        return NULL;
    }

    FWindowWindows* pWindowWindows = (FWindowWindows*)pWindow;

    return pWindowWindows->pRenderData;
}