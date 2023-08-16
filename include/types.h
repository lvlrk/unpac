#pragma once

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

#if (_WIN32 || !__x86_64__) // x86
typedef unsigned long u32;
typedef signed long s32;
#else // x86-64
typedef unsigned int u32;
typedef signed int s32;
#endif

typedef unsigned long long u64;
typedef signed long long s64;
typedef float f32;
typedef double f64;
typedef long double f128;
