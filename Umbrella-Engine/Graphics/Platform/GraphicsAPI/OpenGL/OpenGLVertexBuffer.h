#pragma once
#include "../Buffer.h"



namespace J::Graphics
{

	class OpenGLVertexBuffer final : public Buffer< OpenGLVertexBuffer, _Vertex_buffer_tag >
	{
	public:

		OpenGLVertexBuffer( float* vertices, uint32 size,
							EBufferType type = EBufferType::ARRAY_BUFFER,
							EBufferUsage usage = EBufferUsage::STATIC_DRAW,
							EBufferAccessBits access = GpuApi::EBufferAccessBits::ALL,
							const JString& debugName = "" );

		~OpenGLVertexBuffer() override;

		OpenGLVertexBuffer( OpenGLVertexBuffer&& another ) NOEXCEPT;
		OpenGLVertexBuffer&		operator = ( OpenGLVertexBuffer&& another ) NOEXCEPT;

		void					Bind() const override;
		void					Unbind() const override;

		INLINE resource_type	GetNativeHandle() const override { return Descriptor; }
		INLINE buffer_type		GetBufferType() const override { return eType; }
		INLINE usage_type		GetUsage() const override { return eUsage; }
		INLINE size_type		GetSize() const override { return Size; }
		INLINE size_type		GetOffset() const override { return Offset; }
		INLINE access_type		GetAccess() const override { return eAccess; }

		INLINE bool				IsValid() const override { return !Descriptor.IsNull(); }

	private:

		resource_type	Descriptor;

		buffer_type		eType;

		usage_type		eUsage;

		size_type		Size;

		size_type		Offset;

		access_type		eAccess;

		JString			DebugName;

	};

}



