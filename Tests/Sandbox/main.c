#include "Core/enTypes.h"
#include "Resource/enPackage.h"
#include "Graphics/enGraphics.h"
#include "Input/enInput.h"

int main(int argc, char** argv)
{
    const enWindowInfo winInfo = {"Enterprise Sandbox", 1080, 720};
    const enGraphicsOptions options = {0};

    bool firstTime = true;
    if (enPackageExists("resources.rec", "resources.pak"))
    {
        firstTime = false;
    }

    enPackage* pPackage = enPackageOpen("resources.rec", "resources.pak");
    if (pPackage == NULL)
    {
        return -1;
    }

    if (firstTime)
    {
        enPackageRecordAppend(pPackage)
    }

    if (!enGraphicsInitialize(&winInfo, &options))
    {
        return -1;
    }

    while(!enWindowShouldClose())
    {
        enInputPoll();
    }

    enGraphicsShutdown();
}