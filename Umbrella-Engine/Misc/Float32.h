#pragma once

#include "../Common/PlatformType.h"
#include "../Common/BaseTypes.h"
#include <compare>


namespace J
{
	
	/**
	 * IEEE 754 single-precision binary floating-point format.
	 */
	struct Float32
	{
		union
		{
			struct 
			{
#if ENGINE_PLATFORM_LITTLE_ENDIAN
				uint32 Mantissa : 23;
				uint32 Exponent : 8;
				uint32 Sign : 1;
#else
				uint32 Sign : 1;
				uint32 Exponent : 8;
				uint32 Mantissa : 23;
#endif
			}
			Components;

			float Value;
		};

		Float32() : Value(0) { };
		Float32(float InValue) : Value(InValue) { }

		operator float() const;

		auto operator <=> (const Float32& another) const = default;		// automatically generates all six comparison operators
	};


}