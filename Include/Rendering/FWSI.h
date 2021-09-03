#ifndef __FSURFACE_H__
#define __FSURFACE_H__

/* FWSI
	The Window Surface Interface is designed to be a buffer between
	an OS, its render surface and the RHI. Therefore we can pass
	general commands from the RHI to present an image in an easy way. */

#include "FTypes.h"
#include "Rendering/FRHI.h"

/* FWSICreateInfo
	Properties may be ignored on some devices. Ex. Consoles */
typedef struct FWSICreateInfo
{
	const char* pTitle;
	FInt32 width;
	FInt32 height;
} FWSICreateInfo;

typedef struct FWSI FWSI;

FWSI* FWSICreate(FWSI* pCreateInfo);
void FWSIDestroy(FWSI** ppSurface);
bool FWSIResize(FWSI* pSurface, FInt32 width, FInt32 height);


#endif