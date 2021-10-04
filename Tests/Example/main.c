#include "Core/enTypes.h"
#include "Graphics/enGraphics.h"
#include "Input/enInput.h"

int main(int argc, char** argv)
{
    const enWindowInfo winInfo = {"EnterpriseSandbox", 1080, 720};
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
}