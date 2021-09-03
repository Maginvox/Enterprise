#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "FTypes.h"
#include "FBitset.h"
#include "FList.h"
#include "FHashMapSimple.h"

#define ENTITIES_MAX 8192
#define COMPONENT_MAX 512
#define SYSTEMS_MAX 256

typedef FUInt64 FEntity;
typedef struct FComponent FComponent;

typedef bool (*FSystemOnBegin)(FSystem* pSystem);
typedef void (*FSystemOnEnd)(FSystem* pSystem);    
typedef void (*FSystemOnEntityAdded)(FSystem* pSystem, FEntity pEntity);
typedef void (*FSystemOnEntityRemoved)(FSystem* pSystem, FEntity pEntity);

typedef struct FSystem {
    FSystemOnBegin OnBegin;
    FSystemOnEnd OnEnd;
    FSystemOnEntityAdded OnEntityAdded;
    FSystemOnEntityRemoved OnEntityRemoved;
    char entities[FBITNSLOTS(ENTITIES_MAX)];
} FSystem;

typedef FUInt64 FComponentType;
typedef FUInt64 FSystemType;

typedef struct FComponentList
{
    FUInt16 entityToComponent[ENTITIES_MAX];
    FList* pComponents;
} FComponentList;

typedef struct FECSManager {
    char entities[FBITNSLOTS(ENTITIES_MAX)];
    FHashMapSimple* pComponentLists; /* Each element points to lists of components of that type */
    FList* pSystems;
} FECSManager;

FECSManager* FEntityComponenSystemCreate();
void FECSManagerDestroy(FECSManager** ppECSManager);
bool FECSManagerRegisterComponent(FECSManager* pECSManager, FComponentType* pComponentType, FInt64 componentSize);
bool FECSManagerRegisterSystem(FECSManager* pECSManager, FSystemType* pSystemType, FSystemOnBegin pOnBegin, FSystemOnEnd pOnEnd, FSystemOnEntityAdded pOnEntityAdded, FSystemOnEntityRemoved pOnEntityRemoved);
bool FECSManagerCreateEntity(FECSManager* pECSManager, FEntity* pEntity);
bool FECSManagerDestroyEntity(FECSManager* pECSManager, FEntity* pEntity);
bool FECSManagerAddComponent(FECSManager* pECSManager, FEntity pEntity, FComponentType componentType);
bool FECSManagerRemoveComponent(FECSManager* pECSManager, FEntity pEntity, FComponentType componentType);
bool FECSManagerHasComponent(FECSManager* pECSManager, FEntity pEntity, FComponentType componentType);
bool FECSManagerGetComponent(FECSManager* pECSManager, FEntity pEntity, FComponentType componentType, void* pComponent);


#endif