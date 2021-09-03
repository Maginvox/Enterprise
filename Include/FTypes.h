#ifndef __FTYPES_H__
#define __FTYPES_H__

#ifndef __cplusplus
    typedef signed long bool;
    #define true 1
    #define false 0
#endif

typedef char FInt8;
typedef short FInt16;
typedef long FInt32;
typedef long long FInt64;
typedef unsigned char FUInt8;
typedef unsigned short FUInt16;
typedef unsigned long FUInt32;
typedef unsigned long long FUInt64;
typedef unsigned short FWChar;
typedef int FInt;
typedef unsigned int FuInt;

#define FINT8_MAX 127
#define FINT8_MIN 128
#define FINT16_MAX 32767
#define FINT16_MIN -32768
#define FINT32_MAX 2147483647L
#define FINT32_MIN -2147483647L
#define FINT64_MAX 9223372036854775807LL
#define FINT64_MIN -9223372036854775807LL

#define FUINT8_MAX 255
#define FUINT16_MAX 65535
#define FUINT32_MAX 4294967295U
#define FUINT64_MAX 18446744073709551615ULL
#define FWCHAR_MAX 65535

#define FCHAR_BIT 8

#ifndef NULL
    #define NULL ((void*)0)
#endif

#define FKB 1024
#define F_KB(x) (x * 1024)
#define FMB (FKB * FKB)
#define F_MB(x) (x * FKB)
#define FGB (FMB * FMB)
#define F_GB(x) (x * FMB)

#endif