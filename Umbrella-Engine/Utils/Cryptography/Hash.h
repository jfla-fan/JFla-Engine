#pragma once

#include "../../Common/BaseTypes.h"




namespace UE::Crypto
{
	constexpr uint32 FNV_OFFSET_BASIS32 = 2166136261u;
	constexpr uint32 FNV_OFFSET_PRIME32	= 16777619u;

	constexpr uint64 FNV_OFFSET_BASIS64 = 14695981039346656037u;
	constexpr uint64 FNV_OFFSET_PRIME64 = 1099511628211u;

	
	using THash32 = uint32;
	using THash64 = uint64;

	// algorithm - FNV-1

	constexpr THash32 CalculateHash32( CMemPtr buffer, SIZE_T bufferSize, THash32 baseHash = FNV_OFFSET_BASIS32 );
	
	constexpr THash32 CalculateHash32( const byte* buffer, SIZE_T bufferSize, THash32 baseHash = FNV_OFFSET_BASIS32 );

	constexpr THash32 CalculateAnsiHash32( const ANSICHAR* str, THash32 baseHash = FNV_OFFSET_BASIS32 );

	constexpr THash32 CalculateUnicodeHash32( const UNICHAR* str, THash32 baseHash = FNV_OFFSET_BASIS32 );

	constexpr THash32 CalculateAnsiHash32Bounded( const ANSICHAR* str, size_t length, THash32 baseHash = FNV_OFFSET_BASIS32 );

	constexpr THash32 CalculateUnicodeHash32Bounded(const UNICHAR* str, size_t length, THash32 baseHash = FNV_OFFSET_BASIS32 );

	

	constexpr THash64 CalculateHash64( CMemPtr buffer, SIZE_T bufferSize, THash64 baseHash = FNV_OFFSET_BASIS64 );

	constexpr THash64 CalculateHash64( const byte* buffer, SIZE_T bufferSize, THash64 baseHash = FNV_OFFSET_BASIS64 );

	constexpr THash64 CalculateAnsiHash64( const ANSICHAR* str, THash64 baseHash = FNV_OFFSET_BASIS64 );

	constexpr THash64 CalculateUnicodeHash64( const UNICHAR* str, THash64 baseHash = FNV_OFFSET_BASIS64 );

	constexpr THash64 CalculateAnsiHash64Bounded( const ANSICHAR* str, size_t length, THash64 baseHash = FNV_OFFSET_BASIS64 );

	constexpr THash64 CalculateUnicodeHash32Bounded( const UNICHAR* str, size_t length, THash64 baseHash = FNV_OFFSET_BASIS64 );



}