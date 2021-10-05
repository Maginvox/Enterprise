#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include "Core/enMemory.h"
#include "Core/enString.h"
#include "Graphics/enWindow.h"

enWindowXLib window_xlib;

bool enWindowInitialize(const enWindowInfo* pInfo)
{
    window_xlib.pDisplay = XOpenDisplay(NULL);
    if (window_xlib.pDisplay == NULL)
    {
        return false;
    }

    window_xlib.screen = DefaultScreen(window_xlib.pDisplay);
    window_xlib.window = XCreateSimpleWindow(window_xlib.pDisplay, RootWindow(window_xlib.pDisplay, window_xlib.screen), 0, 0, pInfo->width, pInfo->height, 1, BlackPixel(window_xlib.pDisplay, window_xlib.screen), WhitePixel(window_xlib.pDisplay, window_xlib.screen));

    XSetStandardProperties(
        window_xlib.pDisplay, 
        window_xlib.window, 
        pInfo->pTitle, 
        NULL, 
        None, 
        NULL, 
        0,
        NULL);
    XSelectInput(window_xlib.pDisplay, window_xlib.window, ExposureMask | KeyPressMask | StructureNotifyMask );

    /* Register interest in the delete window message */
    Atom wmDeleteMessage = XInternAtom(window_xlib.pDisplay, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(window_xlib.pDisplay, window_xlib.window, &window_xlib.wmDeleteMessage, 1);

    XClearWindow(window_xlib.pDisplay, window_xlib.window);
    XMapWindow(window_xlib.pDisplay, window_xlib.window);
    
    XFlush(window_xlib.pDisplay);
}

void enWindowShutdown()
{
    XDestroyWindow(window_xlib.pDisplay, window_xlib.window);
    XCloseDisplay(window_xlib.pDisplay);
}

bool enWindowShouldClose()
{
    return window_xlib.shouldClose;
}

void enWindowGetSize(uint32* pWidth, uint32* pHeight)
{
    XGetGeometry(window_xlib.pDisplay, window_xlib.window, NULL, NULL, NULL, (unsigned int*)pWidth, (unsigned int*)pHeight, NULL, NULL);
}

Display* enWindowGetDisplay()
{
    return window_xlib.pDisplay;
}

Window enWindowGetHandle()
{
    return window_xlib.window;
}