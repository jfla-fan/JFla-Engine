#pragma once
#include "../Buffer.h"



namespace Engine::Graphics
{

	class OpenGLVertexBuffer : public Buffer<OpenGLVertexBuffer, _Vertex_buffer_tag>
	{
	public:

		OpenGLVertexBuffer(float* vertices, uint32 size, EBufferType type = EBufferType::ARRAY_BUFFER, EBufferUsage usage = EBufferUsage::STATIC_DRAW);
		~OpenGLVertexBuffer();

		OpenGLVertexBuffer(const OpenGLVertexBuffer& another);
		OpenGLVertexBuffer(OpenGLVertexBuffer&& another) NOEXCEPT;

		OpenGLVertexBuffer&		operator = (const OpenGLVertexBuffer& another);
		OpenGLVertexBuffer&		operator = (OpenGLVertexBuffer&& another) NOEXCEPT;

		void					Bind() const override;
		void					Unbind() const override;

		INLINE resource_type	GetNativeHandle() const override { return Resource; }
		INLINE buffer_type		GetBufferType() const override { return Type; }
		INLINE usage_type		GetUsage() const override { return Usage; }
		INLINE size_type		GetSize() const override { return Size; }

		INLINE bool				IsValid() const override { return Resource != OpenGLContext::GInvalidGLResource && Type != EBufferType::NONE && Usage != EBufferUsage::NONE; }

	private:
		
		friend class Buffer<OpenGLVertexBuffer, _Vertex_buffer_tag>;

		resource_type	Resource;
		buffer_type		Type;
		usage_type		Usage;
		size_type		Size;
	};

}



