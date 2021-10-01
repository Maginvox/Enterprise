#ifndef __FSHADER_H__
#define __FSHADER_H__


typedef void* FShader;

FShader FShaderCreate(FAsset* pVertexShader, FAsset* pFragmentShader);
void FShaderDestroy(FShader shader);

#endif