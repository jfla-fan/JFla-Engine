#pragma once
#include "../../../Core.h"

/** /************************************************************************/

/**
 * This header file contains definitions for standard and custom buffer tags, default buffer traits.
 * 
 * If you want to implement a custom buffer type, you may use existing buffer tags or
 * use _Custom_buffer_tag with your own specified types.
 * 
 * Every custom buffer tag should contain at least the following types:
 * 
 * Tag_type			-		tag type itself.
 * Resource_type	-		buffer UID.
 * Size_type		-		buffer size type.
 * Element_type		-		type of elements in the buffer.
 * Buffer_type		-		type of buffer.
 * Usage_type		-		type of usage.
 * 
 */


/************************************************************************ */

namespace UE::Graphics::GpuApi
{
	enum class EBufferType;
	enum class EBufferUsage;
	enum class EBufferAccessBits : uint8;

	struct BufferRef;
}

namespace UE
{
	using namespace Graphics::GpuApi;


	struct _Index_buffer_tag { };
	struct _Vertex_buffer_tag { };
	struct _Const_buffer_tag { };	// DX3D11, not implemented yet


	template< typename _Resource_type, typename _Size_type, typename _Element_type,
			  typename _Buffer_type, typename _Usage_type, typename _Access_type >
	struct _Custom_buffer_tag
	{

		using tag_type		= _Custom_buffer_tag;

		using resource_type = _Resource_type;
		using size_type		= _Size_type;
		using element_type	= _Element_type;
		using buffer_type	= _Buffer_type;
		using usage_type	= _Usage_type;
		using access_type	= _Access_type;
	
	};


	template<class _Custom_buffer_tag>
	struct _Default_buffer_traits
	{

		using tag_type		= _Custom_buffer_tag::tag_type;

		using resource_type = _Custom_buffer_tag::resource_type;
		using size_type		= _Custom_buffer_tag::size_type;
		using element_type	= _Custom_buffer_tag::element_type;
		using buffer_type	= _Custom_buffer_tag::buffer_type;
		using usage_type	= _Custom_buffer_tag::usage_type;
		using access_type	= _Custom_buffer_tag::access_type;

	};


	template<>
	struct _Default_buffer_traits< _Vertex_buffer_tag >
	{

		using tag_type		= _Vertex_buffer_tag;

		using resource_type = BufferRef;
		using size_type		= uint32;
		using element_type	= float;
		using buffer_type	= EBufferType;
		using usage_type	= EBufferUsage;
		using access_type	= EBufferAccessBits;

	};


	template<>
	struct _Default_buffer_traits< _Index_buffer_tag >
	{

		using tag_type		= _Index_buffer_tag;

		using resource_type = BufferRef;
		using size_type		= uint32;
		using element_type	= uint32;
		using buffer_type	= EBufferType;
		using usage_type	= EBufferUsage;
		using access_type	= EBufferAccessBits;

	};


	//				UNUSED					//

	template<class _Buffer_type, typename RetVal, typename... Args>
	struct _Has_buffer_default_create_impl_static_member_function
	{
	private:
		template<class _Ty>
		using DefaultCreateImplFuncPointer = RetVal (*) (Args...);

		template<typename _Class, DefaultCreateImplFuncPointer<_Class>> struct SFINAE {};

		template<typename _Class> static CHAR Test(SFINAE<_Class, &_Class::CreateImpl>*);
		template<typename _Class> static int32 Test(...);
		
	public:
		constexpr static auto value = std::bool_constant<sizeof(Test<_Buffer_type>(0)) == sizeof(CHAR)>::value;
	};

	//template<class _Buffer_type> using HasVertexBufferDefaultImplFunc =
	//	_Has_buffer_default_create_impl_static_member_function
	//	<
	//		_Buffer_type,
	//		Scope<_Buffer_type>,
	//		_Default_buffer_traits<_Vertex_buffer_tag>::Element_type*,
	//		_Default_buffer_traits<_Vertex_buffer_tag>::Size_type,
	//		_Default_buffer_traits<_Vertex_buffer_tag>::Buffer_type,
	//		_Default_buffer_traits<_Vertex_buffer_tag>::Usage_type
	//	>
	//	::value;

	//template<class _Buffer_type> using HasOpenGLIndexBufferDefaultImplFunc =
	//	_Has_buffer_default_create_impl_static_member_function
	//	<
	//		_Buffer_type,
	//		Scope<_Buffer_type>,
	//		_Default_buffer_traits<_Index_buffer_tag>::Element_type*,
	//		_Default_buffer_traits<_Index_buffer_tag>::Size_type
	//	>::value;

	//template<class _Buffer_type, typename _Buffer_tag> using HasCustomBufferDefaultImplFunc =
	//	_Has_buffer_default_create_impl_static_member_function
	//	<
	//		_Buffer_type,
	//		Scope<_Buffer_type>,
	//		_Default_buffer_traits<_Buffer_tag>::Element_type*,
	//		_Default_buffer_traits<_Buffer_tag>::Size_type,
	//		_Default_buffer_traits<_Buffer_tag>::Buffer_type,
	//		_Default_buffer_traits<_Buffer_tag>::Usage_type
	//	>::value;
}
