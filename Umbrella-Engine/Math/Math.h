#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/integer.hpp>
#include "Color.h"
#include <bit>


namespace UE::Math
{
	// types

	using Matrix2x2		= glm::mat2x2;

	using Matrix3x3		= glm::mat3x3;

	using Matrix3x2		= glm::mat3x2;
	using Matrix2x3		= glm::mat2x3;

	using Matrix4x4		= glm::mat4x4;

	using Matrix4x2		= glm::mat4x2;
	using Matrix2x4		= glm::mat2x4;
	using Matrix4x3		= glm::mat4x3;
	using Matrix3x4		= glm::mat3x4;



	using Vector1		= glm::vec1;
	using Vector2		= glm::vec2;
	using Vector3		= glm::vec3;
	using Vector4		= glm::vec4;

	using VectorInt1	= glm::ivec1;
	using VectorInt2	= glm::ivec2;
	using VectorInt3	= glm::ivec3;
	using VectorInt4	= glm::ivec4;

	using VectorUInt1	= glm::uvec1;
	using VectorUInt2	= glm::uvec2;
	using VectorUInt3	= glm::uvec3;
	using VectorUInt4	= glm::uvec4;

	using Quaternion	= glm::quat;

	template< class _Ty, glm::length_t _Rank >
	using Vector		= glm::vec< _Rank, _Ty >;

	template< class _Ty, glm::length_t _Columns, glm::length_t _Raws >
	using Matrix		= glm::mat< _Columns, _Raws, _Ty >;

	
	// common functions

	template< typename _Ty >
	FORCEINLINE _Ty Clamp( _Ty Value, _Ty Min, _Ty Max )
	{
		return glm::clamp( Value, Min, Max );
	}

	template< typename _Ty >
	FORCEINLINE _Ty Min( _Ty v1, _Ty v2 )
	{
		return glm::min( v1, v2 );
	}

	template< typename _Ty >
	FORCEINLINE _Ty Max( _Ty v1, _Ty v2 )
	{
		return glm::max( v1, v2 );
	}

	template< typename _Ty >
	FORCEINLINE _Ty Abs( _Ty v )
	{
		return glm::abs( v );
	}

	template< typename _Ty >
	FORCEINLINE bool IsPowerOf2( _Ty t )
	{
		return std::has_single_bit( t );
	}

	template< class _Ty, class... Args >
	FORCEINLINE auto MakeVector( Args&&... args )
	{
		return Vector( std::forward< Args >( args )... );
	}

	template<class _Ty >
	FORCEINLINE _Ty Log( _Ty value )
	{
		return glm::log( value );
	}

	template< class _Ty >
	FORCEINLINE _Ty Log2( _Ty value )
	{
		return glm::log2( value );
	}

	FORCEINLINE Vector4 MakeVector4( float x, float y, float z, float w )
	{
		return Vector4( x, y, z, w );
	}

	FORCEINLINE Vector4 MakeVector4( float v )
	{
		return Vector4( v );
	}

	FORCEINLINE Vector4 MakeVector4()
	{
		return Vector4();
	}

	FORCEINLINE Vector3 MakeVector3( float x, float y, float z )
	{
		return Vector3( x, y, z );
	}

	FORCEINLINE Vector3 MakeVector3( float v )
	{
		return Vector3( v );
	}

	FORCEINLINE Vector3 MakeVector3()
	{
		return Vector3();
	}

	FORCEINLINE Vector2 MakeVector2( float x, float y )
	{
		return Vector2(x, y);
	}

	FORCEINLINE Vector2 MakeVector2( float v )
	{
		return Vector2( v );
	}

	FORCEINLINE Vector2 MakeVector2()
	{
		return Vector2();
	}

	FORCEINLINE Vector1 MakeVector( float v )
	{
		return Vector1( v );
	}

	FORCEINLINE Vector1 MakeVector()
	{
		return Vector1();
	}
}
