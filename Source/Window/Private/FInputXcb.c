#include "X11/Xlib-xcb.h"

#include "Window/FInput.h"
#include "FWindowX11.h"

void FInputPoll()
{
    XEvent event;
    XNextEvent(pDisplay, &event);

    switch (event.type)
    {
        case DestroyNotify:
            XtSetValue
            break;
    }

}