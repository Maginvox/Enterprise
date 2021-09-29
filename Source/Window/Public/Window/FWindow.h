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

typedef void* FWindow;

FWindow FWindowCreate(const char* pTitle, FInt32 width, FInt32 height, FWindowStyle style);
void FWindowDestroy(FWindow window);
void* FWindowGetHandle(const FWindow window);
void FWindowSetUserData(FWindow window, void* pData);
void* FWindowGetUserData(const FWindow window);
void FWindowGetSize(const FWindow window, FUInt32* width, FUInt32* height);
void FWindowSetSize(const FWindow window, FUInt32 width, FUInt32 height);
bool FWindowShouldClose(const FWindow window);

/* Input callbacks */
typedef bool (*FWindowCreateCallback)(FWindow window);
typedef void (*FWindowDestroyCallback)(FWindow window);
typedef void (*FWindowResizeCallback)(FWindow window, FUInt32 width, FUInt32 height);
typedef void (*FWindowCharacterCallback)(FWindow window, FUInt32 key, FInputAction action);
typedef void (*FWindowMouseCallback)(FWindow window, double offsetx, double offsety);
typedef void (*FWindowMouseButtonCallback)(FWindow window, FInputMouseButton button, FInputAction action);
typedef void (*FWindowMouseScrollCallback)(FWindow window, double offsetx, double offsety);

void FWindowSetCreateCallback(FWindowCreateCallback pCallback);
void FWindowSetDestroyCallback(FWindowDestroyCallback pCallback);
void FWindowSetCharacterCallback(FWindow window, FWindowCharacterCallback pCallback);
void FWindowSetMouseCallback(FWindow window, FWindowMouseCallback pCallback);
void FWindowSetMouseButtonCallback(FWindow window, FWindowMouseCallback pCallback);
void FWindowSetMouseScrollCallback(FWindow window, FWindowMouseScrollCallback pCallback);


#endif