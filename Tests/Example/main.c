#include "Core/FTypes.h"
#include "Window/FWindow.h"
#include "Graphics/FGraphicsContext.h"

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

    const FGraphicsOptions options =
    {
        .api = FRenderAPI_Vulkan
    };

    FGraphicsContext* pContext = FGraphicsContextCreate(pWindow, &options);
    if (pContext == NULL)
    {
        FWindowDestroy(pWindow);
        return -1;
    }

    FWindowDestroy(pWindow);
    FGraphicsContextDestroy(&pContext);
    FWindowShutdown();
}