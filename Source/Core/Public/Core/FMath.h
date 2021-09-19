/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __MATH_H__
#define __MATH_H__

#include "FTypes.h"

static inline FInt64 FMathPow(FInt64 value, FInt64 power)
{
    FInt64 result = 0;

    for (FInt64 i = 0; i < power; i++)
    {
        result *= value;
    }
    return result;
}

static inline FInt64 FMathMax(FInt64 a, FInt64 b)
{
    return (a > b) ? a : b;
}

static inline FInt64 FMathMin(FInt64 a, FInt64 b)
{
    return (a < b) ? a : b;
}

static inline bool FMathIsBetween(FInt64 value, FInt64 min, FInt64 max)
{
    return (value >= min && value <= max);
}

static inline FInt64 FMathBetween(FInt64 value, FInt64 a, FInt64 b)
{
    return FMathMax(FMathMin(value, b), a);
}

static inline bool FMathIsMultipleOf8(FInt64 value)
{
    return (value % 8) == 0;
}

/* Floating point */
static inline float FFloatMathPow(float power, float value)
{
    float result = 1.0f;
    for (FInt64 i = 0; i < power; i++)
    {
        result *= value;
    }
    return result;
}

static inline float FMathFloatMax(float a, float b)
{
    return (a > b) ? a : b;
}

static inline float FMathFloatMin(float a, float b)
{
    return (a < b) ? a : b;
}

static inline bool FMathFloatIsBetween(float value, float a, float b)
{
    return (value >= a && value <= b);
}

static inline float FMathFloatBetween(float value, float a, float b)
{
    return FMathFloatMax(FMathFloatMin(value, b), a);
}

/* Double precision */
static inline double FMathDoublePow(double power, double value)
{
    double result = 1.0f;
    for (FInt64 i = 0; i < power; i++)
    {
        result *= value;
    }
    return result;
}

static inline double FMathDoubleMax(double a, double b)
{
    return (a > b) ? a : b;
}

static inline double FMathDoubleMin(double a, double b)
{
    return (a < b) ? a : b;
}

static inline bool FMathDoubleIsBetween(double value, double a, double b)
{
    return (value >= a && value <= b);
}

static inline double FMathDoubleBetween(double value, double a, double b)
{
    return FMathDoubleMax(FMathDoubleMin(value, b), a);
}

#endif