#include "Core/enArgumentParser.h"

int main(int argc, char** argv)
{

    enArgumentOption options[] = {
        {""}
        {"ManifestPath", "The path of the manifest file for all of the paths to assets.", enArgumentType_String}
    };

    enArgumentParser* argsParser = enArgumentParserCreate(COUNT_OF(options), options);
    if (argsParser == NULL)
    {
        return -1;
    }

    if (!enArgumentParserParse(argsParser, argc, argv))
    {
        enArgumentParserDestroy(argsParser);
        return -1;
    }

    enArgumentParsedOption manifestOption = {0};
    enArgumentParserGetOption(argsParser, "ManifestPath", &manifestOption);
}