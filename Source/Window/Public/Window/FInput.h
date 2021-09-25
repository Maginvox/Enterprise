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
} FInputAction;

void FInputPoll(); /* Polls the system to see if there is more messages, this must be called for any window callbacks to be activated */

#endif