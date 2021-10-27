/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FTYPES_H__
#define __FTYPES_H__

typedef char int8;
typedef short int16;
typedef long int32;
typedef long long int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned long long uint64;
typedef unsigned short wchar;

#define ENTERPRISE_INT8_MAX 127
#define ENTERPRISE_INT8_MIN 128
#define ENTERPRISE_INT16_MAX 32767
#define ENTERPRISE_INT16_MIN -32768
#define ENTERPRISE_INT32_MAX 2147483647L
#define ENTERPRISE_INT32_MIN -2147483647L
#define ENTERPRISE_INT64_MAX 9223372036854775807LL
#define ENTERPRISE_INT64_MIN -9223372036854775807LL

#define ENTERPRISE_UINT8_MAX 255
#define ENTERPRISE_UINT16_MAX 65535
#define ENTERPRISE_UINT32_MAX 4294967295U
#define ENTERPRISE_UINT64_MAX 18446744073709551615ULL
#define ENTERPRISE_WCHAR_MAX FUINT16_MAX

#define ENTERPRISE_CHAR_BIT 8

#ifndef NULL
    #define NULL ((void*)0)
#endif

#ifndef __cplusplus
    typedef int32 bool;
    #define true 1
    #define false 0
#endif

#define KB 1024
#define ENTERPRISE_KB(x) (x * 1024)
#define MB (KB * KB)
#define ENTERPRISE_MB(x) (x * KB)
#define GB (MB * MB)
#define ENTERPRISE_GB(x) (x * MB)

#endif