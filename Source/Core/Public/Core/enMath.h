/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __MATH_H__
#define __MATH_H__

#include "Core/enTypes.h"

static inline int64 enMathPow(int64 value, int64 power)
{
    int64 result = 0;

    for (int64 i = 0; i < power; i++)
    {
        result *= value;
    }
    return result;
}

static inline int64 enMathMax(int64 a, int64 b)
{
    return (a > b) ? a : b;
}

static inline int64 enMathMin(int64 a, int64 b)
{
    return (a < b) ? a : b;
}

static inline bool enMathIsBetween(int64 value, int64 min, int64 max)
{
    return (value >= min && value <= max);
}

static inline int64 enMathBetween(int64 value, int64 a, int64 b)
{
    return enMathMax(enMathMin(value, b), a);
}

static inline bool enMathIsMultipleOf8(int64 value)
{
    return (value % 8) == 0;
}

/* Floating point */
static inline float enFloatMathPow(float power, float value)
{
    float result = 1.0f;
    for (int64 i = 0; i < power; i++)
    {
        result *= value;
    }
    return result;
}

static inline float enMathFloatMax(float a, float b)
{
    return (a > b) ? a : b;
}

static inline float enMathFloatMin(float a, float b)
{
    return (a < b) ? a : b;
}

static inline bool enMathFloatIsBetween(float value, float a, float b)
{
    return (value >= a && value <= b);
}

static inline float enMathFloatBetween(float value, float a, float b)
{
    return enMathFloatMax(FMathFloatMin(value, b), a);
}

/* Double precision */
static inline double enMathDoublePow(double power, double value)
{
    double result = 1.0f;
    for (int64 i = 0; i < power; i++)
    {
        result *= value;
    }
    return result;
}

static inline double enMathDoubleMax(double a, double b)
{
    return (a > b) ? a : b;
}

static inline double enMathDoubleMin(double a, double b)
{
    return (a < b) ? a : b;
}

static inline bool enMathDoubleIsBetween(double value, double a, double b)
{
    return (value >= a && value <= b);
}

static inline double enMathDoubleBetween(double value, double a, double b)
{
    return enMathDoubleMax(enMathDoubleMin(value, b), a);
}

#endif