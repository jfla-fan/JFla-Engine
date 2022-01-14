#include "OpenGLIndexBuffer.h"
#include "../GpuApi.h"


namespace J::Graphics
{
	// Buffer Builder

	Scope< OpenGLIndexBuffer >
	BufferBuilder< OpenGLIndexBuffer, _Index_buffer_tag >::
	Create( buffer_traits::element_type* elements, buffer_traits::size_type size, buffer_traits::access_type access )
	{
		return MakeScoped< OpenGLIndexBuffer >( elements, size, access );
	}


	// Buffer Class

	OpenGLIndexBuffer::OpenGLIndexBuffer( uint32* indices, uint32 size, EBufferAccessBits access, const JString& debugName )
		: Size( size )
		, Offset( 0 )
		, eAccess( access )
		, DebugName( debugName )
	{
		auto initData = GpuApi::SBufferInitData
		{
			EBufferType::ELEMENT_ARRAY_BUFFER,
			EBufferUsage::STATIC_DRAW,
			0,
			debugName,
			sizeof(uint32) * size,
			indices,
			access
		};

		this->Descriptor = GpuApi::CreateBuffer( initData );

		JF_ASSERT( !Descriptor.IsNull(), "Failed to create opengl index buffer." );
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		GpuApi::SafeRelease( this->Descriptor );
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer&& another) NOEXCEPT
	{

		this->Descriptor	= another.Descriptor;
		this->Size			= another.Size;
		this->Offset		= another.Offset;
		this->eAccess		= another.eAccess;
		this->DebugName		= std::move( another.DebugName );
	
		another.Descriptor	= resource_type::Null();
		another.Size		= 0;
		another.Offset		= 0;
		another.eAccess		= EBufferAccessBits::NONE;

	}

	OpenGLIndexBuffer& OpenGLIndexBuffer::operator = (OpenGLIndexBuffer&& another) NOEXCEPT
	{
		if (this == &another)
		{
			return *this;
		}

		this->Descriptor = another.Descriptor;
		this->Size = another.Size;
		this->Offset = another.Offset;
		this->eAccess = another.eAccess;
		this->DebugName = std::move(another.DebugName);

		another.Descriptor = resource_type::Null();
		another.Size = 0;
		another.Offset = 0;
		another.eAccess = EBufferAccessBits::NONE;


		return *this;
	}

	INLINE void OpenGLIndexBuffer::Bind() const
	{
		GpuApi::BindBuffer(this->Descriptor);
	}

	INLINE void OpenGLIndexBuffer::Unbind() const
	{
		GpuApi::UnbindBuffer(this->Descriptor);
	}
}
