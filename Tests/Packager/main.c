#include "Core/enMemory.h"
#include "Core/enString.h"
#include "Core/enLog.h"
#include "Core/enArgumentParser.h"
#include "Core/enJsonParser.h"


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
    char manifestPath[256] = {0};
    char outputRecordsPath[256] = {0};
    char outputDataPath[256] = {0};
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
    
    if (!enArgumentParserGetOption(argsParser, "ManifestPath", &manifestOption) ||
        manifestOption.type != enArgumentType_String ||
        !enArgumentParserGetOption(argsParser, "OutputName", &outputNameOption) ||
        outputNameOption.type != enArgumentType_String)
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

    enStringConcatenate(".recpak", sizeof(".recpak"), outputRecordsPath, 256);
    enStringConcatenate(".datpak", sizeof(".datpak"), outputDataPath, 256);

    if (enArgumentParserGetOption(argsParser, "Repack", &repackOption))
    {
        repack = repackOption.value.Bool;
    }

    enArgumentParserDestroy(argsParser);

    if (manifestOption.type != enArgumentType_String)
    {
        return -1;
    }

    char message[512] = "Manifest Path: ";
    enStringConcatenate(manifestOption.value.String, 256, message, 512);
    enLogInfo(message);

    /* Read the manifest JSON */
    enFile* manifestFile = enFileOpen(manifestPath, "rb");
    if (!manifestFile)
    {
        enLogError("Manifest file path invalid.");
        return -1;
    }

    enFileSeek(manifestFile, 0, enSeek_End);
    uint32 manifestFileSize = enFileTell(manifestFile);
    enFileSeek(manifestFile, 0, enSeek_Begin);

    uint8* manifestFileBuffer = enCalloc(1, manifestFileSize);
    if (!manifestFileBuffer)
    {
        enFileClose(manifestFile);
        return -1;
    }

    if (enFileRead(manifestFile, manifestFileBuffer, manifestFileSize, 1) != 1)
    {
        enLogError("Could not read the manifest file!");
        enFree(manifestFileBuffer);
        enFileClose(manifestFile);
        return -1;
    }

    /* Parse the manifest */
    int32 manifestParseResult = enJsonParse(manifestFileBuffer, manifestFileSize, 0, NULL);
    if (manifestParseResult < 1)
    {
        enLogError("Could not parse the manifest file.");
        enFree(manifestFileBuffer);
        enFileClose(manifestFile);
        return -1;
    }

    enJsonToken* manifestTokens = enCalloc(manifestParseResult, sizeof(enJsonToken));
    if (!manifestTokens)
    {
        enFree(manifestFileBuffer);
        enFileClose(manifestFile);
        return -1;
    }

    manifestParseResult = enJsonParse(manifestFileBuffer, manifestFileSize, manifestParseResult, manifestTokens);
    if (manifestParseResult < 1)
    {
        enLogError("Could not parse the manifest file.");
        enFree(manifestTokens);
        enFree(manifestFileBuffer);
        enFileClose(manifestFile);
        return -1;
    }

    return 0;
}