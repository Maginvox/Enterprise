#include <xcb/xcb.h>

#include "Core/enMemory.h"
#include "Core/enLog.h"

#include "Input/enInput.h"
#include "Graphics/enWindow.h"

void enInputPoll()
{
    xcb_generic_event_t* event = xcb_poll_for_event(win.connection);
    
    if (event == NULL)
    {
        return;
    }

    switch (event->response_type & 0x7f)
    {
        case XCB_EXPOSE:
            
            break;

        case XCB_CLIENT_MESSAGE: /* Test for close event */
            xcb_client_message_event_t* pClientEvent = (xcb_client_message_event_t*)event;
            
            if (pClientEvent->data.data32[0] == win.deleteAtom)
            {
                win.close = true;
            }

            
            break;
    }
}