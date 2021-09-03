#include "Core/FMemory.h"
#include "Rendering/FRenderer.h"
#include "Core/FLog.h"

#ifdef ENTERPRISE_VULKAN
    #include "Rendering/Vulkan/FRenderDeviceVulkan.h"
#endif

int FRendererEventWatcher(void* pData, SDL_Event* event)
{
    FRenderer* pRenderer = (FRenderer*)pData;

    /* Check for a window resize */
    if (event->type == SDL_WINDOWEVENT && event->window.type == SDL_WINDOWEVENT_RESIZED)
    {
        SDL_Window* pWindow = SDL_GetWindowFromID(event->window.windowID);

        FInt width = 0, height = 0;
        SDL_GetWindowSize(pWindow, &width, &height);

        pRenderer->width = width;
        pRenderer->height = height;
        pRenderer->pRenderDeviceSurfaceResizeCallback(pRenderer->pRenderDevice, width, height);
    }

    return 0;
}

FRenderer* FRendererCreate(const FRenderOptions* pOptions)
{
    if (pOptions == NULL)
    {
        return NULL;
    }

    /* Create the device object */
    FRenderer* pRenderer = FAllocateZero(1, sizeof(FRenderer));
    if (pRenderer == NULL)
    {
        FRendererDestroy(&pRenderer);
        return NULL;
    }

    FMemoryCopy(pOptions, &pRenderer->options, sizeof(FRenderOptions));

    /* Initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        FRendererDestroy(&pRenderer);
        return NULL;
    }

    /* Register the event watcher */
    SDL_AddEventWatch(FRendererEventWatcher, pRenderer);

    /* Create the window */
    SDL_WindowFlags windowFlags = 0;
    windowFlags |= SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    windowFlags |= (pRenderer->options.bFullscreen) ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;

    switch (pRenderer->options.api)
    {
        case E_RENDER_API_OPENGL:
            windowFlags |= SDL_WINDOW_OPENGL;
            break;

        case E_RENDER_API_VULKAN:
            windowFlags |= SDL_WINDOW_VULKAN;
            break;
    }

    if (pRenderer->options.width == 0 || pRenderer->options.height == 0)
    {
        SDL_DisplayMode mode;
        SDL_GetCurrentDisplayMode(0, &mode);
        pRenderer->options.width = mode.w;
        pRenderer->options.height = mode.h;
    }

    pRenderer->width = pRenderer->options.width;
    pRenderer->height = pRenderer->options.height;

    pRenderer->pWindow = SDL_CreateWindow(pRenderer->options.title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pRenderer->width, pRenderer->height, windowFlags);
    if (pRenderer->pWindow == NULL)
    {
        FRendererDestroy(&pRenderer);
        return NULL;
    }

    SDL_GetWindowSize(pRenderer->pWindow, &pRenderer->width, &pRenderer->height);

    /* Create the render data */
    switch (pRenderer->options.api)
    {
        case E_RENDER_API_OPENGL:
            break;

        case E_RENDER_API_VULKAN:
            FRenderVulkanLoad(pRenderer);
            if (!FRenderDeviceVulkanCreate(pRenderer))
            {
                FRendererDestroy(&pRenderer);
                return NULL;
            }
            break;
    }

    return pRenderer;
}

void FRendererDestroy(FRenderer** ppRenderer)
{
    if (ppRenderer == NULL || *ppRenderer == NULL)
    {
        return;
    }

    FRenderer* pRenderer = *ppRenderer;

    /* Destroy the render device */
    if (pRenderer->pRenderDevice != NULL && pRenderer->pRenderDeviceDestroy != NULL)
    {
        pRenderer->pRenderDeviceDestroy(&pRenderer->pRenderDevice);
    }

    /* Destroy the window */
    if (pRenderer->pWindow != NULL)
    {
        SDL_DestroyWindow(pRenderer->pWindow);
    }

    SDL_Quit();

    /* Free the device */
    FDeallocate(pRenderer);
    *ppRenderer = NULL;
}