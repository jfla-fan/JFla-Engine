#include "OpenGLVertexBuffer.h"
#include "OpenGLContext.h"



namespace Engine::Graphics
{
	INLINE Scope<OpenGLVertexBuffer> BufferBuilder<OpenGLVertexBuffer, _Vertex_buffer_tag>::Create(buffer_traits::element_type* elements, buffer_traits::size_type size, buffer_traits::buffer_type type, buffer_traits::usage_type usage)
	{
		return MakeScoped<OpenGLVertexBuffer>(elements, size, type, usage);
	}


	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32 size, EBufferType type, EBufferUsage usage)
		: Type(type), Usage(usage), Size(size)
	{
		OpenGLContext::GenBuffers(1, &Resource);
		OpenGLContext::BindBuffer(GL_ENUM(type), Resource);

		OpenGLContext::BufferData(GL_ENUM(type), size, vertices, GL_ENUM(usage));
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		OpenGLContext::DeleteBuffers(1, &Resource);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const OpenGLVertexBuffer& another)
	{
		this->Type = another.Type;
		this->Usage = another.Usage;
	
		// if buffer is invalid generate a new one
		if (this->Resource == OpenGLContext::GInvalidGLResource)
		{
			OpenGLContext::GenBuffers(1, &Resource);
			this->Size = 0;
		}

		// if buffer size is not big enough for containing another buffer data there is a need for allocating new memory
		if (Size < another.Size)
		{
			OpenGLContext::NamedBufferData(this->Resource, another.Size, nullptr, GL_ENUM(this->Usage));
		}
		OpenGLContext::CopyNamedBufferSubData(another.Resource, this->Resource, 0, 0, another.Size);

		this->Size = another.Size;
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer&& another) NOEXCEPT
	{
		this->Resource = another.Resource;
		this->Type = another.Type;
		this->Usage = another.Usage;
		this->Size = another.Size;

		another.Resource = OpenGLContext::GInvalidGLResource;
		another.Type = EBufferType::NONE;
		another.Usage = EBufferUsage::NONE;
		another.Size = 0;
	}

	OpenGLVertexBuffer& OpenGLVertexBuffer::operator = (const OpenGLVertexBuffer& another)
	{
		if (this == &another)
		{
			return *this;
		}

		this->Type = another.Type;
		this->Usage = another.Usage;

		// if buffer is invalid generate a new one
		if (this->Resource == OpenGLContext::GInvalidGLResource)
		{
			OpenGLContext::GenBuffers(1, &Resource);
		}

		// if buffer size is not big enough for containing another buffer data there is a need for allocating new memory
		if (Size < another.Size)
		{
			OpenGLContext::NamedBufferData(this->Resource, another.Size, nullptr, GL_ENUM(this->Usage));
		}
		OpenGLContext::CopyNamedBufferSubData(another.Resource, this->Resource, 0, 0, another.Size);

		this->Size = another.Size;

		return *this;
	}

	OpenGLVertexBuffer& OpenGLVertexBuffer::operator = (OpenGLVertexBuffer&& another) NOEXCEPT
	{
		if (this == &another)
		{
			return *this;
		}

		this->Resource = another.Resource;
		this->Type = another.Type;
		this->Usage = another.Usage;
		this->Size = another.Size;

		another.Resource = OpenGLContext::GInvalidGLResource;
		another.Type = EBufferType::NONE;
		another.Usage = EBufferUsage::NONE;
		another.Size = 0;
	
		return *this;
	}

	INLINE void Engine::Graphics::OpenGLVertexBuffer::Bind() const
	{
		OpenGLContext::BindBuffer(GL_ENUM(Type), Resource);
	}

	INLINE void Engine::Graphics::OpenGLVertexBuffer::Unbind() const
	{
		OpenGLContext::BindBuffer(GL_ENUM(Type), OpenGLContext::GInvalidGLResource);
	}
}

