#ifndef __FRHI_H__
#define __FRHI_H__

/* FRHI
	The Render Hardware Interface is designed to be a buffer between
	the gpu driver and the Renderer System. This makes is easier to 
	pass generalized commands that will create a similar or the same
	outcome on all graphics processors. */ 

#include "Rendering/FTexture.h"
#include "Rendering/"

typedef enum ERHIType
{
	E_RHI_TYPE_VULKAN,
	E_RHI_TYPE_OPENGL,
	E_RHI_TYPE_D3D12,
} ERHIType;

typedef struct FRHICreateInfo
{
	ERHIType type;
} FRHICreateInfo;

typedef FRHI* (*FRHICreateFunction)(const FRHICreateInfo* pInfo);
typedef void (*FRHIDestroyFunction)(FRHI** ppRHI);

typedef bool (*FRHISurfaceResizeCallback)(FRHI* pRHI, FInt32 width, FInt32 height);

typedef struct FRHI
{
	FRHICreateFunction pFRHICreateFunction;
	FRHIDestroyFunction pRHIDestroyFunction;

	/* GPU Commands */
	FTextureCreateFunction pTextureCreateFunction;
	FTextureDestroyFunction pTextureDestroyFunction;



	/* RHI Callbacks */
	FRHISurfaceResizeCallback pSurfaceResizeCallback;
} FRHI;

FRHI* FRHICreate(const FRHICreateInfo* pInfo);
void FRHIDestroy(FRHI** ppRHI);

#endif