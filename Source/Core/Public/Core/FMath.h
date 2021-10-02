/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __MATH_H__
#define __MATH_H__

#include "FTypes.h"

static inline int64 FMathPow(int64 value, int64 power)
{
    int64 result = 0;

    for (int64 i = 0; i < power; i++)
    {
        result *= value;
    }
    return result;
}

static inline int64 FMathMax(int64 a, int64 b)
{
    return (a > b) ? a : b;
}

static inline int64 FMathMin(int64 a, int64 b)
{
    return (a < b) ? a : b;
}

static inline bool FMathIsBetween(int64 value, int64 min, int64 max)
{
    return (value >= min && value <= max);
}

static inline int64 FMathBetween(int64 value, int64 a, int64 b)
{
    return FMathMax(FMathMin(value, b), a);
}

static inline bool FMathIsMultipleOf8(int64 value)
{
    return (value % 8) == 0;
}

/* Floating point */
static inline float FFloatMathPow(float power, float value)
{
    float result = 1.0f;
    for (int64 i = 0; i < power; i++)
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
    for (int64 i = 0; i < power; i++)
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