#pragma once
#include "GpuTypes.h"
#include "BufferTraits.h"
#include "OpenGL/OpenGLContext.h"



namespace UE::Graphics
{
	USE_BUFFER_TYPES()


	static_assert( to_underlying( EBufferUsage::STREAM_DRAW )					== GL_STREAM_DRAW,				 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferUsage::STREAM_READ )					== GL_STREAM_READ,				 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferUsage::STREAM_COPY )					== GL_STREAM_COPY,				 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferUsage::STATIC_DRAW )					== GL_STATIC_DRAW,				 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferUsage::STATIC_READ )					== GL_STATIC_READ,				 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferUsage::STATIC_COPY )					== GL_STATIC_COPY,				 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferUsage::DYNAMIC_DRAW )					== GL_DYNAMIC_DRAW,				 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferUsage::DYNAMIC_READ )					== GL_DYNAMIC_READ,				 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferUsage::DYNAMIC_COPY )					== GL_DYNAMIC_COPY,				 "Require direct mapping to OpenGL constant." );


	static_assert( to_underlying( EBufferType::ARRAY_BUFFER )					== GL_ARRAY_BUFFER,				 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferType::ATOMIC_COUNTER_BUFFER )			== GL_ATOMIC_COUNTER_BUFFER,	 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferType::COPY_READ_BUFFER )				== GL_COPY_READ_BUFFER,			 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferType::COPY_WRITE_BUFFER )				== GL_COPY_WRITE_BUFFER,		 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferType::DISPATCH_INDIRECT_BUFFER )		== GL_DISPATCH_INDIRECT_BUFFER,	 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferType::DRAW_INDIRECT_BUFFER )			== GL_DRAW_INDIRECT_BUFFER,		 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferType::ELEMENT_ARRAY_BUFFER )			== GL_ELEMENT_ARRAY_BUFFER,		 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferType::PIXEL_PACK_BUFFER )				== GL_PIXEL_PACK_BUFFER,		 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferType::PIXEL_UNPACK_BUFFER )			== GL_PIXEL_UNPACK_BUFFER,		 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferType::QUERY_BUFFER )					== GL_QUERY_BUFFER,				 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferType::SHADER_STORAGE_BUFFER )			== GL_SHADER_STORAGE_BUFFER,	 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferType::TEXTURE_BUFFER )					== GL_TEXTURE_BUFFER,			 "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferType::TRANSFORM_FEEDBACK_BUFFER )		== GL_TRANSFORM_FEEDBACK_BUFFER, "Require direct mapping to OpenGL constant." );
	static_assert( to_underlying( EBufferType::UNIFORM_BUFFER )					== GL_UNIFORM_BUFFER,			 "Require direct mapping to OpenGL constant." );



	template< class _Derived_buffer, typename _Buffer_tag >
	class BufferBuilder
	{
	public:

		using buffer_traits = _Default_buffer_traits< _Buffer_tag >;

	public:

		Scope< _Derived_buffer > Create( buffer_traits::element_type* elements, buffer_traits::size_type size,
										 buffer_traits::buffer_type type, buffer_traits::usage_type usage,
										 buffer_traits::access_type access );

	private:

	};

	template< class _Derived_buffer, typename _Buffer_tag >
	class Buffer
	{
	public:

		using buffer_traits		= _Default_buffer_traits<_Buffer_tag>;

		using this_type			= _Derived_buffer;
		using this_pointer_type = this_type*;
		
		using tag_type			= buffer_traits::tag_type;
		using resource_type		= buffer_traits::resource_type;
		using element_type		= buffer_traits::element_type;
		using size_type			= buffer_traits::size_type;
		using buffer_type		= buffer_traits::buffer_type;
		using usage_type		= buffer_traits::usage_type;
		using access_type		= buffer_traits::access_type;


	public:

		static_assert( std::is_class<_Derived_buffer>::value, "Derived buffer should be a custom class" );
		
	public:

		Buffer() { }

		Buffer(const Buffer&) = delete;
		Buffer& operator = (const Buffer&) = delete;

		virtual					~Buffer() { }

		virtual void			Bind() const = 0;
		virtual void			Unbind() const = 0;

		virtual resource_type	GetDescriptor() const = 0;
		virtual buffer_type		GetBufferType() const = 0;
		virtual usage_type		GetUsage() const = 0;
		virtual size_type		GetSize() const = 0;
		virtual access_type		GetAccess() const = 0;

		virtual bool			IsValid() const = 0;

		static Scope<this_type> Create( element_type* elements, size_type size, buffer_type type, usage_type usage, access_type access )
		{
			return BufferBuilder< this_type, tag_type >().Create( elements, size, type, usage, access );
		}

	private:

		friend class BufferBuilder< this_type, tag_type >;
	};

	template< class _Derived_buffer >
	class BufferBuilder< _Derived_buffer, _Vertex_buffer_tag >
	{
	public:

		using buffer_traits = _Default_buffer_traits< _Vertex_buffer_tag >;
		
	public:

		Scope< _Derived_buffer > Create( buffer_traits::element_type* elements, buffer_traits::size_type size,
										 buffer_traits::buffer_type type, buffer_traits::usage_type usage,
										 buffer_traits::access_type access );

	private:

	};

	template< class _Derived_buffer >
	class Buffer< _Derived_buffer, _Vertex_buffer_tag >
	{
	public:

		using buffer_traits		= _Default_buffer_traits< _Vertex_buffer_tag >;
		
		using this_type			= _Derived_buffer;
		using this_pointer_type = this_type*;

		using tag_type			= buffer_traits::tag_type;
		using resource_type		= buffer_traits::resource_type;
		using element_type		= buffer_traits::element_type;
		using size_type			= buffer_traits::size_type;
		using buffer_type		= buffer_traits::buffer_type;
		using usage_type		= buffer_traits::usage_type;
		using access_type		= buffer_traits::access_type;


	public:

		static_assert( std::is_class<_Derived_buffer>::value, "Derived buffer should be a custom class" );
		
	public:

		Buffer() { }

		Buffer(const Buffer&) = delete;
		Buffer& operator = (const Buffer&) = delete;
		
		virtual					~Buffer() { }

		virtual void			Bind() const = 0;
		virtual void			Unbind() const = 0;

		virtual resource_type	GetDescriptor() const = 0;
		virtual buffer_type		GetBufferType() const = 0;
		virtual usage_type		GetUsage() const = 0;
		virtual size_type		GetSize() const = 0;
		virtual access_type		GetAccess() const = 0;

		virtual bool			IsValid() const = 0;

		static Scope< this_type > Create( element_type* elements, size_type size,
										  buffer_type type = EBufferType::ARRAY_BUFFER,
										  usage_type usage = EBufferUsage::STATIC_DRAW,
										  access_type access = EBufferAccessBits::ALL )
		{
			return BufferBuilder< this_type, tag_type >().Create( elements, size, type, usage, access );
		}

	private:
		
		friend class BufferBuilder< this_type, tag_type >;
	};


	template< class _Derived_buffer >
	class BufferBuilder< _Derived_buffer, _Index_buffer_tag >
	{
	public:

		using buffer_traits = _Default_buffer_traits< _Index_buffer_tag >;

	public:

		Scope< _Derived_buffer > Create( buffer_traits::element_type* elements,
										 buffer_traits::size_type size,
										 buffer_traits::access_type access );

	private:

	};

	template< class _Derived_buffer >
	class Buffer< _Derived_buffer, _Index_buffer_tag >
	{
	public:

		static_assert( std::is_class<_Derived_buffer>::value, "Derived buffer should be a custom class" );
	
	public:

		using buffer_traits		= _Default_buffer_traits< _Index_buffer_tag >;

		using this_type			= _Derived_buffer;
		using this_pointer_type = this_type*;

		using tag_type			= buffer_traits::tag_type;
		using resource_type		= buffer_traits::resource_type;
		using element_type		= buffer_traits::element_type;
		using size_type			= buffer_traits::size_type;
		using buffer_type		= buffer_traits::buffer_type;
		using usage_type		= buffer_traits::usage_type;
		using access_type		= buffer_traits::access_type;

	public:

		Buffer() { }

		Buffer(const Buffer&) = delete;
		Buffer& operator = (const Buffer&) = delete;

		virtual					~Buffer() { }

		virtual void			Bind() const = 0;
		virtual void			Unbind() const = 0;

		virtual resource_type	GetDescriptor() const = 0;
		virtual buffer_type		GetBufferType() const = 0;
		virtual usage_type		GetUsage() const = 0;
		virtual size_type		GetSize() const = 0;
		virtual access_type		GetAccess() const = 0;

		virtual bool			IsValid() const = 0;

		static Scope< this_type > Create( element_type* elements, size_type size, access_type access )
		{
			return BufferBuilder< this_type, _Index_buffer_tag >().Create( elements, size, access );
		}

	private:

		friend class BufferBuilder< this_type, tag_type >;
	};
}


