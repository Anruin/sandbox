//
// Copyright (c) E. A. Pristavka, 2017
//

#pragma once

#if defined(_WIN32) && defined(_MSC_VER)
	#define CPLATFORM_WINDOWS 1
	#define strdup _strdup
#endif

#if CPLATFORM_WINDOWS
// Memory leak detection.
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include "common/errors.h"

#if defined CPLATFORM_WINDOWS
	#ifdef SANDBOX_MODULE // Building module exporting symbols
		#ifdef __GNUC__
			#define PUBLIC_SYMBOL __attribute__ ((dllexport))
		#else
			#define PUBLIC_SYMBOL __declspec(dllexport)
		#endif
	#else // Building application
		#ifdef __GNUC__
			#define PUBLIC_SYMBOL __attribute__ ((dllimport))
		#else
			#define PUBLIC_SYMBOL __declspec(dllimport)
		#endif
	#endif
	#define LOCAL_SYMBOL
#else
	#if __GNUC__ >= 4
		#define PUBLIC_SYMBOL __attribute__ ((visibility ("default"))) extern
		#define LOCAL_SYMBOL __attribute__ ((visibility ("hidden"))) extern
	#else
		#define PUBLIC_SYMBOL
		#define LOCAL_SYMBOL
	#endif
#endif

// Convert value to size_t.
//-V:SIZE:201
#define SIZE(v) (size_t)(v)

// Interprets value as string.
#define TO_CSTRING(s) #s

// Interprets symbol as string.
#define SYMBOL_TO_CSTRING(s) TO_CSTRING(s)

// Cast value to type.
#define CAST(v, type) ((type)(v))

#ifdef NDEBUG
	#ifdef _MSC_VER
		#define DEBUG_PRINT(fmt, ...)
	#else
		#define DEBUG_PRINT(fmt, args...)
	#endif
#else
	#include <stdio.h>

#include <SDL_config.h>

#ifdef CPLATFORM_WINDOWS
		// Debug log console output.
		#define DEBUG_PRINT(fmt, ...) fprintf(stderr, fmt, ## __VA_ARGS__)
	#else
		// Debug log console output.
		#define DEBUG_PRINT(fmt, args...) fprintf(stderr, fmt, ## args)
	#endif
#endif

typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

// Boolean values.
typedef int CBool;

// Destructor functions.
typedef void(*CDestructorFunc)(void *);

// Callback functions.
typedef void(*CCallbackFunc)(void *);

// Thread functions.
typedef void *(*CThreadFunc)(void *);
