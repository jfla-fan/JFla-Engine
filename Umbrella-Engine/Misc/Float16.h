#pragma once
#include "../Common/PlatformType.h"
#include "../Common/BaseTypes.h"
#include <compare>


namespace UE
{
	/**
	 * IEEE 754 half-precision binary floating-point format.
	 * 
	 */
	struct Float16
	{
		union
		{
			struct 
			{
#if ENGINE_PLATFORM_LITTLE_ENDIAN
				uint16 Mantissa : 10;
				uint16 Exponent : 5;
				uint16 Sign : 1;
#else
				uint16 Sign : 1;
				uint16 Exponent : 5;
				uint16 Mantissa : 10;
#endif
			}
			Components;
			
			uint16 Value;
		};

		Float16() : Value( 0 ) { };
		Float16( float InValue );

		Float16( const Float16& another );
		Float16( Float16&& another );

		Float16& operator = ( const Float16& another );
		Float16& operator = ( Float16&& another );

		operator float() const;

		auto operator <=> ( const Float16& another ) const = default;		// automatically generates all six comparison operators
	};




}
