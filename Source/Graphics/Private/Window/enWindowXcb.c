#include "Core/FMemory.h"
#include "Core/FString.h"
#include "Graphics/FWindow.h"

/* I'm having some troubles with XCB sending client events and getting the close event. 
   XLib has a close event while XCB doesn't for some reason. I've attempted to get
   some kind of event that triggers a close but to no success. I cant any events actually. */

FWindowXcb window_xcb = {0};

bool enWindowInitialize(const enWindowInfo* pInfo)
{
    if (pInfo->pTitle == NULL /*|| !FMathIsBetween(style, 0, FWindowStyle_Max) */)
    {
        return false;
    }

    int primaryScreenIndex = 0;
    window_xcb.pConnection = xcb_connect(NULL, &primaryScreenIndex);
    if (window_xcb.pConnection == NULL)
    {
        return false;
    }

    /* Get the screens */
    xcb_screen_iterator_t iterator = xcb_setup_roots_iterator(xcb_get_setup(window_xcb.pConnection));

    /* Find the primary screen */
    for (uint32 i = 0; i < primaryScreenIndex; i++)
    {
        xcb_screen_next(&iterator);
    }

    window_xcb.pPrimaryScreen = iterator.data;

    /* Find the user screen */
    iterator = xcb_setup_roots_iterator(xcb_get_setup(window_xcb.pConnection));

    window_xcb.pScreen = window_xcb.pPrimaryScreen;

    for (uint32 i = 0; i < pInfo->screen; i++)
    {
        if (iterator.data == NULL)
        {
            window_xcb.pScreen = window_xcb.pPrimaryScreen;
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
        XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_STRUCTURE_NOTIFY
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
    xcb_intern_atom_cookie_t protocolsCookie = xcb_intern_atom(window_xcb.pConnection, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_reply_t* pProtocolsReply = xcb_intern_atom_reply(window_xcb.pConnection, protocolsCookie, 0);
    xcb_intern_atom_cookie_t deleteCookie = xcb_intern_atom(window_xcb.pConnection, 0, 16, "WM_DELETE_WINDOW");
    window_xcb.pDeleteReply = xcb_intern_atom_reply(window_xcb.pConnection, deleteCookie, 0);
    xcb_change_property(window_xcb.pConnection, XCB_PROP_MODE_REPLACE, window_xcb.window, pProtocolsReply->atom, 4, 32, 1, &pProtocolsReply->atom);
    enFree(pProtocolsReply);

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

xcb_connection_t* FWindowGetDisplay()
{

}

xcb_window_t enWindowGetHandle()
{
    return window_xcb.window;
}

xcb_connection_t* FWindowGetConnection()
{
    return window_xcb.pConnection;
}