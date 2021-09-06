#include "Graphics/FRenderAPI.h"
#include "Graphics/FImage.h"

typedef struct FRenderAPI
{
    PFN_FImageCreate pImageCreate;
    PFN_FImageDestroy pImageDestroy;
} FRenderAPI;