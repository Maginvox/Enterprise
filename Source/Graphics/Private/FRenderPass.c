#include "FGraphicsContext_Impl.h"
#include "Graphics/FRenderPass.h"


FRenderPass* FRenderPassCreate(FGraphicsContext* pContext, const FRenderPassCreateInfo* pInfo)
{
    if (pContext == NULL || pContext->pRenderPassCreate == NULL)
    {
        return NULL;
    }

    return pContext->pRenderPassCreate(pContext, pInfo);
}

void FRenderPassDestroy(FGraphicsContext* pContext, FRenderPass** ppRenderPass)
{
    if (pContext == NULL || pContext->pRenderPassDestroy == NULL || ppRenderPass == NULL || *ppRenderPass == NULL)
    {
        return;
    }

    pContext->pRenderPassDestroy(pContext, ppRenderPass);
}