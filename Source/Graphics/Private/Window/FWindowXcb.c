#include "Graphics/FWindow.h"

FWindowXcb window_xcb = {0};

bool FWindowInitialize(const FWindowInfo* pInfo)
{
    if (pInfo->pTitle == NULL /*|| !FMathIsBetween(style, 0, FWindowStyle_Max) */)
    {
        return false;
    }

    FUInt primaryScreenIndex = 0;
    window_xcb.pConnection = xcb_connect(NULL, &primaryScreenIndex);
    if (window_xcb.pConnection == NULL)
    {
        return false;
    }

    /* Get the screens */
    xcb_screen_iterator_t iterator = xcb_setup_roots_iterator(xcb_get_setup(window_xcb.pConnection));

    /* Find the primary screen */
    for (FUInt i = 0; i < primaryScreenIndex; i++)
    {
        xcb_screen_next(&iterator);
    }

    window_xcb.pPrimaryScreen = iterator.data;

    /* Find the user screen */
    iterator = xcb_setup_roots_iterator(xcb_get_setup(window_xcb.pConnection));

    window_xcb.pScreen = window_xcb.pPrimaryScreen;

    for (FUInt i = 0; i < pInfo->screen; i++)
    {
        if (iterator.data == NULL)
        {
            window_xcb.pScreen = window_xcb.pPrimaryScreen;
            break;
        }

        window_xcb.pScreen = iterator.data;
        xcb_screen_next(&iterator);
    }

    

    window_xcb.window = xcb_generate_id(window_xcb.pConnection);
    xcb_create_window(window_xcb.pConnection, XCB_COPY_FROM_PARENT, window_xcb.window, window_xcb.pScreen->root, 0, 0, pInfo->width, pInfo->height, 4, XCB_WINDOW_CLASS_INPUT_OUTPUT, window_xcb.pScreen->root_visual, 0, NULL);

    xcb_map_window(window_xcb.pConnection, window_xcb.window);
}

void FWindowShutdown()
{
    xcb_destroy_window(window_xcb.pConnection, window_xcb.window);
    xcb_flush(window_xcb.pConnection);
}

bool FWindowShouldClose()
{

}

void FWindowGetSize(FUInt* pWidth, FUInt* pHeight)
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

xcb_window_t FWindowGetHandle()
{
    return window_xcb.window;
}

xcb_connection_t* FWindowGetConnection()
{
    return window_xcb.pConnection;
}