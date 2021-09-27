/* Copyright © 2021 Caden Miller, All Rights Reserved. */


#include "X11/Xlib.h"

#include "Core/FMemory.h"
#include "Core/FMath.h"
#include "Core/FLog.h"
#include "FWindow_Impl.h"

Display* pDisplay = NULL;

bool FWindowInitialize()
{
    pDisplay = XOpenDisplay(NULL);
    if (pDisplay == NULL) {
        FLogError("Could not open an X11 display!");
        return false;
    }

    return true;
}

void FWindowShutdown()
{
    if (pDisplay != NULL)
    {
        XCloseDisplay(pDisplay);
    }
}

void* FWindowSystemHandle()
{
    return pDisplay;
}

FWindow* FWindowCreate(const char* pTitle, FInt32 width, FInt32 height, FWindowStyle style)
{
    if (width <= 0 || height <= 0 || !FMathIsBetween(style, 0, FWindowStyle_Max))
    {
        return NULL;
    }

    if (pDisplay == NULL)
    {
        if (!FWindowInitialize())
        {
            return NULL;
        }
    }

    FWindow* pWindow = FAllocateZero(1, sizeof(FWindow));
    if (pWindow == NULL)
    {
        return NULL;
    }
    
    /* Create the X11 window */
    int screen = DefaultScreen(pDisplay);
    pWindow->pHandle = (void*)XCreateSimpleWindow(pDisplay, RootWindow(pDisplay, screen), 10, 10, 100, 100, 1, BlackPixel(pDisplay, screen), WhitePixel(pDisplay, screen));
    /* ^ A pointer ^ is just a number, so I guess this works? */
 
    if (pWindow->pHandle == NULL)
    {
        FWindowDestroy(pWindow);
        return NULL;
    }

    XMapWindow(pDisplay, (Window)pWindow->pHandle);
    XSelectInput(pDisplay, (Window)pWindow->pHandle, ExposureMask | KeyPressMask);

    /* Send out the create callback */
    if (pCreateCallback != NULL)
    {
        if (!pCreateCallback(pWindow))
        {
            FWindowDestroy(pWindow);
            return NULL;
        }
    }

    return pWindow;
}

void FWindowDestroy(FWindow* pWindow)
{
    if (pWindow == NULL)
    {
        return;
    }

    XDestroyWindow(pDisplay, (Window)pWindow->pHandle);

    FDeallocate(pWindow);
}

void* FWindowGetSystemHandle(const FWindow* pWindow)
{
    return pWindow->pHandle;
}

void FWindowGetSize(const FWindow* pWindow, FUInt32* pWidth, FUInt32* pHeight)
{
    Window window = (Window)pWindow;

    XWindowAttributes attributes = {0};
    XGetWindowAttributes(pDisplay, window, &attributes);

    *pWidth = attributes.width;
    *pHeight = attributes.height;
}