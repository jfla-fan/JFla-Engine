#include "GpuDeviceData.h"
#include "../../GpuApi.h"

// #todo
/*********************************************************************************************************************/
/* IS IT A GOOD IDEA TO SPLIT DIFFERENT APIS (BUFFER, TEXTURE, etc.) INTO DIFFERENT CPP FILES OR LEAVE IT AS IT IS ? */
/*********************************************************************************************************************/


namespace J::Graphics::GpuApi
{
	extern SDeviceData* GDeviceData;

	bool InitDevice()
	{
		GDeviceData = new SDeviceData();

		return true;
	}

	// helper functions, inline implementations

	SDeviceData& GetDeviceData()
	{
		auto& deviceData = *GpuApi::GDeviceData;

		if (!deviceData.bDeviceInitDone)
		{
			// #todo HALT - not initialized
		}

		return deviceData;
	}

	EGPUVendor GetGPUVendor()
	{
		const auto& deviceData = GetDeviceData();

		return deviceData.eGPUVendor;
	}

	EDeviceType GetDeviceType()
	{
		const auto& deviceData = GetDeviceData();

		return deviceData.eDevice;
	}


#define DEFINE_DEFAULT_REF_FUNCTIONS( ResRefType, ResContainer )					\
	void AddRef( const ResRefType& ref )											\
	{																				\
		auto& deviceData = GetDeviceData();											\
																					\
		JF_ASSERT( !deviceData.bDeviceShutDone, "Device is shut." );				\
		JF_ASSERT( deviceData.ResContainer.IsInUse( ref ), "Resource not used" );	\
																					\
		deviceData.ResContainer.IncRefCount( ref );									\
	}																				\
																					\
	int32 Release( const ResRefType& ref )											\
	{																				\
		auto& deviceData = GetDeviceData();											\
																					\
		JF_ASSERT( !deviceData.bDeviceShutDone, "Device is shut." );				\
		JF_ASSERT( deviceData.ResContainer.IsInUse( ref ), "Resource not used." );	\
																					\
		int32 newCount = deviceData.ResContainer.DecRefCount( ref );				\
																					\
		if ( newCount == 0 )														\
		{																			\
			deviceData.ResContainer.Destroy( ref );									\
		}																			\
																					\
		return newCount;															\
	}

	DEFINE_DEFAULT_REF_FUNCTIONS(BufferRef, BufferResources)

	// #todo - implement other apis later
	
	//DEFINE_DEFAULT_REF_FUNCTIONS(TextureRef, TextureResources)
	//DEFINE_DEFAULT_REF_FUNCTIONS(ShaderRef, ShaderResources)
	//DEFINE_DEFAULT_REF_FUNCTIONS(VertexArrayRef, VertexArrayResources)


#undef DEFINE_DEFAULT_REF_FUNCTIONS



	static GLbitfield ToGLFlags( EBufferAccessBits bits )
	{
		GLbitfield flags = 0;

		if ( bits & EBufferAccessBits::WRITE )
		{
			flags |= GL_DYNAMIC_STORAGE_BIT;
		}

		if ( bits & EBufferAccessBits::DYNAMIC_READ )
		{
			flags |= GL_MAP_READ_BIT;
		}

		if ( bits & EBufferAccessBits::DYNAMIC_WRITE )
		{
			flags |= GL_MAP_WRITE_BIT;
		}

		return flags;
	}

	BufferRef AllocateBuffer()
	{
		auto& deviceData = GetDeviceData();

		BufferRef ref = deviceData.BufferResources.Create();
		
		if ( ref.IsNull() )
		{
			/* #TODO HALT HERE */
			return ref;
		}

		auto& bufData = deviceData.BufferResources.Data( ref );

		OpenGLContext::GenBuffers( 1, &bufData.Resource );

		// Have we allocated real resource on the gpu?
		if ( bufData.Resource == OpenGLContext::GInvalidGLResource )
		{
			// #TODO HALT
			deviceData.BufferResources.Destroy( ref );
			return BufferRef::Null();
		}

		return ref;
	}

	BufferRef CreateBuffer(	EBufferType type,
							EBufferUsage usage,
							EBufferAccessBits access,
							const JString& debugName )
	{
		auto& deviceData = GetDeviceData();
	
		BufferRef ref = AllocateBuffer();

		if ( ref.IsNull() )
		{
			return ref;
		}

		auto& bufData = deviceData.BufferResources.Data( ref );

		bufData.Description.eType = type;
		bufData.Description.eUsage = usage;
		bufData.Description.eAccess = access;

		if ( type != EBufferType::NONE )
		{
			OpenGLContext::BindBuffer( GL_ENUM( type ), bufData.Resource );
			//OpenGLContext::UnbindBuffer( GL_ENUM( type ) );	#TODO SHOULD WE?
		}

		if ( !debugName.empty() )
		{
			OpenGLContext::ObjectLabel( GL_BUFFER, bufData.Resource, debugName.length(), debugName.c_str() );
			bufData.Description.DebugName = debugName;
		}

		return ref;
	}

