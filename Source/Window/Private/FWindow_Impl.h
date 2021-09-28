#ifndef __FWINDOW_IMPL_H__
#define __FWINDOW_IMPL_H__

#include "Window/FWindow.h"

typedef struct FWindow
{
    void* pHandle;
    void* pUserData;
    bool shouldClose;

    FWindowCharacterCallback pCharacterCallback;
    FWindowMouseCallback pMouseCallback;
    FWindowMouseButtonCallback pMouseButtonCallback;
    FWindowMouseScrollCallback pMouseScrollCallback;
} FWindow;

#endif