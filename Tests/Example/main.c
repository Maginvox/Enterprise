#include "Core/FTypes.h"
#include "Graphics/FGraphics.h"
#include "Input/FInput.h"

int main(int argc, char** argv)
{
    const FWindowInfo winInfo = {"EnterpriseSandbox", 1080, 720};
    const FGraphicsOptions options = {0};

    if (!FGraphicsInitialize(&winInfo, &options))
    {
        return -1;
    }

    while(!FWindowShouldClose())
    {
        FInputPoll();
    }

    FGraphicsShutdown();
}