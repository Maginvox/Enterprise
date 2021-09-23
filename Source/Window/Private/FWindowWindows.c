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

static FWindowSystemCreateCallbackFunction g_pWindowSystemCreateCallback;
static FWindowSystemDestroyCallbackFunction g_pWindowSystemDestroyCallback;
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

bool FWindowSystemIsInitialized()
{
    return systemInitialized;
}

void FWindowSystemSetCreateCallback(FWindowSystemCreateCallbackFunction pCreateCallback)
{
    g_pWindowSystemCreateCallback = pCreateCallback;
}

void FWindowSystemSetDestroyCallback(FWindowSystemDestroyCallbackFunction pDestroyCallback)
{
    g_pWindowSystemDestroyCallback = pDestroyCallback;
}

FWindow* FWindowCreate(const FWindowCreateInfo* pInfo)
{
    if (pInfo == NULL || pInfo->pTitle == NULL || pInfo->width <= 0 || pInfo->height <= 0)
    {
        return NULL;
    }

    if (!FWindowSystemIsInitialized())
    {
        if (!FWindowSystemInitialize())
        {
            FLogError("Could not initialize the windowing system!");
            return NULL;
        }
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

    if (g_pWindowSystemCreateCallback != NULL && !g_pWindowSystemCreateCallback((FWindow*)pWindowWindows))
    {
        return NULL;
    }

    return (FWindow*)pWindowWindows;
}

void FWindowDestroy(FWindow** ppWindow)
{
    if (ppWindow == NULL || *ppWindow == NULL)
    {
        return;
    }

    FWindowWindows* pWindowWindows = (FWindowWindows*)*ppWindow;

    CloseWindow(pWindowWindows->hWnd);

    FDeallocate(pWindowWindows);
}