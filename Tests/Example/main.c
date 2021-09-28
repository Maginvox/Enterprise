#include "Core/FTypes.h"
#include "Window/FWindow.h"
#include "Window/FInput.h"
#include "Graphics/FGraphics.h"

int main(int argc, char** argv)
{
    if (!FWindowInitialize())
    {
        return -1;
    }
    
    FWindow* pWindow = FWindowCreate("Enterprise Example", 1080, 720, FWindowStyle_Default);
    if (pWindow == NULL)
    {
        return -1;
    }

    FWindow* pWindow2 = FWindowCreate("Enterprise Example Window 2", 1080, 720, FWindowStyle_Default);
    if (pWindow2 == NULL)
    {   
        FWindowDestroy(pWindow);
        return -1;
    }

    const FGraphicsOptions options = {0};

    FContext pContext = NULL;
    FGraphicsInitialize(pWindow, &pContext, &options);

    FContext* pContext2 = FGraphicsRegisterWindow(pWindow2);
    bool bWindow2Open = true;

    while(!FWindowShouldClose(pWindow))
    {
        FInputPoll();
        if (FWindowShouldClose(pWindow2) && bWindow2Open)
        {
            FGraphicsUnRegisterWindow(pWindow2, pContext2);
            FWindowDestroy(pWindow2);
            bWindow2Open = false;
        }
    }

    if (bWindow2Open)
    {
        FGraphicsUnRegisterWindow(pWindow2, pContext2);
        FWindowDestroy(pWindow2);
    }

    FGraphicsUnRegisterWindow(pWindow, pContext);
    FGraphicsShutdown(&pContext);

    FWindowDestroy(pWindow);
    FWindowShutdown();
}