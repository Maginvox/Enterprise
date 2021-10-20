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

    /* Open the package */
    enPackage* package = enPackageOpen(outputRecordsPath, outputDataPath);
    if (!package)
    {
        enLogError("Could not open package.");
        enArgumentParserDestroy(argsParser);
        return -1;
    }

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

    for (int32 tokenIndex = 0; tokenIndex < manifestParseResult; tokenIndex++)
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
                char name[256] = {0};
                char path[256] = {0};
                char type[256] = {0};

                if (enJsmnEqual(manifestJson, &manifestTokens[tokenIndex + i + 2], "Name"))
                {
                    tokenIndex++;
                    enStringCopy(manifestJson + manifestTokens[tokenIndex + i + 2].start, manifestTokens[tokenIndex + i + 2].end - manifestTokens[tokenIndex + i + 2].start, name, 256);
                    tokenIndex++;
                }
                
                if (enJsmnEqual(manifestJson, &manifestTokens[tokenIndex + i + 2], "Path"))
                {
                    tokenIndex++;
                    enStringCopy(manifestJson + manifestTokens[tokenIndex + i + 2].start, manifestTokens[tokenIndex + i + 2].end - manifestTokens[tokenIndex + i + 2].start, path, 256);
                    tokenIndex++;
                }
                
                if (enJsmnEqual(manifestJson, &manifestTokens[tokenIndex + i + 2], "Type"))
                {
                    tokenIndex++;
                    enStringCopy(manifestJson + manifestTokens[tokenIndex + i + 2].start, manifestTokens[tokenIndex + i + 2].end - manifestTokens[tokenIndex + i + 2].start, type, 256);
                    tokenIndex++;
                }

                /* Check if there are any same assets */
                if (enPackageExists(package, name))
                {
                    enLog("SKIP", name, "Already found.");
                    continue;
                }
                
                /* Log the information we gotten */
                enStringFormatArgument assetLogFmtArgs[] = 
                {
                    {
                        .type = enStringFormatType_String,
                        .value.String = name
                    },
                    {
                        .type = enStringFormatType_String,
                        .value.String = path
                    },
                    {
                        .type = enStringFormatType_String,
                        .value.String = type
                    }
                };
                char assetLogFormat[] = "NEW ASSET\n\tName: %s\n\tPath: %s\n\tType: %s";
                char assetLogMessage[512] = {0};
                enStringFormat(assetLogMessage, sizeof(assetLogMessage), assetLogFormat, assetLogFmtArgs, COUNT_OF(assetLogFmtArgs));
                enLogInfo(assetLogMessage);


                /* Get the proper type */
                enAssetType assetType = enAssetType_None;
                if (enStringCompare(type, "None", sizeof("None")) == 0)
                {
                    assetType = enAssetType_None;
                } else if (enStringCompare(type, "Texture", sizeof("Texture")) == 0)
                {
                    assetType = enAssetType_Texture;
                } else if (enStringCompare(type, "Model", sizeof("Model")) == 0)
                {
                    assetType = enAssetType_Model;
                } else if (enStringCompare(type, "Audio", sizeof("Audio")) == 0)
                {
                    assetType = enAssetType_Audio;
                }
                else
                {
                    enLogWarning("Unknown asset type!");
                }

                /* Read the asset file */
                enFile* assetFile = enFileOpen(path, "rb");
                if (!assetFile)
                {
                    enLogError("Asset file path invalid.");
                    continue;
                }

                enFileSeek(assetFile, 0, enSeek_End);
                uint32 assetFileSize = (uint32)enFileTell(assetFile);
                enFileSeek(assetFile, 0, enSeek_Begin);

                uint8* assetData = enCalloc(1, assetFileSize);
                if (!assetData)
                {
                    enFileClose(assetFile);
                    continue;
                }

                if (enFileRead(assetFile, assetData, assetFileSize, 1) != 1)
                {
                    enLogError("Could not read the asset file!");
                    enFree(assetData);
                    enFileClose(assetFile);
                    continue;
                }

                enFileClose(assetFile);

                /* Create the asset */
                if (!enPackageAdd(package, name, assetType, assetFileSize, assetFile))
                {
                    enLogError("Could not add the asset to the package!");
                    enFree(assetData);
                    continue;
                }
            }
        }
    }

    enFree(manifestTokens);
    enFree(manifestJson);

    return 0;
}