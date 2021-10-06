#include <xcb/xcb_icccm.h>

#include "Core/enMemory.h"
#include "Core/enString.h"
#include "Core/enLog.h"

#include "Graphics/enWindow.h"

enWindowXcb window_xcb = {0};

bool enWindowInitialize(const enWindowInfo* pInfo)
{
    if (pInfo->pTitle == NULL /*|| !FMathIsBetween(style, 0, FWindowStyle_Max) */)
    {
        return false;
    }

    window_xcb.pConnection = xcb_connect(NULL, (int*)&window_xcb.defaultScreen);
    if (xcb_connection_has_error(window_xcb.pConnection))
    {
        enLogError("Failed to connect to X server");
        return false;
    }

    /* Get the screens */
    xcb_screen_iterator_t iterator = xcb_setup_roots_iterator(xcb_get_setup(window_xcb.pConnection));

    for (uint32 i = 0; i < window_xcb.defaultScreen; i++)
    {
        xcb_screen_next(&iterator);
    }

    window_xcb.pDefaultScreen = iterator.data;

    /* Find the user screen */
    iterator = xcb_setup_roots_iterator(xcb_get_setup(window_xcb.pConnection));

    window_xcb.pScreen = window_xcb.pDefaultScreen;

    for (uint32 i = 0; i < pInfo->screen; i++)
    {
        if (iterator.data == NULL)
        {
            window_xcb.pScreen = window_xcb.pDefaultScreen;
            enLogWarning("Could not find the specified screen!");
            break;
        }

        window_xcb.pScreen = iterator.data;
        xcb_screen_next(&iterator);
    }

    /* Create an xcb window. */
    window_xcb.window = xcb_generate_id(window_xcb.pConnection);

    uint32 valueList[] =
    {
        window_xcb.pScreen->white_pixel,
        XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_NO_EVENT
    };

    xcb_create_window(
        window_xcb.pConnection, 
        XCB_COPY_FROM_PARENT, 
        window_xcb.window, window_xcb.pScreen->root, 
        0, 0, pInfo->width, pInfo->height, 
        0, XCB_WINDOW_CLASS_INPUT_OUTPUT, 
        window_xcb.pScreen->root_visual,
        XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK, valueList);

    xcb_flush(window_xcb.pConnection);

    xcb_change_property(window_xcb.pConnection, XCB_PROP_MODE_REPLACE, window_xcb.window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, enStringLength(pInfo->pTitle, 64), pInfo->pTitle);

    /* Configure atoms to allow for a close window event. */
    char deleteWindowName[] = "WM_DELETE_WINDOW";
    xcb_intern_atom_cookie_t deleteWindowAtomCookie = xcb_intern_atom(window_xcb.pConnection, false, sizeof(deleteWindowName), deleteWindowName);
    xcb_intern_atom_reply_t* pDeleteWindowAtomReply = xcb_intern_atom_reply(window_xcb.pConnection, deleteWindowAtomCookie, NULL);

    char protocolsName[] = "WM_PROTOCOLS";
    xcb_intern_atom_cookie_t protocolsAtomCookie = xcb_intern_atom(window_xcb.pConnection, false, sizeof(protocolsName), protocolsName);
    xcb_intern_atom_reply_t* pProtocolsAtomReply = xcb_intern_atom_reply(window_xcb.pConnection, protocolsAtomCookie, NULL);

    if (pDeleteWindowAtomReply == NULL || pProtocolsAtomReply == NULL)
    {
        enWindowShutdown();
        enLogError("Could not get the WM_DELETE_WINDOW and WM_PROTOCOLS atoms!");
        return false;
    }

    xcb_icccm_set_wm_protocols(window_xcb.pConnection, window_xcb.window, pProtocolsAtomReply->atom, 1, &pDeleteWindowAtomReply->atom);
    window_xcb.deleteWindowAtom = pDeleteWindowAtomReply->atom;

    enFree(pProtocolsAtomReply);
    enFree(pDeleteWindowAtomReply);

    xcb_map_window(window_xcb.pConnection, window_xcb.window);

    xcb_flush(window_xcb.pConnection);
    return true;
}

void enWindowShutdown()
{
    xcb_destroy_window(window_xcb.pConnection, window_xcb.window);
    xcb_disconnect(window_xcb.pConnection);
}

bool enWindowShouldClose()
{
    return window_xcb.shouldClose;
}

void enWindowGetSize(uint32* pWidth, uint32* pHeight)
{
    if (pWidth == NULL || pHeight == NULL)
    {
        return;
    }

    xcb_get_geometry_cookie_t cookie = xcb_get_geometry(window_xcb.pConnection, window_xcb.window);
    xcb_get_geometry_reply_t* pReply = xcb_get_geometry_reply(window_xcb.pConnection, cookie, NULL);

    *pWidth = pReply->width;
    *pHeight = pReply->height;
}

xcb_window_t enWindowGetHandle()
{
    return window_xcb.window;
}

xcb_connection_t* enWindowGetConnection()
{
    return window_xcb.pConnection;
}