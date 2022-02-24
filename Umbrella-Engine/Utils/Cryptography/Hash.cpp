#include "Hash.h"


namespace UE::Crypto
{
	// hash function - FNV-1


	constexpr THash32 CalculateHash32( CMemPtr buffer, SIZE_T bufferSize, THash32 baseHash )
	{
		auto hash = baseHash;
		auto data = static_cast< const uint8* >( buffer );
		
		while ( bufferSize-- )
			{
				hash *= FNV_OFFSET_PRIME32;
				hash ^= *data++;
			}
	
		return hash;
	}

	constexpr THash32 CalculateHash32( const byte* buffer, SIZE_T bufferSize, THash32 baseHash )
	{
		auto hash = baseHash;
		auto data = ( const uint8* )( buffer );

		while ( bufferSize-- )
		{
			hash *= FNV_OFFSET_PRIME32;
			hash ^= *data++;
		}

		return hash;
	}

	constexpr THash32 CalculateHash32( const uint8* buffer, SIZE_T bufferSize, THash32 baseHash )
	{
		auto hash = baseHash;
		
		while ( bufferSize-- )
		{
			hash *= FNV_OFFSET_PRIME32;
			hash ^= *buffer++;
		}
	
		return hash;
	}

	constexpr THash32 CalculateAnsiHash32( const ANSICHAR* str, THash32 baseHash )
	{
		auto hash = baseHash;

		if ( str )
		{
			while ( const auto next = *str )
			{
				hash *= FNV_OFFSET_PRIME32;
				hash ^= next;
			}
		}

		return hash;
	}

	constexpr THash32 CalculateUnicodeHash32( const UNICHAR* str, THash32 baseHash )
	{
		auto hash = baseHash;

		if ( str )
		{
			while ( const auto next = *str )
			{
				hash *= FNV_OFFSET_PRIME32;
				hash ^= next;
			}
		}

		return hash;
	}

	constexpr THash32 CalculateAnsiHash32Bounded( const ANSICHAR* str, size_t length, THash32 baseHash )
	{
		auto hash = baseHash;

		while ( length-- )
		{
			hash *= FNV_OFFSET_PRIME32;
			hash ^= *str++;
		}

		return hash;
	}

	constexpr THash32 CalculateUnicodeHash32Bounded( const UNICHAR* str, size_t length, THash32 baseHash )
	{
		auto hash = baseHash;

		while ( length-- )
		{
			hash *= FNV_OFFSET_PRIME32;
			hash ^= *str++;
		}

		return hash;
	}



	constexpr THash64 CalculateHash64( CMemPtr buffer, SIZE_T bufferSize, THash64 baseHash )
	{
		auto hash = baseHash;
		auto data = static_cast< const uint8* >( buffer );

		while ( bufferSize-- )
		{
			hash *= FNV_OFFSET_PRIME64;
			hash ^= *data++;
		}

		return hash;
	}

	constexpr THash64 CalculateHash64( const byte* buffer, SIZE_T bufferSize, THash64 baseHash )
	{
		auto hash = baseHash;
		auto data = ( const uint8* )( buffer );

		while ( bufferSize-- )
		{
			hash *= FNV_OFFSET_PRIME64;
			hash ^= *data++;
		}

		return hash;
	}

	constexpr THash64 CalculateAnsiHash64( const ANSICHAR* str, THash64 baseHash )
	{
		auto hash = baseHash;

		if ( str )
		{
			while ( const auto next = *str )
			{
				hash *= FNV_OFFSET_PRIME64;
				hash ^= next;
			}
		}

		return hash;
	}

	constexpr THash64 CalculateUnicodeHash64( const UNICHAR* str, THash64 baseHash )
	{
		auto hash = baseHash;

		if ( str )
		{
			while ( const auto next = *str )
			{
				hash *= FNV_OFFSET_PRIME64;
				hash ^= next;
			}
		}

		return hash;
	}

	constexpr THash64 CalculateAnsiHash64Bounded( const ANSICHAR* str, size_t length, THash64 baseHash )
	{
		auto hash = baseHash;

		while ( length-- )
		{
			hash *= FNV_OFFSET_PRIME64;
			hash ^= *str++;
		}

		return hash;
	}

	constexpr THash64 CalculateUnicodeHash32Bounded( const UNICHAR* str, size_t length, THash64 baseHash )
	{
		auto hash = baseHash;

		while ( length-- )
		{
			hash *= FNV_OFFSET_PRIME64;
			hash ^= *str++;
		}

		return hash;
	}



}
