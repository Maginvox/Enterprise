#include "FRenderContext_Impl.h"
#include "Graphics/FRenderPass.h"


FRenderPass* FRenderPassCreate(FRenderContext* pContext, const FRenderPassCreateInfo* pInfo)
{
    if (pContext == NULL || pContext->pRenderPassCreate == NULL)
    {
        return NULL;
    }

    return pContext->pRenderPassCreate(pContext, pInfo);
}

void FRenderPassDestroy(FRenderContext* pContext, FRenderPass** ppRenderPass)
{
    if (pContext == NULL || pContext->pRenderPassDestroy == NULL || ppRenderPass == NULL || *ppRenderPass == NULL)
    {
        return;
    }

    pContext->pRenderPassDestroy(pContext, ppRenderPass);
}