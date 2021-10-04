#include "Windows.h"

#include "Input/enInput.h"
#include "Graphics/enWindow.h"

LRESULT enWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            window_win.shouldClose = true;
            break;
    }
 
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void enInputPoll()
{
    MSG msg;
    if (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}