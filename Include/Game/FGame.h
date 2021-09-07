#ifndef __FGAME_H__
#define __FGAME_H__


#include "Resource/FAsset.h"
#include "Networking/FOnlineService.h"

typedef struct FGame
{   
    FOnlineService* pOnlineService;
} FGame;

FGame* FGameCreate(int argc, const char* const* argv);
void FGameDestroy(FGame** ppGame);
void FGameRun(FGame* pGame);

bool FGameOpenLevel(FGame* pGame, const FAsset* pLevelAsset);
void FGameCloseLevel(FGame* pGame);
bool FGameOpenLevelStream(FGame* pGame, const FAsset* pLevelAsset);

#endif