	BufferRef CreateBuffer( const SBufferInitData& initData )
	{
		auto& deviceData = GetDeviceData();

		const auto& [ Type, Usage, Offset, DebugName, Size, Data, Access ] = initData;

		BufferRef ref = CreateBuffer( Type, Usage, Access, DebugName );

		if ( ref.IsNull() )
		{
			return ref;
		}

		auto& bufData = deviceData.BufferResources.Data( ref );

		// trying to upload data
		if ( Size > 0 )
		{
			if ( Type != EBufferType::NONE && Usage != EBufferUsage::NONE )
			{
				OpenGLContext::BufferData( GL_ENUM( Type ), Size, Data, GL_ENUM( Usage ) );
				bufData.Description.Size = Size;
				bufData.Description.Offset = Offset;
				bufData.Description.bStorageAllocated = true;
			}
			else
			{
				// #todo HALT not possible to upload data without type and usage
				bufData.Description.bStorageAllocated = false;
			}
		}

		return ref;
	}

	void DestroyBuffer( BufferRef ref )
	{
		int32 newCount = SafeRelease( ref );
		JF_ASSERT( newCount == 0, "Cannot destroy buffer. It used used somewhere else." );
	}

	void BindBuffer( BufferRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.BufferResources.IsInUse( ref ), "buffer does not exist" );
	
		auto& bufData = deviceData.BufferResources.Data( ref );

		if ( bufData.Description.eType == EBufferType::NONE )
		{
			// #TODO HALT HERE
			return;
		}

