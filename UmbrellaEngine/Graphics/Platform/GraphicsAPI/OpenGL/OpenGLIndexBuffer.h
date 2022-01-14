#pragma once
#include "../Buffer.h"




namespace J::Graphics
{

	class OpenGLIndexBuffer final : public Buffer<OpenGLIndexBuffer, _Index_buffer_tag>
	{
	public:

		OpenGLIndexBuffer( uint32* indices, uint32 size, EBufferAccessBits access = GpuApi::EBufferAccessBits::ALL, const JString& debugName = "" );
		~OpenGLIndexBuffer() override;

		OpenGLIndexBuffer( OpenGLIndexBuffer&& another ) NOEXCEPT;
		OpenGLIndexBuffer&		operator = ( OpenGLIndexBuffer&& another ) NOEXCEPT;

		void					Bind() const override;
		void					Unbind() const override;

		INLINE resource_type	GetNativeHandle() const override { return Descriptor; }
		INLINE buffer_type		GetBufferType() const override { return EBufferType::ELEMENT_ARRAY_BUFFER; }
		INLINE usage_type		GetUsage() const override { return EBufferUsage::STATIC_DRAW; }
		INLINE size_type		GetSize() const override { return Size; }
		INLINE size_type		GetOffset() const override { return Offset; }
		INLINE access_type		GetAccess() const override { return eAccess; }

		INLINE bool				IsValid() const override { return !Descriptor.IsNull(); }

	private:

		resource_type	Descriptor;

		size_type		Size;

		size_type		Offset;

		access_type		eAccess;

		JString			DebugName;
	};



}


