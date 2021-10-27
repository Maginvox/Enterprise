#include "Core/enMemory.h"
#include "Core/enString.h"
#include "Core/enLog.h"

#include "Graphics/enWindow.h"

enWindowXcb win = {0};

bool enWindowInitialize(const enWindowInfo* pInfo)
{
    /* Connect to X11 */
    win.connection = xcb_connect(NULL, &win.defaultIndex);
    if (xcb_connection_has_error(win.connection))
    {
        return false;
    }

    xcb_screen_iterator_t iterator = xcb_setup_roots_iterator(xcb_get_setup(win.connection));
    for (uint32 i = 0; i < win.defaultIndex; i++)
    {
        xcb_screen_next(&iterator);
    }

    /* Create the X window */
    xcb_screen_t* defaultScreen = iterator.data;
    win.window = xcb_generate_id(win.connection);

    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t values[] =
    {
        defaultScreen->white_pixel,
        defaultScreen->black_pixel,
        XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_STRUCTURE_NOTIFY
    };

    xcb_create_window(
        win.connection, XCB_COPY_FROM_PARENT,
        win.window, defaultScreen->root,
        0, 0, pInfo->width, pInfo->height,
        1, XCB_WINDOW_CLASS_INPUT_OUTPUT,
        defaultScreen->root_visual,
        mask, values);

    /* Get the window close atoms */
    char protocolsName[] = "WM_PROTOCOLS";
    xcb_intern_atom_cookie_t protocolsCookie = xcb_intern_atom(win.connection, true, sizeof(protocolsName) - 1, protocolsName);
    xcb_intern_atom_reply_t* protocolsReply = xcb_intern_atom_reply(win.connection, protocolsCookie, NULL);

    char deleteName[] = "WM_DELETE_WINDOW";
    xcb_intern_atom_cookie_t deleteCookie = xcb_intern_atom(win.connection, false, sizeof(deleteName) - 1, deleteName);
    xcb_intern_atom_reply_t* deleteReply = xcb_intern_atom_reply(win.connection, deleteCookie, NULL);

    if (protocolsReply == NULL || deleteReply == NULL)
    {
        enWindowShutdown();
        enLogError("Could not get the window protocols/delete atoms!");
        return false;
    }

    xcb_change_property(win.connection, XCB_PROP_MODE_REPLACE, win.window, protocolsReply->atom, XCB_ATOM_ATOM, 32, 1, &deleteReply->atom);
    win.deleteAtom = deleteReply->atom;

    enFree(protocolsReply);
    enFree(deleteReply);

    /* Map the window and flush */
    xcb_map_window(win.connection, win.window);
    xcb_flush(win.connection);
}

void enWindowShutdown()
{
    xcb_destroy_window(win.connection, win.window);
    xcb_disconnect(win.connection);
}

bool enWindowShouldClose()
{
    return win.close;
}

void enWindowGetSize(uint32* pWidth, uint32* pHeight)
{
    if (pWidth == NULL || pHeight == NULL)
    {
        return;
    }

    xcb_get_geometry_cookie_t cookie = xcb_get_geometry(win.connection, win.window);
    xcb_get_geometry_reply_t* pReply = xcb_get_geometry_reply(win.connection, cookie, NULL);

    *pWidth = pReply->width;
    *pHeight = pReply->height;
}

xcb_window_t enWindowGetHandle()
{
    return win.window;
}

xcb_connection_t* enWindowGetConnection()
{
    return win.connection;
}