#pragma once
#include "../../../Core.h"
#include "OpenGL/OpenGLContext.h"
#include "BufferTraits.h"


#define GL_ENUM(element) (GLenum)(element)


namespace Engine::Graphics
{
	enum class EBufferUsage
	{
		NONE = 0,							// Unknown
		STREAM_DRAW = GL_STREAM_DRAW,		//
		STREAM_READ = GL_STREAM_READ,
		STREAM_COPY = GL_STREAM_COPY,
		STATIC_DRAW = GL_STATIC_DRAW,
		STATIC_READ = GL_STATIC_READ,
		STATIC_COPY = GL_STATIC_COPY,
		DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
		DYNAMIC_READ = GL_DYNAMIC_READ,
		DYNAMIC_COPY = GL_DYNAMIC_COPY,
	};
	
	enum class EBufferType
	{
		NONE = 0,													// Unknown
		ARRAY_BUFFER = GL_ARRAY_BUFFER,								// Vertex attributes
		ATOMIC_COUNTER_BUFFER = GL_ATOMIC_COUNTER_BUFFER,			// Atomic counter storage
		COPY_READ_BUFFER = GL_COPY_READ_BUFFER,						// Buffer copy source
		COPY_WRITE_BUFFER = GL_COPY_WRITE_BUFFER,					// Buffer copy destination
		DISPATCH_INDIRECT_BUFFER = GL_DISPATCH_INDIRECT_BUFFER,		// Indirect compute dispatch commands
		DRAW_INDIRECT_BUFFER = GL_DRAW_INDIRECT_BUFFER,				// Indirect command arguments
		ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,				// Vertex array indices
		PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,					// Pixel read target
		PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER,				// Texture data source
		QUERY_BUFFER = GL_QUERY_BUFFER,								// Query result buffer
		SHADER_STORAGE_BUFFER = GL_SHADER_STORAGE_BUFFER,			// Read - write storage for shaders
		TEXTURE_BUFFER = GL_TEXTURE_BUFFER,							// Texture data buffer
		TRANSFORM_FEEDBACK_BUFFER = GL_TRANSFORM_FEEDBACK_BUFFER,	// Transform feedback buffer
		UNIFORM_BUFFER = GL_UNIFORM_BUFFER,							// Uniform block storage
	};


	template<class _Derived_buffer, typename _Buffer_tag>
	class BufferBuilder
	{
	public:

		using buffer_traits = _Default_buffer_traits<_Buffer_tag>;

	public:

		Scope<_Derived_buffer> Create(buffer_traits::element_type* elements, buffer_traits::size_type size,
			buffer_traits::buffer_type type, buffer_traits::usage_type usage);

	private:

	};

	template<class _Derived_buffer, typename _Buffer_tag>
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

	public:

		static_assert(std::is_class<_Derived_buffer>::value, "Derived buffer should be a custom class");
		
	public:

		virtual					~Buffer() { }

		virtual void			Bind() const = 0;
		virtual void			Unbind() const = 0;

		virtual resource_type	GetNativeHandle() const = 0;
		virtual buffer_type		GetBufferType() const = 0;
		virtual usage_type		GetUsage() const = 0;
		virtual size_type		GetSize() const = 0;

		virtual bool			IsValid() const = 0;

		static Scope<this_type> Create(element_type* elements, size_type size, buffer_type type, usage_type usage)
		{
			//return CreateFunc(elements, size, type, usage);
			return BufferBuilder<this_type, tag_type>().Create(elements, size, type, usage);
		}

	private:

		friend class BufferBuilder<this_type, tag_type>;
	};

	template<class _Derived_buffer>
	class BufferBuilder<_Derived_buffer, _Vertex_buffer_tag>
	{
	public:

		using buffer_traits = _Default_buffer_traits<_Vertex_buffer_tag>;
		
	public:

		Scope<_Derived_buffer> Create(buffer_traits::element_type* elements, buffer_traits::size_type size,
			buffer_traits::buffer_type type, buffer_traits::usage_type usage);

	private:

	};

	template<class _Derived_buffer>
	class Buffer<_Derived_buffer, _Vertex_buffer_tag>
	{
	public:

		using buffer_traits		= _Default_buffer_traits<_Vertex_buffer_tag>;
		
		using this_type			= _Derived_buffer;
		using this_pointer_type = this_type*;

		using tag_type			= buffer_traits::tag_type;
		using resource_type		= buffer_traits::resource_type;
		using element_type		= buffer_traits::element_type;
		using size_type			= buffer_traits::size_type;
		using buffer_type		= buffer_traits::buffer_type;
		using usage_type		= buffer_traits::usage_type;

	public:

		static_assert(std::is_class<_Derived_buffer>::value, "Derived buffer should be a custom class");
		
	public:

		virtual					~Buffer() { }

		virtual void			Bind() const = 0;
		virtual void			Unbind() const = 0;

		virtual resource_type	GetNativeHandle() const = 0;
		virtual buffer_type		GetBufferType() const = 0;
		virtual usage_type		GetUsage() const = 0;
		virtual size_type		GetSize() const = 0;

		virtual bool			IsValid() const = 0;

		static Scope<this_type> Create(element_type* elements, size_type size, buffer_type type = EBufferType::ARRAY_BUFFER, usage_type usage = EBufferUsage::STATIC_DRAW)
		{
			return BufferBuilder<this_type, tag_type>().Create(elements, size, type, usage);
		}

	private:
		
		friend class BufferBuilder<this_type, tag_type>;
	};


	template<class _Derived_buffer>
	class BufferBuilder<_Derived_buffer, _Index_buffer_tag>
	{
	public:

		using buffer_traits = _Default_buffer_traits<_Index_buffer_tag>;

	public:

		Scope<_Derived_buffer> Create(buffer_traits::element_type* elements, buffer_traits::size_type size);

	private:

	};

	template<class _Derived_buffer>
	class Buffer<_Derived_buffer, _Index_buffer_tag>
	{
	public:

		static_assert(std::is_class<_Derived_buffer>::value, "Derived buffer should be a custom class");
	
	public:

		using buffer_traits		= _Default_buffer_traits<_Index_buffer_tag>;

		using this_type			= _Derived_buffer;
		using this_pointer_type = this_type*;

		using tag_type			= buffer_traits::tag_type;
		using resource_type		= buffer_traits::resource_type;
		using element_type		= buffer_traits::element_type;
		using size_type			= buffer_traits::size_type;
		using buffer_type		= buffer_traits::buffer_type;
		using usage_type		= buffer_traits::usage_type;

	public:

		virtual					~Buffer() { }

		virtual void			Bind() const = 0;
		virtual void			Unbind() const = 0;

		virtual resource_type	GetNativeHandle() const = 0;
		virtual buffer_type		GetBufferType() const = 0;
		virtual usage_type		GetUsage() const = 0;
		virtual size_type		GetSize() const = 0;

		virtual bool			IsValid() const = 0;

		static Scope<this_type> Create(element_type* elements, size_type size)
		{
			return BufferBuilder<this_type, _Index_buffer_tag>().Create(elements, size);
		}

	private:

		friend class BufferBuilder<this_type, tag_type>;
	};
}


