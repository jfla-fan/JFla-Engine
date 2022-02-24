#pragma once
#include <bit>
#include "SDL_endian.h"

#if defined( _WIN32 )
	#define ENGINE_WINDOWS_PLATFORM 1
	
	#if defined( _WIN64 )
		#define ENGINE_PLATFORM_X64 1
		#define ENGINE_PLATFORM_X32 0	
	#else
		#define ENGINE_PLATFORM_X32 1
		#define ENGINE_PLATFORM_X64 0
	#endif
#else
	#define ENGINE_WINDOWS_PLATFORM 0
#endif


#if defined( __linux__ )
	#if defined(__ANDROID__)
		#define ENGINE_LINUX_PLATFORM 0
		#define ENGINE_ANDROID_PLATFORM 1
	#else
		#define ENGINE_ANDROID_PLATFORM 0
		#define ENGINE_LINUX_PLATFORM 1
	#endif
#else
	#define ENGINE_LINUX_PLATFORM 0
#endif

#if defined( __APPLE__ )
	#define ENGINE_MACOS_PLATFORM 1
#else
	#define ENGINE_MACOS_PLATFORM 0
#endif

#if !ENGINE_WINDOWS_PLATFORM && !ENGINE_LINUX_PLATFORM && !ENGINE_MACOS_PLATFORM
#error "Unsupported platform!"
#endif


namespace UE::Platform::Details
{
	//constexpr int8 GIsX32PlatformEnvironment = (sizeof(void*) == 4);

	//constexpr int8 GIsPlatformLittleEndian = (std::endian::native == std::endian::little);
}


#if !defined( ENGINE_PLATFORM_X32 ) && !defined( ENGINE_PLATFORM_X64 )
#define ENGINE_PLATFORM_X32  (UE::Platform::Details::GIsX32PlatformEnvironment )
#define ENGINE_PLATFORM_X64 !ENGINE_PLATFORM_X32
#endif


// platform endianness

#define ENGINE_PLATFORM_LITTLE_ENDIAN ( SDL_BYTEORDER == SDL_LIL_ENDIAN )
#define ENGINE_PLATFORM_BIG_ENDIAN !ENGINE_PLATFORM_LITTLE_ENDIAN
