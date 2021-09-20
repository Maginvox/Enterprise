#ifndef __FSYSTEM_H__
#define __FSYSTEM_H__

#include "Core/FBitset.h"
#include "Game/FEntity.h"

#define FSYSTEMS_MAX 512

typedef void (*FSystemOnBegin)();
typedef void (*FSystemOnEnd)();
typedef void (*FSystemOnRegister)(FEntity entity);
typedef void (*FSystemOnUnRegister)(FEntity entity);

typedef struct FSystem
{
    FSystemOnBegin pOnBegin;
    FSystemOnEnd pOnEnd;
    FSystemOnRegister pOnRegister;
    FSystemOnUnRegister pOnUnRegister;
    char entitiesAdded[FBITNSLOTS(FENTITIES_MAX)];
} FSystem;

#endif