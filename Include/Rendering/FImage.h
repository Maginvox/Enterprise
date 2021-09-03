#ifndef __FIMAGE_H__
#define __FIMAGE_H__

/* FImage
    The image is an abstraction of a multipurpose image for
    renderpasses, textures and other functions. */

#include "FTypes.h"

typedef struct FRenderSystem FRenderSystem;

typedef enum FImageFormat
{
    E_IMAGE_FORMAT_RGBA8,
} FImageFormat;

typedef struct FImageCreateInfo
{
    FInt32 width;
    FInt32 height;
    FInt32 mipLevels;
    EImageFormat format;
    const void* pData;
} FImageCreateInfo;

typedef struct FImage FImage;

typedef FImage* (*FImageCreateFunction)(FRenderSystem* pRenderSystem, const FImageCreateInfo* pInfo);
typedef void (*FImageDestroyFunction)(FRenderSystem* pRenderSystem, FImage** ppImage);
typedef bool (*FImageUpdateFunction)(FRenderSystem* pRenderSystem, FImage* pImage, const char* pData);

FImage* FImageCreate(FRenderSystem* pRenderSystem, const FImageCreateInfo* pInfo);
void FImageDestroy(FRenderSystem* pRenderSystem, FImage** ppImage);
bool FImageUpdate(FRenderSystem* pRenderSystem, FImage* pImage, const void* pData); /* pData must be the same size as (width * height * 4) */

#endif