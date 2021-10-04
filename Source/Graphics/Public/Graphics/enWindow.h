#ifndef __FWINDOW_H__
#define __FWINDOW_H__

#include "Core/enTypes.h"

#ifdef ENTERPRISE_WINDOWS
    #include <Windows.h>

    typedef struct
    {
        HWND hWnd;
        bool shouldClose;
    } enWindowWindows;

    extern enWindowWindows window_win;

    LRESULT CALLBACK enWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
} enWindowInfo;

bool enWindowInitialize(const enWindowInfo* pInfo);
void enWindowShutdown();
bool enWindowShouldClose();
void enWindowGetSize(uint32* pWidth, uint32* pHeight);

#ifdef ENTERPRISE_WINDOWS
    HWND enWindowGetHandle();
#elif ENTERPRISE_MACOS
    #error NOT IMPLEMENTED
#elif ENTERPRISE_ANDROID
    #error NOT_IMPLEMENTED
#elif ENTERPRISE_LINUX
    Display* FWindowGetDisplay();
    Window enWindowGetHandle();
#endif


/* On window callbacks */
void enWindowOnResize(uint32 width, uint32 height);

uint32 enScreenPrimary();
void enScreenEnumerate(uint32* pCount, uint32* pScreens);

#endif