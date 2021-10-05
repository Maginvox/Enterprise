#include "Graphics/enWindow.h"
#include "Input/enInput.h"

void enInputPoll()
{
    XEvent event;
    if (XNextEvent(enWindowGetDisplay(), &event) > 0)
    {
        switch (event.type)
        {
            case ClientMessage:
                if (event.xclient.data.l[0] == window_xlib.wmDeleteMessage)
                    window_xlib.shouldClose = true;
                break;
        }
    }
}