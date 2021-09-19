/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FVECTOR_H__
#define __FVECTOR_H__

#include "FTypes.h"

typedef float FVector2[2];
typedef float FVector3[3];
typedef float FVector4[4];
typedef float FQuaternion[4];
typedef float FMatrix3[9];
typedef float FMatrix4[16];

FVector2* FVector2Add(float* pResult, FVector2 a, FVector2 b);
FVector2* FVector2Sub(float* pResult, FVector2 a, FVector2 b);
FVector2* FVector2Mul(float* pResult, FVector2 a, FVector2 b);
FVector2* FVector2Div(float* pResult, FVector2 a, FVector2 b);
bool FVector2IsZero(FVector2 v);
FVector2* FVector2Abs(float* pResult, FVector2 a);
FVector2* FVector2Dot(float* pResult, FVector2 a, FVector2 b);
FVector2* FVector2Min(float* pResult, FVector2 a, FVector2 b);
FVector2* FVector2Max(float* pResult, FVector2 a, FVector2 b);
FVector2* FVector2Clamp(float* pVec2, FVector2 min, FVector2 max);
FVector2* FVector2Normalize(float* pResult, FVector2 a);

FVector3* FVector3Add(float* pResult, FVector3 a, FVector3 b);
FVector3* FVector3Sub(float* pResult, FVector3 a, FVector3 b);
FVector3* FVector3Mul(float* pResult, FVector3 a, FVector3 b);
FVector3* FVector3Div(float* pResult, FVector3 a, FVector3 b);
bool FVector3IsZero(FVector3 v);
FVector3* FVector3Abs(float* pResult, FVector3 a);
FVector3* FVector3Dot(float* pResult, FVector3 a, FVector3 b);
FVector3* FVector3Min(float* pResult, FVector3 a, FVector3 b);
FVector3* FVector3Max(float* pResult, FVector3 a, FVector3 b);
FVector3* FVector3Clamp(float* pVec3, FVector3 min, FVector3 max);
FVector3* FVector3Normalize(float* pResult, FVector3 a);

FVector4* FVector4Add(float* pResult, FVector4 a, FVector4 b);
FVector4* FVector4Sub(float* pResult, FVector4 a, FVector4 b);
FVector4* FVector4Mul(float* pResult, FVector4 a, FVector4 b);
FVector4* FVector4Div(float* pResult, FVector4 a, FVector4 b);
bool FVector4IsZero(FVector4 v);
FVector4* FVector4Abs(float* pResult, FVector4 a);
FVector4* FVector4Dot(float* pResult, FVector4 a, FVector4 b);
FVector4* FVector4Min(float* pResult, FVector4 a, FVector4 b);
FVector4* FVector4Max(float* pResult, FVector4 a, FVector4 b);
FVector4* FVector4Clamp(float* pVec4, FVector4 min, FVector4 max);
FVector4* FVector4Normalize(float* pResult, FVector4 a);

FQuaternion* FQuaternionAdd(float* pResult, FQuaternion a, FQuaternion b);
FQuaternion* FQuaternionSub(float* pResult, FQuaternion a, FQuaternion b);
FQuaternion* FQuaternionMul(float* pResult, FQuaternion a, FQuaternion b);
bool FQuaternionIsZero(FQuaternion v);
FQuaternion* FQuaternionFromAxisAngle(float* pResult, FVector3 axis, float angle);
FQuaternion* FQuaternionConvertFromEuler(float* pResult, FVector3 euler);
FVector3* FQuaternionConvertToEuler(float* pResult, FQuaternion q);



#endif