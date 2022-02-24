#pragma once
#include <atomic>
#include <cstddef>


namespace UE::Atomic
{
	template< class _Ty >
	using TAtomic		= std::atomic< _Ty >;

	using TAtomic8		= std::atomic_int8_t;
	using TAtomic8U		= std::atomic_uint8_t;

	using TAtomic16		= std::atomic_int16_t;
	using TAtomic16U	= std::atomic_uint16_t;

	using TAtomic32		= std::atomic_int32_t;
	using TAtomic32U	= std::atomic_uint32_t;

	using TAtomic64		= std::atomic_int64_t;
	using TAtomic64U	= std::atomic_uint64_t;

	using TAtomicChar	= std::atomic_char;
	using TAtomicUchar	= std::atomic_uchar;

	using TAtomicByte	= TAtomic< std::byte >;

	using TAtomicBool	= std::atomic_bool;
	
	using TAtomicChar8	= std::atomic_char8_t;
	using TAtomicChar16 = std::atomic_char16_t;
	using TAtomicChar32 = std::atomic_char32_t;

}
