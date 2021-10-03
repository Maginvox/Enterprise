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

    LRESULT CALLBACK FWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#elif ENTERPRISE_MACOS
    #error NOT IMPLEMENTED
#elif ENTERPRISE_ANDROID
    #error NOT IMPLEMENTED
#elif ENTERPRISE_LINUX
    #include <X11/Xlib.h>

    typedef struct
    {
        Display* pDisplay;
        Window window;
        bool shouldClose;
    } FWindowXLib;

    extern FWindowXLib window_xlib;
#endif

typedef struct
{
    const char* pTitle;
    uint32 width;
    uint32 height;
    uint32 screen;
} FWindowInfo;

bool FWindowInitialize(const FWindowInfo* pInfo);
void FWindowShutdown();
bool FWindowShouldClose();
void FWindowGetSize(uint32* pWidth, uint32* pHeight);

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
void FWindowOnResize(uint32 width, uint32 height);

uint32 FScreenPrimary();
void FScreenEnumerate(uint32* pCount, uint32* pScreens);

#endif