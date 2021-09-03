#ifndef __FRENDER_DEVICE_H__
#define __FRENDER_DEVICE_H__

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "FTypes.h"
#include "Core/FMacro.h"
#include "Core/FVector.h"
#include "Resource/FAsset.h"
#include "Rendering/FVertex.h"

typedef enum ERenderAPI
{
    E_RENDER_API_OPENGL = 0,
    E_RENDER_API_VULKAN,
} ERenderAPI;

typedef struct FRenderOptions
{
    ERenderAPI api;
    FInt width;
    FInt height;
    const char title[ENTERPRISE_NAME_MAX_LENGTH];
    bool bFullscreen;
    bool bVSync;
    bool bDebugAndValidate;
    FInt physicalDeviceVulkanID;
} FRenderOptions;

typedef enum ETextureFormat
{
    E_TEXTURE_FORMAT_RGBA8 = 0,
    E_TEXTURE_FORMAT_D24S8,
} ETextureFormat;

typedef struct FTexture FTexture;
typedef struct FMesh FMesh;
typedef struct FMaterial FMaterial;
typedef struct FMaterialInstance FMaterialInstance;

typedef struct FTextureCreateOptions
{
    FInt32 width;
    FInt32 height;
    FInt32 mipLevels;
    ETextureFormat format;
    const char* pImageData;
} FTextureCreateOptions;

typedef union FMeshCreateOptions
{
    FInt verticesCount;
    FVertex* pVertices;
    FInt indicesCount;
    FInt32* pIndex;
} FMeshCreateOptions;

typedef struct FMaterialCreateOptions
{
    const FAsset* pVertexShader;
    const FAsset* pFragmentShader;
} FMaterialCreateOptions;

typedef struct FRenderDevice FRenderDevice;

typedef struct FRenderer
{
    ERenderAPI api;
    FRenderOptions options;
    SDL_Window* pWindow;
    FInt width, height;
    FRenderDevice* pRenderDevice;

    void (*pRenderDeviceDestroy)(FRenderDevice** ppRenderDevice);
    FTexture* (*pTextureCreate)(FRenderDevice* pRenderDevice, const FTextureCreateOptions* pOptions);
    void (*pTextureDestroy)(FRenderDevice* pRenderDevice, FTexture** ppTexture);
    FMesh* (*pMeshCreate)(FRenderDevice* pRenderDevice, const FMeshCreateOptions* pOptions);
    void (*pMeshDestroy)(FRenderDevice* pRenderDevice, FMesh** ppMesh);
    FMaterial* (*pMaterialCreate)(FRenderDevice* pRenderDevice, const FMaterialCreateOptions* pOptions);
    void (*pMaterialDestroy)(FRenderDevice* pRenderDevice, FMaterial** ppMaterial);
    FMaterialInstance* (*pMaterialInstanceCreate)(FRenderDevice* pRenderDevice, FMaterial* pMaterial);
    void (*pMaterialInstanceDestroy)(FRenderDevice* pRenderDevice, FMaterialInstance** ppMaterialInstance);
    void (*pMaterialInstanceSetFloat)(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, float value);
    void (*pMaterialInstanceSetVector2)(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FVector2* value);
    void (*pMaterialInstanceSetVector3)(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FVector3* value);
    void (*pMaterialInstanceSetVector4)(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FVector4* value);
    void (*pMaterialInstanceSetMatrix4)(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FMatrix4* value);
    void (*pMaterialInstanceSetTexture)(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, FTexture* pTexture);
    
    /* Callbacks */
    void (*pRenderDeviceSurfaceResizeCallback)(FRenderDevice* pRenderDevice, FInt width, FInt height);
} FRenderer;

FRenderer* FRendererCreate(const FRenderOptions* pOptions);
void FRendererDestroy(FRenderer** ppRenderDevice);
void FRendererSetOptions(FRenderer* pOptions);

FTexture* FTextureCreate(FRenderer* pRenderDevice, const FTextureCreateOptions* pOptions);
void FTextureDestroy(FRenderer* pRenderDevice, FTexture** ppTexture);

FMesh* FMeshCreate(FRenderer* pRenderDevice, const FMeshCreateOptions* pOptions);
void FMeshDestroy(FRenderer* pRenderDevice, FMesh** ppMesh);

FMaterial* FMaterialCreate(FRenderer* pRenderDevice, FMaterialCreateOptions* pOptions);
void FMaterialDestroy(FRenderer* pRenderDevice, FMaterial** ppMaterial);

FMaterialInstance* FMaterialInstanceCreate(FRenderer* pRenderDevice, FMaterial* pMaterial);
void FMaterialInstanceDestroy(FRenderer* pRenderDevice, FMaterialInstance** ppMaterialInstance);
void FMaterialInstanceSetFloat(FRenderer* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, float value);
void FMaterialInstanceSetVector2(FRenderer* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FVector2* value);
void FMaterialInstanceSetVector3(FRenderer* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FVector3* value);
void FMaterialInstanceSetVector4(FRenderer* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FVector4* value);
void FMaterialInstanceSetMatrix4(FRenderer* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FMatrix4* value);
void FMaterialInstanceSetTexture(FRenderer* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FTexture* pTexture);

#endif