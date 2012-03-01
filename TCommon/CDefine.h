#pragma once

#include <assert.h>
#include <wchar.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <map>
#include <list>

#include "CVector.h"

// CRT's memory leak detection
#if defined(DEBUG) || defined(_DEBUG)
#include <crtdbg.h>
//#include <vld.h>  // use only when memory leak detected
#endif

#define ENUMSTR(a) (#a) 

//Unsigned types.
typedef unsigned __int64	uint64;
typedef unsigned int		uint32;
typedef unsigned short		uint16;
typedef unsigned char		ubyte;
typedef unsigned char		uint8;

//Signed types
typedef __int64 int64;
typedef int				int32;
typedef short			int16;
typedef unsigned char	byte;
typedef char			int8;

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

#define BIT(x) (1<<(x))

template <class T> void TSWAP ( T& a, T& b )	{	T c(a); a=b; b=c;	}

#include "CMathType.h"