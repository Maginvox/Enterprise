#ifndef __FWINDOW_H__
#define __FWINDOW_H__

#include "Core/FTypes.h"

#ifdef ENTERPRISE_WINDOWS
    #include <Windows.h>

    typedef struct
    {
        HWND hWnd;
        bool shouldClose;
    } FWindowWindows;

    extern FWindowWindows window_win;

#elif ENTERPRISE_MACOS
    #error NOT IMPLEMENTED
#elif ENTERPRISE_ANDROID
    #error NOT IMPLEMENTED
#elif ENTERPRISE_LINUX
    #include <X11/Xlib.h>

    typedef struct
    {
        Window window;
    } FWindowXLib;

    extern FWindowXLib window_xlib;
#endif

typedef struct
{
    const char* pTitle;
    FUInt width;
    FUInt height;
} FWindowInfo;

bool FWindowInitialize(const FWindowInfo* pInfo);
void FWindowShutdown();
bool FWindowShouldClose();
void FWindowGetSize(FUInt* pWidth, FUInt* pHeight);

#ifdef ENTERPRISE_WINDOWS
    HWND FWindowGetHandle();
#elif ENTERPRISE_MACOS
    #error NOT IMPLEMENTED
#elif ENTERPRISE_ANDROID
    #error NOT_IMPLEMENTED
#elif ENTERPRISE_LINUX
    Display* FWindowGetDisplay();
    Window FWindowGetHandle();
#endif


/* On window callbacks */
void FWindowOnResize(FUInt width, FUInt height);

#endif