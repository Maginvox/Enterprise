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

    char relativePath[256] = {0};
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

    /* Get the relative path */
    enStringCopy(manifestPath, 256, relativePath, 256);
    enStringReverse(relativePath, 256);

    char* lastDot = enStringSeperate(relativePath, 256, "/\\", 2);
    if (lastDot == NULL)
    {
        enLogError("Could not find relative path.");
        return -1;
    }
    uint64 distanceFromLast = lastDot - relativePath;
    enStringReverse(relativePath, 256);
    enStringCopy(relativePath, enStringLength(relativePath, 256) - (uint32)distanceFromLast, relativePath, 256);
    
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

    enJsmnToken* t = enCalloc(manifestParseResult, sizeof(enJsmnToken));
    if (!t)
    {
        enFree(manifestJson);
        return -1;
    }
    
    enJsmnInit(&parser);
    manifestParseResult = enJsmnParse(&parser, manifestJson, manifestFileSize, t, manifestParseResult);
    if (manifestParseResult < 1)
    {
        enLogError("Could not parse the manifest file.");
        enFree(t);
        enFree(manifestJson);
        return -1;
    }

    if (manifestParseResult < 1 || t[0].type != enJsmnType_Object)
    {
        enLogError("No root object found in manifest json.");
        return -1;
    }

    /* Find the childeren of the root object */
    for (uint32 i = 1; i < (uint32)manifestParseResult; i++)
    {

        /* Check the different keys in root */
        if (enJsmnEqual(manifestJson, &t[i], "Assets"))
        {
            
            /* Assert asset type */
            if (t[i + 1].type != enJsmnType_Array)
            {
                enLogError("Assets is not an array.");
                return -1;
            }

            /* Iterate over the asset objects */
            enJsmnToken* assets = &t[i + 1];
            for (uint32 j = 0; j < (uint32)assets->size; j++)
            {
                if (t[i + j + 2].type != enJsmnType_Object)
                {
                    enLogError("Asset is not an object!");
                    return -1;
                }

                /* Iterate over the objects childeren */
                char name[256] = {0};
                char path[256] = {0};
                char type[256] = {0};

                enJsmnToken* g = &t[i + j + 2];
                for (uint32 k = 0; k < (uint32)g->size; k++)
                {

                    if (enJsmnEqual(manifestJson, &t[i + j + k + 3], "Name"))
                    {
                        i++;
                        enStringCopy(manifestJson + t[i + j + k + 3].start, t[i + j + k + 3].end - t[i + j + k + 3].start, name, 256);
                    }
                    else if (enJsmnEqual(manifestJson, &t[i + j + k + 3], "Path"))
                    {
                        i++;
                        enStringCopy(manifestJson + t[i + j + k + 3].start, t[i + j + k + 3].end - t[i + j + k + 3].start, path, 256);
                    }
                    else if (enJsmnEqual(manifestJson, &t[i + j + k + 3], "Type"))
                    {
                        i++;
                        enStringCopy(manifestJson + t[i + j + k + 3].start, t[i + j + k + 3].end - t[i + j + k + 3].start, type, 256);
                    }
                    else
                    {
                        i++;
                        enLogError("Unknown key in asset object.");
                        return -1;
                    }
                }

                i += g->size;

                /* Make sure that the asset does not already exist */
                if (enPackageExists(package, name))
                {
                    enLogInfo("Skipping found asset.");
                    continue;
                }

                /* Log the info */
                char message[1024] = {0};
                const char format[] = "\nNEW ASSET\n\tNAME: %s\n\tPATH: %s\n\tTYPE: %s";
                enStringFormatArgument arguments[3] = {
                    {.type = enStringFormatType_String, .value.String = name},
                    {.type = enStringFormatType_String, .value.String = path},
                    {.type = enStringFormatType_String, .value.String = type}
                };
                enStringFormat(message, sizeof(message), format, arguments, COUNT_OF(arguments));
                enLogInfo(message);

                /* Get the assets formal type */
                enAssetType assetType = enAssetType_None;
                if (enStringCompare(name, "None", sizeof("None")))
                {
                    assetType = enAssetType_None;                    
                }
                else if (enStringCompare(name, "Texture", sizeof("Texture")))
                {
                    assetType = enAssetType_Texture;
                }
                else if (enStringCompare(name, "Shader", sizeof("Shader")))
                {
                    assetType = enAssetType_Shader;
                }
                else if (enStringCompare(name, "Model", sizeof("Model")))
                {
                    assetType = enAssetType_Model;
                }
                else if (enStringCompare(name, "Audio", sizeof("Audio")))
                {
                    assetType = enAssetType_Audio;
                }
                else
                {
                    enLogWarning("Unknown asset type.");
                }

                /* Get the assets path */
                char actualPath[256] = {0};
                enStringCopy(relativePath, enStringLength(relativePath, sizeof(relativePath)), actualPath, sizeof(actualPath));
                enStringConcatenate(path, enStringLength(path, sizeof(path)), actualPath, sizeof(actualPath));

                /* Read the asset data */
                enFile* assetFile = enFileOpen(actualPath, "rb");
                if (!assetFile)
                {
                    enLogError("Could not open asset file!");
                    continue;
                }

                enFileSeek(assetFile, 0, enSeek_End);
                uint32 assetFileSize = (uint32)enFileTell(assetFile);
                enFileSeek(assetFile, 0, enSeek_Begin);

                uint8* assetData = enCalloc(assetFileSize, sizeof(uint8));
                if (!assetData)
                {
                    enLogError("Could not allocate memory for asset data!");
                    enFileClose(assetFile);
                    continue;
                }

                if (!enFileRead(assetFile, assetData, assetFileSize, 1))
                {
                    enLogError("Could not read asset data!");
                    enFileClose(assetFile);
                    enFree(assetData);
                    continue;
                }

                enFileClose(assetFile);

                /* Add the asset to the package */
                if (!enPackageAdd(package, name, assetType, assetFileSize, assetData))
                {
                    enFree(assetData);
                    enLogError("Could not add asset to package!");
                    continue;
                }

                enFree(assetData);
            }

            i += t[i + 1].size + 1;
        }
    }

    enFree(t);
    enFree(manifestJson);

    enPackageClose(package);

    return 0;
}