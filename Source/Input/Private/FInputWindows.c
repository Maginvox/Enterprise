#include "Windows.h"

#include "Input/FInput.h"

#include "Graphics/FWindow.h"

LRESULT FWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            window_win.shouldClose = true;
            break;
    }
 
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void FInputPoll()
{
    MSG msg;
    if (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}