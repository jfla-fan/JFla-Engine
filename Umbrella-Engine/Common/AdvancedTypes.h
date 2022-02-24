#pragma once

#include "../Misc/Float16.h"
#include "../Misc/Float32.h"



namespace UE::Platform::Details
{
	constexpr bool GIsFloat32Bits = (sizeof(float) == 4);
}

#define IS_BASE_FLOAT_TYPE_32_BITS (UE::Platform::Details::GIsFloat32Bits)


namespace UE::Platform
{
	static_assert( IS_BASE_FLOAT_TYPE_32_BITS );

	using float16 = Float16;

	using float32 = Float32;

	static NullPtrType NullPtr = nullptr;
}

namespace UE
{

	DECLARE_PLATFORM_TYPE( float16 );

	DECLARE_PLATFORM_TYPE( float32 );

	DECLARE_PLATFORM_TYPE( NullPtr );
}
