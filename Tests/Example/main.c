#include "Game/FGame.h"

int main(int argc, char** argv)
{
    
    FGame* pExampleGame = FGameCreate(argc, (const char* const*)argv);
    if (pExampleGame == NULL)
    {
        return -1;
    }

    FGameRun(pExampleGame);
    FGameDestroy(&pExampleGame);
}