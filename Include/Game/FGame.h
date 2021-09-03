#ifndef __FGAME_H__
#define __FGAME_H__


#include "Networking/FOnlineService.h"
#include "Rendering/FRenderer.h"


typedef struct FGame
{   
    FRenderer* pRenderDevice;
    FOnlineService* pOnlineService;
} FGame;

FGame* FGameCreate(int argc, const char* const* argv);
void FGameDestroy(FGame** ppGame);
void FGameRun(FGame* pGame);

bool FGameOpenLevel(FGame* pGame, const FAsset* pLevelAsset);
void FGameCloseLevel(FGame* pGame);
bool FGameOpenLevelStream(FGame* pGame, const FAsset* pLevelAsset);

#endif