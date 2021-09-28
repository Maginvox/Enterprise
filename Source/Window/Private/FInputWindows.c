#include "FInputWindows.h"
#include "FWindow_Impl.h"
#include "Window/FInput.h"

LRESULT FWindowProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    FWindow* pWindow = (FWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch(Msg)
    {
        case WM_CLOSE:
            pWindow->shouldClose = true;
        break;
    }
 
    return DefWindowProc(hWnd, Msg, wParam, lParam);   
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