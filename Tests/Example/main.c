#include "Core/FTypes.h"
#include "Graphics/FGraphics.h"

int main(int argc, char** argv)
{
    const FWindowInfo winInfo = {"EnterpriseSandbox", 1080, 720};
    const FGraphicsOptions options = {0};

    FGraphicsInitialize(&winInfo, &options);

    while(!FWindowShouldClose())
    {
        FInputPoll();
    }

    FGraphicsShutdown();
}