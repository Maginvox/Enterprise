#include "Core/enMemory.h"
#include "Core/enLog.h"
#include "Core/enJsmn.h"

#include "enMeshParserGLTF.h"

#define GLTF_MAGIC 0x46546C67
#define GLTF_VERSION 2
#define GLTF_TYPE_JSON 0x4E4F534A
#define GLTF_TYPE_BIN 0x004E4942

typedef struct enGLTFHeader
{
    uint32 magicNumber;
    uint32 version;
    uint32 length;
} enGLTFHeader;

typedef struct enGLTFChunk
{
    uint32 chunkLength;
    uint32 chunkType;
} enGLTFChunk;

bool enMeshParserGLTF(enAsset* pAsset, enMesh* pMesh)
{
    const char* pData = pAsset->pData;

    /* Read for header */
    if (pAsset->size < sizeof(enGLTFHeader) + sizeof(enGLTFChunk))
    {
        return false; /* There must be a header and chunk */
    }

    enGLTFHeader* pHeader = (enGLTFHeader*)pData; 
    if (pHeader->magicNumber != GLTF_MAGIC || pHeader->version != GLTF_VERSION)
    {
        return false; /* Must be correct magic number and version */
    }
    pData += sizeof(enGLTFHeader);

    /* Read the first JSON chunk */
    enGLTFChunk* pJSONChunk = (enGLTFChunk*)pData;
    if (pJSONChunk->chunkLength <= 0 || pJSONChunk->chunkType != GLTF_TYPE_JSON)
    {
        return false; /* First chunk must be json and greater than zero */
    }
    pData += sizeof(enGLTFChunk);

    const char* pJSON = pData; /* We can only read up to pJSONChunk->chunkLength */

    /* Parse the GLTF Json */
    enJsmnParser parser;
    enJsmnInit(&parser);

    uint32 tokensCount = enJsmnParse(&parser, pJSON, pJSONChunk->chunkLength, NULL, 0);
    if (tokensCount <= 0)
    {
        return false; /* Found no JSON tokens or was invalid */
    }

    enJsmnToken* pTokens = enCalloc(tokensCount, sizeof(enJsmnToken));
    if (pTokens == NULL)
    {
        return false;
    }

    enJsmnInit(&parser);
    tokensCount = enJsmnParse(&parser, pJSON, pJSONChunk->chunkLength, pTokens, tokensCount);
    if (tokensCount <= 0)
    {
        enFree(pTokens);
        return false; /* Found no JSON tokens or was invalid */
    }

    

}