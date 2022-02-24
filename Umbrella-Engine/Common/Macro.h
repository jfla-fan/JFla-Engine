#pragma once
#include <type_traits>
#include "Compiler.h"
#include "PlatformType.h"


#define JF_BIT( n ) ( 1u << ( n ) )

#define UNDERLYING_TYPE( type ) std::underlying_type_t< type >


#define DECLARE_ENUM_FLAG_OPERATIONS( enum_name)																											\
		FORCEINLINE enum_name operator & ( enum_name lhs, enum_name rhs )																					\
		{ return static_cast< enum_name >( static_cast< UNDERLYING_TYPE( enum_name ) >( lhs ) & static_cast< UNDERLYING_TYPE( enum_name ) >( rhs ) ); }		\
		FORCEINLINE enum_name operator | ( enum_name lhs, enum_name rhs )																					\
		{ return static_cast< enum_name >( static_cast< UNDERLYING_TYPE( enum_name ) >( lhs ) | static_cast< UNDERLYING_TYPE( enum_name ) >( rhs ) ); }		\
		FORCEINLINE enum_name operator & ( enum_name lhs, UNDERLYING_TYPE( enum_name ) rhs )																\
		{ return static_cast< enum_name >( static_cast< UNDERLYING_TYPE( enum_name ) >( lhs ) | rhs ); }													\
		FORCEINLINE enum_name operator & ( UNDERLYING_TYPE( enum_name ) lhs, enum_name rhs )																\
		{ return static_cast< enum_name >( lhs | static_cast< UNDERLYING_TYPE( enum_name ) >( rhs ) ); }													\
		FORCEINLINE enum_name operator | ( enum_name lhs, UNDERLYING_TYPE( enum_name ) rhs )																\
		{ return static_cast< enum_name >( static_cast< UNDERLYING_TYPE( enum_name ) >( lhs ) | rhs ); }													\
		FORCEINLINE enum_name operator | ( UNDERLYING_TYPE( enum_name ) lhs, enum_name rhs )																\
		{ return static_cast< enum_name >( lhs | static_cast< UNDERLYING_TYPE( enum_name ) >( rhs ) ); }													\
		FORCEINLINE enum_name operator &= ( enum_name& lhs, enum_name rhs )																					\
		{ return lhs = lhs & rhs; }																															\
		FORCEINLINE enum_name operator |= ( enum_name& lhs, enum_name rhs )																					\
		{ return lhs = lhs | rhs; }																															\
		FORCEINLINE bool operator ! ( enum_name flags )																										\
		{ return static_cast< UNDERLYING_TYPE( enum_name ) >( flags ) == UNDERLYING_TYPE( enum_name )( 0 );  }												\
		FORCEINLINE bool Any( enum_name var ) { return !!( var ); }


#define JF_ARRAY_COUNT( arr ) sizeof( ( arr ) ) / sizeof( ( arr )[0] )

#define JF_ARRAY_COUNT_TYPE( arr, type ) sizeof( ( arr ) ) / sizeof( type )


#if ENGINE_MSVC_COMPILER
#define FORCEINLINE __forceinline
#elif ENGINE_GCC_COMPILER
#define FORCEINLINE __attribute__((always_inline))
#else
#define FORCEINLINE
#endif


#define NOEXCEPT noexcept
#define INLINE inline
#define NODISCARD [[nodiscard]]
#define FALLTHROUGH [[fallthrough]]
#define MAYBE_UNUSED [[maybe_unused]]


#define JF_UNUSED( var ) (void)var
#define JF_UNUSED2( var1, var2 ) JF_UNUSED( var1 ); JF_UNUSED( var2 )
#define JF_UNUSED3( var1, var2, var3 ) JF_UNUSED2( var1, var2 ); JF_UNUSED( var3 )
#define JF_UNUSED4( var1, var2, var3, var4 ) JF_UNUSED3( var1, var2, var3 ); JF_UNUSED( var4 )


#if ENGINE_WINDOWS_PLATFORM
	#define JF_DEBUG_BREAK() __debugbreak()
#elif ENGINE_LINUX_PLATFORM
	#define JF_DEBUG_BREAK asm(int3)
#else
	#error Unsupported platform
#endif


// #TODO MAKE NORMAL DEBUG INFORMATION, LIKE __LINE__, __PRETTY_FUNCTION__, other compiler-specific decorations ...
#define NOT_IMPLEMENTED_YET()										\
	{																\
		std::cout << "This function is not implemented yet!\n";		\
		throw;														\
	}

#define JF_CONCATENATE( x, y ) x##y

#define JF_CONCATENATE2( x, y ) JF_CONCATENATE( x, y )

#define JF_EXPAND(...) __VA_ARGS__

#define JF_STRINGIFY( x, y ) #x #y

#define JF_NEW( type ) new type

// logic mark meaning - this method/member/class/entry is optional in the current context.
#define OPTIONAL

// logic mark meaning - this method/member/class/entry is required in the current context.
#define REQUIRED

// logic mark meaning - this method/member/class/entry is deprecated in the current context.
#define DEPRECATED


// Unicode string literal
#define UTEXT( text ) L##text

// Ansi string literal
#define TEXT( text ) text

