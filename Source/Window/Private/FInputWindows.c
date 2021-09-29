#include "FInputWindows.h"
#include "Window/FInput.h"
#include "FWindowWindows.h"

LRESULT FWindowProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    FWindowWindows* pWindow = (FWindowWindows*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

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