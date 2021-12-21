#pragma once
#include "../Buffer.h"




namespace Engine::Graphics
{

	class OpenGLIndexBuffer : public Buffer<OpenGLIndexBuffer, _Index_buffer_tag>
	{
	public:

		OpenGLIndexBuffer(uint32* indices, uint32 size);
		~OpenGLIndexBuffer();

		OpenGLIndexBuffer(const OpenGLIndexBuffer& another);
		OpenGLIndexBuffer(OpenGLIndexBuffer&& another) NOEXCEPT;

		OpenGLIndexBuffer&			operator = (const OpenGLIndexBuffer& another);
		OpenGLIndexBuffer&			operator = (OpenGLIndexBuffer&& another) NOEXCEPT;

		void					Bind() const override;
		void					Unbind() const override;

		INLINE resource_type	GetNativeHandle() const override { return Resource; }
		INLINE buffer_type		GetBufferType() const override { return EBufferType::ELEMENT_ARRAY_BUFFER; }
		INLINE usage_type		GetUsage() const override { return EBufferUsage::STATIC_DRAW; }
		INLINE size_type		GetSize() const override { return Size; }

		INLINE bool				IsValid() const override { return Resource != OpenGLContext::GInvalidGLResource; }

	private:

		friend class Buffer<OpenGLIndexBuffer, _Index_buffer_tag>;

		resource_type	Resource;
		size_type		Size;
	};



}


