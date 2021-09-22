/* Copyright © 2021 Caden Miller, All Rights Reserved. */


#include "X11/Xlib.h"

#include "Core/FLog.h"
#include "Window/FWindow.h"

Display* pDisplay = NULL;

bool FX11Initialize()
{
    pDisplay = XOpenDisplay(NULL);
    if (pDisplay == NULL) {
        FLogError("Could not open an X11 display!");
        return false;
    }

    return true;
}

void FX11Shutdown()
{
    if (pDisplay != NULL)
    {
        XCloseDisplay(pDisplay);
    }
}

FWindow* FWindowCreate(FWindowCreateInfo* pInfo)
{
    


    int screen = DefaultScreen(pDisplay);
    Window window = XCreateSimpleWindow(pDisplay, RootWindow(pDisplay, screen), 10, 10, 100, 100, 1, BlackPixel(pDisplay, screen), WhitePixel(pDisplay, screen));

    
    XMapWindow(pDisplay, window);

    XSelectInput(pDisplay, window, ExposureMask | KeyPressMask);

}