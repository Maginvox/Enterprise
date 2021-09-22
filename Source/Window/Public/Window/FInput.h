#ifndef __FINPUT_H__
#define __FINPUT_H__

#include "Core/FTypes.h"

typedef enum FInputMouseButton
{
    FInputMouseButton_Left = 0,
    FInputMouseButton_Right,
    FInputMouseButton_Scroll,
    FInputMouseButton_Extra1,
    FInputMouseButton_Extra2,  
} FInputMouseButton;

typedef enum FInputAction
{
    FInputAction_Pressed = 0,
    FInputAction_Released,
    FInputAction_Repeated
} FInputAction;

typedef void (*FInputKeyboardCharacterCallback)(FWindow* pWindow, FUInt16 key); /* Reports exact character keys from the system, any keyboard layout is supported. */
typedef void (*FInputMousePositionCallback)(FWindow* pWindow, double x, double y);
typedef void (*FInputMouseButtonCallback)(FWindow* pWindow, FInputMouseButton button, FInputAction action);

void FInputPoll();

#endif