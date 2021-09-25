#ifndef __FWINDOW_IMPL_H__
#define __FWINDOW_IMPL_H__

#include "Window/FWindow.h"

typedef struct FWindow
{
    void* pHandle;
    void* pUserData;

    FWindowCharacterCallback pCharacterCallback;
    FWindowMouseCallback pMouseCallback;
    FWindowMouseButtonCallback pMouseButtonCallback;
    FWindowMouseScrollCallback pMouseScrollCallback;
} FWindow;

extern FWindowCreateCallback pCreateCallback;
extern FWindowDestroyCallback pDestroyCallback;

#endif