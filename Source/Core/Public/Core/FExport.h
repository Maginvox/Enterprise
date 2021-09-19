/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FEXPORT_H__
#define __FEXPORT_H__

#if defined(_MSC_VER)
    //  Microsoft 
    #define FEXPORT __declspec(dllexport)
    #define FIMPORT __declspec(dllimport)
    #define FDEPRECATED __declspec(deprecated)
#elif defined(__GNUC__)
    //  GCC
    #define FEXPORT
    #define FIMPORT
    #define FDEPRECATED __attribute__((deprecated))
#else
    //  do nothing and hope for the best?
    #define FEXPORT
    #define FIMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

#endif