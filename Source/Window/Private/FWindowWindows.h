#ifndef __FWINDOW_WINDOWS_H__
#define __FWINDOW_WINDOWS_H__

#include <Windows.h>

#include "Core/FTypes.h"

typedef struct
{
    HWND hWnd;
    bool shouldClose;

    void* pUserData;
} FWindowWindows;


#endif