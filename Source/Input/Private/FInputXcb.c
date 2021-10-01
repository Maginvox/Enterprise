#include <xcb/xcb.h>

#include "Input/FInput.h"
#include "Graphics/FWindow.h"

void FInputPoll()
{
    xcb_generic_event_t* pEvent = xcb_poll_for_event(window_xcb.pConnection);
    
    switch (pEvent->response_type)
    {
        case XCB_CLIENT_MESSAGE:
            
            break;
    }
}