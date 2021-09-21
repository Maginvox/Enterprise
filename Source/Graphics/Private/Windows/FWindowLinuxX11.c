/* Copyright © 2021 Caden Miller, All Rights Reserved. */


#include "X11/Xlib.h"

#include "Core/FLog.h"
#include "Graphics/FWindow.h"

Display* pXDisplay = NULL;

FWindow* FWindowCreate(FGraphicsContext* pContext, FWindowCreateInfo* pInfo)
{
    
    Display* pDisplay = XOpenDisplay(NULL);
    if (pDisplay == NULL) {
        FLogError("Could not open an X11 display!");
        return NULL;
    }

    int screen = DefaultScreen(pDisplay);
    Window window = XCreateSimpleWindow(pDisplay, RootWindow(pDisplay, screen), 10, 10, 100, 100, 1, NULL, NULL);
    if (window == NULL)
    {
        XCloseDisplay(pDisplay);
        FLogError("Could not create an X11 window!");
        return NULL;
    }
    
    XMapWindow(pDisplay, window);

    XSelectInput(pDisplay, window, ExposureMask | KeyPressMask);

}