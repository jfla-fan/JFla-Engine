#pragma once
#include "Types.h"
#include "../Common/Macro.h"
#include "../Common/Assert.h"


namespace UE::Memory
{
	// RED ENGINE MEMORY UTILS

	bool constexpr	IsPowerOf2( u64 value );

	bool			IsAligned( u64 value, u32 alignment );

	bool			IsAligned( CMemPtr memory, u32 alignment );

	u64				AlignAddress( u64 address, u32 alignment );

	MemPtr			AlignAddress( MemPtr address, u32 alignment );

	// same as align address
	u64				RoundUp( u64 value, u64 roundUpTo );

	u64				RoundUp( u32 value, u32 roundUpTo );

	/**
	 * This function does not behave like std::addressof. It returns address pointed by pointer.
	 * Use AddressOf(&pointer) if you want the pointer address.
	 */
	template< typename T >
	u64 AddressOf( T& value );

	template< typename T >
	u64 AddressOf( T* value );

	template< typename T >
	u64 AddressOf( T&& value ) = delete;

}
