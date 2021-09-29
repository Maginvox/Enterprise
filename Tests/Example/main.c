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

    const FGraphicsOptions options = {0};

    FContext pContext = NULL;
    FGraphicsInitialize(pWindow, &pContext, &options);

    while(!FWindowShouldClose(pWindow))
    {
        FInputPoll();
    }

    FGraphicsUnRegisterWindow(pWindow, pContext);
    FGraphicsShutdown(&pContext);

    FWindowDestroy(pWindow);
    FWindowShutdown();
}