#pragma once

#ifdef _MSC_VER
	#define ENGINE_MSVC_COMPILER 1
#else
	#define ENGINE_MSVC_COMPILER 0
#endif

#ifdef __GNUC__
	#define ENGINE_GCC_COMPILER 1
#else
	#define ENGINE_GCC_COMPILER 0
#endif

#ifdef __clang__
#define ENGINE_CLANG_COMPILER 1
#else
#define ENGINE_CLANG_COMPILER 0
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#define ENGINE_MINGW_COMPILER 1
#else
#define ENGINE_MINGW_COMPILER 0
#endif

#ifdef __MINGW32__
#define ENGINE_MINGW_X32_COMPILER 1
#else
#define ENGINE_MINGW_X32_COMPILER 0
#endif

#ifdef __MINGW64__
#define ENGINE_MINGW_X64_COMPILER 1
#else
#define ENGINR_MINGW_X64_COMPILER 0
#endif


#if !ENGINE_MSVC_COMPILER && !ENGINE_GCC_COMPILER && !ENGINE_MINGW_COMPILER && !ENGINE_CLANG_COMPILER
#error "Unsupported compiler"
#endif