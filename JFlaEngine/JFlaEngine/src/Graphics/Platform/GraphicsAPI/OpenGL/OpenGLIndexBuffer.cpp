#include "OpenGLIndexBuffer.h"


namespace Engine::Graphics
{
	// Buffer Builder

	Scope<OpenGLIndexBuffer> BufferBuilder<OpenGLIndexBuffer, _Index_buffer_tag>::Create(buffer_traits::element_type* elements, buffer_traits::size_type size)
	{
		return MakeScoped<OpenGLIndexBuffer>(elements, size);
	}


	// Buffer Class

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32* indices, uint32 size)
		: Size(size)
	{
		OpenGLContext::GenBuffers(1, &Resource);
		OpenGLContext::BindBuffer(GL_ENUM(EBufferType::ELEMENT_ARRAY_BUFFER), Resource);

		OpenGLContext::BufferData(GL_ENUM(EBufferType::ELEMENT_ARRAY_BUFFER), size, indices, GL_ENUM(EBufferUsage::DYNAMIC_DRAW));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		OpenGLContext::DeleteBuffers(1, &Resource);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(const OpenGLIndexBuffer& another)
	{
		// if buffer is invalid generate a new one
		if (this->Resource == OpenGLContext::GInvalidGLResource)
		{
			OpenGLContext::GenBuffers(1, &Resource);
			this->Size = 0;
		}

		// if buffer size is not big enough for containing another buffer data there is a need for allocating new memory
		if (this->Size < another.Size)
		{
			OpenGLContext::NamedBufferData(this->Resource, another.Size, nullptr, GL_ENUM(this->GetUsage()));
		}
		OpenGLContext::CopyNamedBufferSubData(another.Resource, this->Resource, 0, 0, another.Size);
		
		this->Size = another.Size;
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer&& another) NOEXCEPT
	{
		this->Resource = another.Resource;
		this->Size = another.Size;

		another.Resource = OpenGLContext::GInvalidGLResource;
		another.Size = 0;
	}

	OpenGLIndexBuffer& OpenGLIndexBuffer::operator = (const OpenGLIndexBuffer& another)
	{
		if (this == &another)
		{
			return *this;
		}

		// if buffer is invalid generate a new one
		if (this->Resource == OpenGLContext::GInvalidGLResource)
		{
			OpenGLContext::GenBuffers(1, &Resource);
			this->Size = 0;
		}

		// if buffer size is not big enough for containing another buffer data there is a need for allocating new memory
		if (this->Size < another.Size)
		{
			OpenGLContext::NamedBufferData(this->Resource, another.Size, nullptr, GL_ENUM(this->GetUsage()));
		}
		OpenGLContext::CopyNamedBufferSubData(another.Resource, this->Resource, 0, 0, another.Size);

		return *this;
	}

	OpenGLIndexBuffer& OpenGLIndexBuffer::operator = (OpenGLIndexBuffer&& another) NOEXCEPT
	{
		if (this == &another)
		{
			return *this;
		}

		this->Resource = another.Resource;
		this->Size = another.Size;

		another.Resource = OpenGLContext::GInvalidGLResource;
		another.Size = 0;

		return *this;
	}

	INLINE void OpenGLIndexBuffer::Bind() const
	{
		OpenGLContext::BindBuffer(GL_ENUM(EBufferType::ELEMENT_ARRAY_BUFFER), Resource);
	}

	INLINE void OpenGLIndexBuffer::Unbind() const
	{
		OpenGLContext::BindBuffer(GL_ENUM(EBufferType::ELEMENT_ARRAY_BUFFER), OpenGLContext::GInvalidGLResource);
	}
}
