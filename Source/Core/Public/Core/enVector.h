/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __ENTERPRISE_VECTOR_H__
#define __ENTERPRISE_VECTOR_H__

#include "Core/enTypes.h"

typedef float enVector2[2];
typedef float enVector3[3];
typedef float enVector4[4];
typedef float enQuaternion[4];

enVector2*  enVector2Add(float* pResult, enVector2 a, enVector2 b);
enVector2*  enVector2Sub(float* pResult, enVector2 a, enVector2 b);
enVector2*  enVector2Mul(float* pResult, enVector2 a, enVector2 b);
enVector2*  enVector2Div(float* pResult, enVector2 a, enVector2 b);
bool        enVector2IsZero(enVector2 v);
enVector2*  enVector2Abs(float* pResult, enVector2 a);
enVector2*  enVector2Dot(float* pResult, enVector2 a, enVector2 b);
enVector2*  enVector2Min(float* pResult, enVector2 a, enVector2 b);
enVector2*  enVector2Max(float* pResult, enVector2 a, enVector2 b);
enVector2*  enVector2Clamp(float* pVec2, enVector2 min, enVector2 max);
enVector2*  enVector2Normalize(float* pResult, enVector2 a);

enVector3*  enVector3Add(float* pResult, enVector3 a, enVector3 b);
enVector3*  enVector3Sub(float* pResult, enVector3 a, enVector3 b);
enVector3*  enVector3Mul(float* pResult, enVector3 a, enVector3 b);
enVector3*  enVector3Div(float* pResult, enVector3 a, enVector3 b);
bool        enVector3IsZero(enVector3 v);
enVector3*  enVector3Abs(float* pResult, enVector3 a);
enVector3*  enVector3Dot(float* pResult, enVector3 a, enVector3 b);
enVector3*  enVector3Min(float* pResult, enVector3 a, enVector3 b);
enVector3*  enVector3Max(float* pResult, enVector3 a, enVector3 b);
enVector3*  enVector3Clamp(float* pVec3, enVector3 min, enVector3 max);
enVector3*  enVector3Normalize(float* pResult, enVector3 a);

enVector4*  enVector4Add(float* pResult, enVector4 a, enVector4 b);
enVector4*  enVector4Sub(float* pResult, enVector4 a, enVector4 b);
enVector4*  enVector4Mul(float* pResult, enVector4 a, enVector4 b);
enVector4*  enVector4Div(float* pResult, enVector4 a, enVector4 b);
bool        enVector4IsZero(enVector4 v);
enVector4*  enVector4Abs(float* pResult, enVector4 a);
enVector4*  enVector4Dot(float* pResult, enVector4 a, enVector4 b);
enVector4*  enVector4Min(float* pResult, enVector4 a, enVector4 b);
enVector4*  enVector4Max(float* pResult, enVector4 a, enVector4 b);
enVector4*  enVector4Clamp(float* pVec4, enVector4 min, enVector4 max);
enVector4*  enVector4Normalize(float* pResult, enVector4 a);

enQuaternion*   enQuaternionAdd(float* pResult, enQuaternion a, enQuaternion b);
enQuaternion*   enQuaternionSub(float* pResult, enQuaternion a, enQuaternion b);
enQuaternion*   enQuaternionMul(float* pResult, enQuaternion a, enQuaternion b);
bool            enQuaternionIsZero(enQuaternion v);
enQuaternion*   enQuaternionFromAxisAngle(float* pResult, enVector3 axis, float angle);
enQuaternion*   enQuaternionConvertFromEuler(float* pResult, enVector3 euler);
enVector3*      enQuaternionConvertToEuler(float* pResult, enQuaternion q);

#endif