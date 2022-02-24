#pragma once
#include <cstdint>
#include <cstddef>


namespace UE::Platform
{

	// signed types

	// 8-bit signed integer
	using int8				= std::int8_t;

	// 16-bit signed integer
	using int16				= std::int16_t;

	// 32-bit signed integer
	using int32				= std::int32_t;

	// 64-bit signed integer
	using int64				= std::int64_t;


	// unsigned types

	// 8-bit unsigned integer
	using uint8				= std::uint8_t;

	// 16-bit unsigned integer
	using uint16			= std::uint16_t;

	// 32-bit unsigned integer
	using uint32			= std::uint32_t;

	// 64-bit unsigned integer
	using uint64			= std::uint64_t;

	// others types

	// unsigned int
	using SIZE_T			= std::uint64_t;

	// should be at least 8 bytes
	using byte				= std::byte;

	// An ANSI character. 8-bit fixed-width representation of 7-bit character
	using ANSICHAR			= char;

	// A wide character. ?-bit fixed-width representation of the platform's natural wide character set. Could be different sizes on different platforms. 
	using UNICHAR			= wchar_t;

	// unsigned char
	using UCHAR				= unsigned char;

	// 8-bit character type
	using CHAR8				= unsigned char;

	// 16-bit character type
	using CHAR16			= char16_t;

	// 32-bit character type
	using CHAR32			= char32_t;

	// Generic character type. Varying.
	using CHAR				= ANSICHAR;
	
	// type of null
	using NullPtrType		= decltype(nullptr);

	using MemPtr			= void*;
	
	using CMemPtr			= const void*;

	using MemCPtr			= void* const;			// ?? const pointer, but not the value it points to (check?)

	using CMemCPtr			= const void* const;	// ?? const pointer to const value (check?)
}

#define DECLARE_PLATFORM_TYPE( type ) using UE::Platform::##type

namespace UE
{

	// signed integer types

	DECLARE_PLATFORM_TYPE( int8 );
	DECLARE_PLATFORM_TYPE( int16 );
	DECLARE_PLATFORM_TYPE( int32 );
	DECLARE_PLATFORM_TYPE( int64 );

	// unsigned integer types

	DECLARE_PLATFORM_TYPE( uint8 );
	DECLARE_PLATFORM_TYPE( uint16 );
	DECLARE_PLATFORM_TYPE( uint32 );
	DECLARE_PLATFORM_TYPE( uint64 );

	// other types

	DECLARE_PLATFORM_TYPE( byte );

	DECLARE_PLATFORM_TYPE( SIZE_T );

	DECLARE_PLATFORM_TYPE( ANSICHAR );
	DECLARE_PLATFORM_TYPE( UNICHAR );

	DECLARE_PLATFORM_TYPE( CHAR16 );
	DECLARE_PLATFORM_TYPE( CHAR32 );

	DECLARE_PLATFORM_TYPE( CHAR );
	DECLARE_PLATFORM_TYPE( UCHAR );

	DECLARE_PLATFORM_TYPE( NullPtrType );

	DECLARE_PLATFORM_TYPE( MemPtr );
	DECLARE_PLATFORM_TYPE( CMemPtr );
	DECLARE_PLATFORM_TYPE( MemCPtr );
	DECLARE_PLATFORM_TYPE( CMemCPtr );

}