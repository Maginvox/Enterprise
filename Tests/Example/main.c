#include "Core/FTypes.h"
#include "Window/FWindow.h"
#include "Graphics/FGraphicsContext.h"

int main(int argc, char** argv)
{
    FWindowCreateInfo info = {
        "Enteprise Example",
        1080,
        720,
        FWindowStyle_Default
    };

    FWindow* pWindow = FWindowCreate(&info);
    if (pWindow == NULL)
    {
        return -1;
    }

    FGraphicsContext* pContext = FGraphicsContextCreate(FRenderAPI_Vulkan);
    if (pContext == NULL)
    {
        FWindowDestroy(&pWindow);
        return -1;
    }

    FGraphicsContextDestroy(&pContext);
    FWindowDestroy(&pWindow);
}