		OpenGLContext::BindBuffer( GL_ENUM( bufData.Description.eType ), bufData.Resource );
		bufData.Description.bBoundToCurrentContext = true;
	}

	void UnbindBuffer( BufferRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.BufferResources.IsInUse( ref ), "buffer does not exist" );

		auto& bufData = deviceData.BufferResources.Data( ref );

		if ( bufData.Description.eType == EBufferType::NONE )
		{
			// #TODO HALT HERE
			return;
		}

		OpenGLContext::UnbindBuffer( GL_ENUM( bufData.Description.eType ) );
		bufData.Description.bBoundToCurrentContext = false;
	}

	bool IsBufferBound( BufferRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.BufferResources.IsInUse( ref ), "buffer does not exist" );

		auto& bufData = deviceData.BufferResources.Data( ref );

		return bufData.Description.bBoundToCurrentContext;
	}

	SBufferDesc GetBufferDesc( BufferRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.BufferResources.IsInUse( ref ), "buffer does not exist" );

		return deviceData
						.BufferResources
						.Data( ref )
						.Description;
	}

	void AllocateBufferStorage( BufferRef  ref, SIZE_T size,
								SIZE_T offset /* = 0 */, MemPtr data /* = NullPtr */,
								EBufferAccessBits accessBits /* = EBufferAccessBits::ALL */ )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.BufferResources.IsInUse( ref ), "buffer does not exist" );

		auto& bufData = deviceData.BufferResources.Data( ref );

		if ( bufData.Description.eType == EBufferType::NONE )
		{
			// #TODO HALT, can proceed with no type
		}

		if ( bufData.Description.eUsage == EBufferUsage::NONE )
		{
			// #TODO HALT
			return;
		}

		if ( !( bufData.Description.eAccess & EBufferAccessBits::REALLOCATE ) )
		{
			// #TODO HALT
			return;
		}

		if ( size == 0 )
		{
			// #TODO HALT
			return;
		}

		if ( accessBits & EBufferAccessBits::REALLOCATE )
		{
			OpenGLContext::NamedBufferData( bufData.Resource, size, data, GL_ENUM( bufData.Description.eUsage ) );
		}
		else
		{
			OpenGLContext::NamedBufferStorage( bufData.Resource, size, data, ToGLFlags( accessBits ) );
		}

		bufData.Description.eAccess = accessBits;
		bufData.Description.Size = size;
		bufData.Description.Offset = offset;
		bufData.Description.bStorageAllocated = true;
	}

	MemPtr ReadBufferData( BufferRef ref, SIZE_T offset, SIZE_T size )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.BufferResources.IsInUse( ref ), "buffer does not exist" );

		auto& bufData = deviceData.BufferResources.Data( ref );

		if ( !( bufData.Description.eAccess & EBufferAccessBits::READ ) )
		{
			// #TODO HALT
			return NullPtr;
		}

		if ( size == 0 )
		{
			// #TODO HALT
			return NullPtr;
		}

		if ( !bufData.Description.bStorageAllocated )
		{
			// #todo HALT
			return NullPtr;
		}

		// #TODO USE MEMORY POOL
		MemPtr allocated = new byte[size];

		OpenGLContext::GetNamedBufferSubData( bufData.Resource, offset, size, allocated );

		return allocated;
	}
		   
	void ReadBufferData( BufferRef ref, SIZE_T offset, SIZE_T size, MemPtr storage )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.BufferResources.IsInUse( ref ), "buffer does not exist" );

		auto& bufData = deviceData.BufferResources.Data( ref );

		if ( !( bufData.Description.eAccess & EBufferAccessBits::READ ) )
		{
			// #TODO HALT
			return;
		}

		if ( size == 0 )
		{
			// #TODO HALT
			return;
		}

		if ( !bufData.Description.bStorageAllocated )
		{
			// #todo HALT
			return;
		}

		OpenGLContext::GetNamedBufferSubData( bufData.Resource, offset, size, storage );
	}
	
	MemPtr ReadBufferData( BufferRef ref, SIZE_T size )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.BufferResources.IsInUse( ref ), "buffer does not exist" );

		auto& bufData = deviceData.BufferResources.Data( ref );

		return ReadBufferData( ref, bufData.Description.Offset, size );
	}

	void ReadBufferData( BufferRef ref, SIZE_T size, MemPtr storage )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.BufferResources.IsInUse( ref ), "buffer does not exist" );

		auto& bufData = deviceData.BufferResources.Data( ref );

		ReadBufferData(ref, bufData.Description.Offset, size, storage);
	}

	MemPtr ReadBufferData( BufferRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT ( deviceData.BufferResources.IsInUse( ref ), "buffer does not exist" );

		auto& bufData = deviceData.BufferResources.Data( ref );

		return ReadBufferData( ref, bufData.Description.Offset, bufData.Description.Size );
	}

	void ReadBufferData( BufferRef ref, MemPtr storage )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.BufferResources.IsInUse( ref ), "buffer does not exist" );

		auto& bufData = deviceData.BufferResources.Data( ref );

		ReadBufferData( ref, bufData.Description.Offset, bufData.Description.Size, storage );
	}

	void CopyBufferData( BufferRef srcRef, BufferRef dstRef, SIZE_T srcOffset, SIZE_T dstOffset, SIZE_T size )
	{
		auto& deviceData = GetDeviceData();

		JF_ASSERT( deviceData.BufferResources.IsInUse( srcRef ), "buffer does not exist" );
		JF_ASSERT( deviceData.BufferResources.IsInUse( dstRef ), "buffer does not exist" );

		auto& srcBufData = deviceData.BufferResources.Data( srcRef );
		auto& dstBufData = deviceData.BufferResources.Data( dstRef );


		if ( !srcBufData.Description.bStorageAllocated )
		{
			// #TODO HALT
			return;
		}

		if ( dstBufData.Description.bStorageAllocated )
		{
			// #TODO HALT
			return;
		}

		JF_ASSERT( dstBufData.Description.eAccess & EBufferAccessBits::WRITE, "Writing to destination buffer is not permitted." );

		JF_ASSERT( srcBufData.Description.Size >= srcOffset + size, "Trying to read too many bytes from a source buffer." );
		JF_ASSERT( dstBufData.Description.Size >= dstOffset + size, "Trying to write too many bytes from a source buffer.");


		OpenGLContext::BindBuffer( GL_COPY_READ_BUFFER, srcBufData.Resource );
		OpenGLContext::BindBuffer( GL_COPY_WRITE_BUFFER, dstBufData.Resource );

		OpenGLContext::CopyNamedBufferSubData( srcBufData.Resource, dstBufData.Resource, srcOffset, dstOffset, size );

		OpenGLContext::UnbindBuffer( GL_COPY_READ_BUFFER );
		OpenGLContext::UnbindBuffer( GL_COPY_WRITE_BUFFER );
	}

	void WriteBufferData( BufferRef ref, SIZE_T offset, SIZE_T size, MemPtr source )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.BufferResources.IsInUse( ref ), "buffer does not exist" );

		auto& bufData = deviceData.BufferResources.Data( ref );

		JF_ASSERT( bufData.Description.eAccess & EBufferAccessBits::WRITE, "Writing to this buffer is not permitted." );

		if ( !bufData.Description.bStorageAllocated )
		{
			// #TODO HALT
			return;
		}

		JF_ASSERT( bufData.Description.Size >= offset + size, "Trying to write too many bytes into buffer." );

		OpenGLContext::NamedBufferSubData( bufData.Resource, offset, size, source );

	}

	void WriteBufferData( BufferRef ref, SIZE_T size, MemPtr source )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.BufferResources.IsInUse( ref ), "buffer does not exist" );

		auto& bufData = deviceData.BufferResources.Data( ref );

		WriteBufferData( ref, bufData.Description.Offset, size, source );
	}

	void SetBufferDebugName( BufferRef ref, const JString& name )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.BufferResources.IsInUse( ref ), "buffer does not exist" );

		auto& bufData = deviceData.BufferResources.Data( ref );
		
		OpenGLContext::ObjectLabel( GL_BUFFER, bufData.Resource, name.length(), name.c_str() );

		bufData.Description.DebugName = name;
	}

	JString GetBufferDebugName( BufferRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.BufferResources.IsInUse( ref ), "buffer does not exist" );

		auto& bufData = deviceData.BufferResources.Data( ref );

		return bufData.Description.DebugName;
	}

}
