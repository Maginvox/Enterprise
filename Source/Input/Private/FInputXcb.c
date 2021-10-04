#include <xcb/xcb.h>

#include "Core/FMemory.h"
#include "Input/FInput.h"
#include "Graphics/FWindow.h"

void FInputPoll()
{
    xcb_allow_events(window_xcb.pConnection, XCB_ALLOW_REPLAY_POINTER, XCB_CURRENT_TIME);
    xcb_generic_event_t* pEvent = xcb_poll_for_event(window_xcb.pConnection);
    
    if (pEvent == NULL)
    {
        return;
    }

    switch (pEvent->response_type & ~0x80)
    {
        case XCB_CLIENT_MESSAGE:
            xcb_client_message_event_t* pClientEvent = (xcb_client_message_event_t*)pEvent;

            if (pClientEvent->data.data32[0] == window_xcb.pDeleteReply->atom) {
                window_xcb.shouldClose = true;
                enDeallocate(window_xcb.pDeleteReply);
            }
            break;
    }
}