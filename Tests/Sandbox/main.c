#include "Core/enTypes.h"
#include "Resource/enPackage.h"
#include "Graphics/enGraphics.h"
#include "Input/enInput.h"

int main(int argc, char** argv)
{
    enPackage* pPackage = enPackageOpen("resources.rec", "resources.pak", false);
    if (pPackage == NULL)
    {
        return -1;
    }

    const enWindowInfo winInfo = {"Enterprise Sandbox", 1080, 720};
    const enGraphicsOptions options = {0};

    if (!enGraphicsInitialize(&winInfo, &options))
    {
        return -1;
    }

    while(!enWindowShouldClose())
    {
        enInputPoll();
    }

    enGraphicsShutdown();

    enPackageClose(pPackage);
}