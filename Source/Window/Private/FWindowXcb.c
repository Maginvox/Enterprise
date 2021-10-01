/* Copyright © 2021 Caden Miller, All Rights Reserved. */


#include "xcb/xcb.h"

#include "Core/FMemory.h"
#include "Core/FMath.h"
#include "Core/FLog.h"


FUInt defaultScreen = 0;
xcb_connection_t* pConnection = NULL;

bool FWindowInitialize()
{
    /* Connect to the X Server */
    pConnection = xcb_connect(NULL, &defaultScreen);

    if (xcb_connection_has_error(pConnection))
    {
        FLogError("Could not connect to the X Server!");
        return false;
    }

    

    return true;
}

void FWindowShutdown()
{
    if (pConnection != NULL)
    {
        xcb_flush(pConnection);
    }
}

void* FWindowSystemHandle()
{
    return pConnection;
}

FWindow* FWindowCreate(const char* pTitle, FInt32 width, FInt32 height, FWindowStyle style, FScreen screen)
{
    if (width <= 0 || height <= 0 || !FMathIsBetween(style, 0, FWindowStyle_Max))
    {
        return NULL;
    }

    FWindow* pWindow = FAllocateZero(1, sizeof(FWindow));
    if (pWindow == NULL)
    {
        return NULL;
    }

    /* Get the xcb screen */
    const xcb_setup_t* pSetup = xcb_get_setup(pConnection);
    xcb_screen_iterator_t screenIterator = xcb_setup_roots_iterator(pSetup);  
    
    for (FScreen screen = 0; )

    /* Create the Xcb window */
    xcb_window_t window = xcb_generate_id(pConnection);
    xcb_void_cookie_t cookie = xcb_create_window(pConnection, 0, window,  );

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

void* FWindowGetHandle(const FWindow* pWindow)
{
    return pWindow->pHandle;
}