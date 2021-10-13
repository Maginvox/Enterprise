#include "Core/enLog.h"
#include "Core/enString.h"
#include "Core/enArgumentParser.h"

#include "Resource/enPackage.h"

void pkShowHelp()
{
    enLogInfo("Usage: packager --[Option]=[Value] --[Option2]=[Value] ...");
    enLogInfo("Example: packager --ManifestPath=maginvox_manifest.json --OutputName=maginvox --Repack=True");
    enLogInfo("Required Options: ManifestPath:String, OutputName:String");
    enLogInfo("Optional Options: Repack:Bool");
}

int main(int argc, char** argv)
{
    char manifestPath[256];
    char outputRecordsPath[256];
    char outputDataPath[256];
    bool repack = false;

    /* Parse the passed options */
    enArgumentOption options[] = {
        {"ManifestPath", "The path of the manifest file for all of the paths to assets.", enArgumentType_String},
        {"OutputName", "The name of the package to open.", enArgumentType_String},
        {"Repack", "Repack the package at the end of adding new manifest assets.", enArgumentType_Bool}
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

    /* Verify the parsed options */
    enArgumentParsedOption manifestOption = {0};
    enArgumentParsedOption outputNameOption = {0};
    enArgumentParsedOption repackOption = {0};
    
    if (enArgumentParserGetOption(argsParser, "ManifestPath", &manifestOption) || 
        enArgumentParserGetOption(argsParser, "OutputName", &outputNameOption))
    {
        enLogError("Could not find required parameters.");
        pkShowHelp();
        enArgumentParserDestroy(argsParser);
        return -1;
    }

    /* Create our paths */
    enStringCopy(manifestOption.value.String, 256, manifestPath, 256);
    enStringCopy(outputNameOption.value.String, 256, outputRecordsPath, 256);
    enStringCopy(outputNameOption.value.String, 256, outputDataPath, 256);

    enStringConcatenate(outputRecordsPath, 256, ".recpak", sizeof(".recpak"));
    enStringConcatenate(outputDataPath, 256, ".datpak", sizeof(".datpak"));

    if (enArgumentParserGetOption(argsParser, "Repack", &repackOption))
    {
        repack = repackOption.value.Bool;
    }

    enArgumentParserDestroy(argsParser);

    if (manifestOption.type != enArgumentType_String)
    {
        return -1;
    }

    enStringCopy(manifestOption.value.String, 256, manifestPath, 256);
    
    char message[512] = "Manifest Path: ";
    enStringConcatenate(manifestOption.value.String, 256, message, 512);
    enLogInfo(message);



    return 0;
}