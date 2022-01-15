#include "OpenGLVertexBuffer.h"
#include "../GpuApi.h"



namespace J::Graphics
{
	INLINE Scope < OpenGLVertexBuffer >
	BufferBuilder < OpenGLVertexBuffer, _Vertex_buffer_tag > ::
	Create( buffer_traits::element_type* elements,
			buffer_traits::size_type size,
			buffer_traits::buffer_type type,
			buffer_traits::usage_type usage,
			buffer_traits::access_type access )
	{
		return MakeScoped < OpenGLVertexBuffer > ( elements, size, type, usage, access );
	}


	OpenGLVertexBuffer::OpenGLVertexBuffer( float* vertices, uint32 size,
											EBufferType type, EBufferUsage usage,
											EBufferAccessBits access, const JString& debugName )
		: eType( type )
		, eUsage( usage )
		, Size( size )
		, eAccess( access )
		, DebugName( debugName )
	{
		GpuApi::SBufferInitData initData =
		{
			type,
			usage,
			0,
			debugName,
			sizeof(float) * size,
			vertices,
			access
		};

		this->Descriptor = GpuApi::CreateBuffer( initData );
		
		JF_ASSERT( !Descriptor.IsNull(), "Failed to create opengl vertex buffer." );
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		GpuApi::SafeRelease(this->Descriptor);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer&& another) NOEXCEPT
	{
		this->Descriptor	= another.Descriptor;
		this->eType			= another.eType;
		this->eUsage		= another.eUsage;
		this->eAccess		= another.eAccess;
		this->Size			= another.Size;
		this->Offset		= another.Offset;
		this->DebugName		= std::move( another.DebugName );

		another.Descriptor	= resource_type::Null();	// GpuApi::Invalidate?
		another.eType		= GpuApi::EBufferType::NONE;
		another.eUsage		= GpuApi::EBufferUsage::NONE;
		another.eAccess		= GpuApi::EBufferAccessBits::NONE;
		another.Size		= 0;
		another.Offset		= 0;
	}

	OpenGLVertexBuffer& OpenGLVertexBuffer::operator = (OpenGLVertexBuffer&& another) NOEXCEPT
	{
		if (this == &another)
		{
			return *this;
		}

		this->Descriptor	= another.Descriptor;
		this->eType			= another.eType;
		this->eUsage		= another.eUsage;
		this->eAccess		= another.eAccess;
		this->Size			= another.Size;
		this->Offset		= another.Offset;
		this->DebugName		= std::move(another.DebugName);

		another.Descriptor	= resource_type::Null();	// GpuApi::Invalidate?
		another.eType		= GpuApi::EBufferType::NONE;
		another.eUsage		= GpuApi::EBufferUsage::NONE;
		another.eAccess		= GpuApi::EBufferAccessBits::NONE;
		another.Size		= 0;
		another.Offset		= 0;
	
		return *this;
	}

	INLINE void OpenGLVertexBuffer::Bind() const
	{
		GpuApi::BindBuffer(this->Descriptor);
	}

	INLINE void OpenGLVertexBuffer::Unbind() const
	{
		GpuApi::UnbindBuffer(this->Descriptor);
	}
}

