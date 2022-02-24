#include "MemoryUtils.h"


namespace UE::Memory
{
	// RED ENGINE MEMORY UTILS

	INLINE constexpr bool IsPowerOf2( u64 value )
	{
		return ( value & ( value - 1 ) ) == 0;
	}

	INLINE bool IsAligned( u64 value, u32 alignment )
	{
		FATAL_ASSERT( IsPowerOf2( alignment ), "Provided alignment need to be power of 2" );
		return ( value & ( alignment - 1)) == 0;
	}

	INLINE bool IsAligned( CMemPtr memory, u32 alignment )
	{
		return IsAligned( reinterpret_cast< u64 >( memory ), alignment );
	}

	INLINE u64 AlignAddress( u64 address, u32 alignment )
	{
		FATAL_ASSERT( IsPowerOf2( alignment ), "Provided alignment need to be power of 2" );
		const u64 mask = -static_cast< int64 >( alignment );
		const u64 result = ( address + ( alignment - 1 ) ) & mask;
		return result;
	}

	INLINE MemPtr AlignAddress( MemPtr address, u32 alignment )
	{
		u64 value = reinterpret_cast< u64 >( address );
		const u64 result = AlignAddress( value, alignment );
		return reinterpret_cast< MemPtr >( result );
	}

	INLINE u64 RoundUp( u64 value, u64 roundUpTo )
	{
		FATAL_ASSERT( IsPowerOf2( roundUpTo ), "Function can be used only for power of 2 values." );
		return ( value + ( roundUpTo - 1 ) ) & ~( roundUpTo - 1 );
	}

	INLINE u64 RoundUp( u32 value, u32 roundUpTo )
	{
		FATAL_ASSERT( IsPowerOf2( roundUpTo ), "Function can be used only for power of 2 values." );
		return ( value + ( roundUpTo - 1 ) ) & ~( roundUpTo - 1 );
	}

	template< typename T >
	INLINE u64 AddressOf( T& value )
	{
		return reinterpret_cast< u64 >( std::addressof( value ) );
	}

	template< typename T >
	INLINE u64 AddressOf( T* value )
	{
		return reinterpret_cast< u64 >( value );
	}
}
