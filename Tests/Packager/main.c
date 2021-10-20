#include "Core/enMemory.h"
#include "Core/enString.h"
#include "Core/enLog.h"
#include "Core/enArgumentParser.h"
#include "Core/enJsmn.h"


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
    uint32 manifestFileSize = (uint32)enFileTell(manifestFile);
    enFileSeek(manifestFile, 0, enSeek_Begin);

    uint8* manifestJson = enCalloc(1, manifestFileSize);
    if (!manifestJson)
    {
        enFileClose(manifestFile);
        return -1;
    }

    if (enFileRead(manifestFile, manifestJson, manifestFileSize, 1) != 1)
    {
        enLogError("Could not read the manifest file!");
        enFree(manifestJson);
        enFileClose(manifestFile);
        return -1;
    }

    enFileClose(manifestFile);

    /* Parse the manifest */
    enJsmnParser parser = {0};
    enJsmnInit(&parser);
    
    int32 manifestParseResult = enJsmnParse(&parser, manifestJson, manifestFileSize, NULL, 0);
    if (manifestParseResult < 1)
    {
        enLogError("Could not parse the manifest file.");
        enFree(manifestJson);
        return -1;
    }

    enJsmnToken* manifestTokens = enCalloc(manifestParseResult, sizeof(enJsmnToken));
    if (!manifestTokens)
    {
        enFree(manifestJson);
        return -1;
    }
    
    enJsmnInit(&parser);
    manifestParseResult = enJsmnParse(&parser, manifestJson, manifestFileSize, manifestTokens, manifestParseResult);
    if (manifestParseResult < 1)
    {
        enLogError("Could not parse the manifest file.");
        enFree(manifestTokens);
        enFree(manifestJson);
        return -1;
    }

    for (uint32 tokenIndex = 0; tokenIndex < manifestParseResult; tokenIndex++)
    {
        enJsmnToken* token = &manifestTokens[tokenIndex];

        if (enJsmnEqual(manifestJson, token, "Assets"))
        {
            tokenIndex++;
            token = &manifestTokens[tokenIndex];
            
            if (token->type != enJsmnType_Array)
            {
                enLogWarning("Malformed manifest JSON!");
                continue;
            }

            for (uint32 i = 0; i < manifestTokens[tokenIndex + 1].size; i++)
            {
                enJsmnToken* arrayToken = &manifestTokens[tokenIndex + i + 2];

                if (enJsmnEqual(manifestJson, arrayToken, "Name"))
                {
                    
                } 
                
                else if (enJsmnEqual(manifestJson, arrayToken, "Path"))
                {

                } 
                
                else if (enJsmnEqual(manifestJson, arrayToken, "Type"))
                {

                } 
                
                else
                {

                }

                /* Check if the package is the same */

                /* Read the asset file */



                continue;
            }

        }

    }

    enFree(manifestTokens);
    enFree(manifestJson);

    return 0;
}