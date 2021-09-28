#ifndef __FWINDOW_H__
#define __FWINDOW_H__

#include "Core/FTypes.h"
#include "FInput.h"

typedef enum FWindowStyle
{
    FWindowStyle_Default, /* All normal buttons and styling for the system */
    FWindowStyle_Minimal, /* No buttons or styling, only a surface */
    FWindowStyle_Max = FWindowStyle_Minimal,
} FWindowStyle;

bool FWindowInitialize();
void FWindowShutdown();
void* FWindowSystemHandle();

typedef struct FWindow FWindow;

FWindow* FWindowCreate(const char* pTitle, FInt32 width, FInt32 height, FWindowStyle style);
void FWindowDestroy(FWindow* pWindow);
void* FWindowGetHandle(const FWindow* pWindow);
void FWindowSetUserData(FWindow* pWindow, void* pData);
void* FWindowGetUserData(const FWindow* pWindow);
void FWindowGetSize(const FWindow* pWindow, FUInt32* width, FUInt32* height);
void FWindowSetSize(const FWindow* pWindow, FUInt32 width, FUInt32 height);
bool FWindowShouldClose(const FWindow* pWindow);

/* Input callbacks */
typedef bool (*FWindowCreateCallback)(FWindow* pWindow);
typedef void (*FWindowDestroyCallback)(FWindow* pWindow);
typedef void (*FWindowResizeCallback)(FWindow* pWindow, FUInt32 width, FUInt32 height);
typedef void (*FWindowCharacterCallback)(FWindow* pWindow, FUInt32 key, FInputAction action);
typedef void (*FWindowMouseCallback)(FWindow* pWindow, double offsetx, double offsety);
typedef void (*FWindowMouseButtonCallback)(FWindow* pWindow, FInputMouseButton button, FInputAction action);
typedef void (*FWindowMouseScrollCallback)(FWindow* pWindow, double offsetx, double offsety);

void FWindowSetCreateCallback(FWindowCreateCallback pCallback);
void FWindowSetDestroyCallback(FWindowDestroyCallback pCallback);
void FWindowSetCharacterCallback(FWindow* pWindow, FWindowCharacterCallback pCallback);
void FWindowSetMouseCallback(FWindow* pWindow, FWindowMouseCallback pCallback);
void FWindowSetMouseButtonCallback(FWindow* pWindow, FWindowMouseCallback pCallback);
void FWindowSetMouseScrollCallback(FWindow* pWindow, FWindowMouseScrollCallback pCallback);


#endif