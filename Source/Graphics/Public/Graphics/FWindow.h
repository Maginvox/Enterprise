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
    #include <xcb/xcb.h>

    typedef struct
    {
        xcb_connection_t* pConnection;
        xcb_window_t window;
        xcb_screen_t* pPrimaryScreen;
        xcb_screen_t* pScreen;
        bool shouldClose;
    } FWindowXcb;

    extern FWindowXcb window_xcb;
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
    xcb_connection_t* FWindowGetConnection();
    xcb_window_t FWindowGetHandle();
#endif


/* On window callbacks */
void FWindowOnResize(uint32 width, uint32 height);

uint32 FScreenPrimary();
void FScreenEnumerate(uint32* pCount, uint32* pScreens);

#endif