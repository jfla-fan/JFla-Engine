#include "GpuDeviceData.h"
#include "../../GpuApiCommon.h"




namespace UE::Graphics::GpuApi
{
	SDeviceData* GDeviceData = NullPtr;

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

	template< typename _ResType > void ReleaseResourceInternal( uint32 );
	template< typename _ResType > void ReleaseResourceInternal( uint32, uint32 );
	template< typename _ResType > void ReleaseResourceInternal( uint32, uint32, uint32 );
	template< typename _ResType > void ReleaseResourceInternal( uint32, uint32, uint32, uint32 );


	template<>
	void ReleaseResourceInternal< TextureRef >( uint32 textureId )
	{
		OpenGLContext::DeleteTextures( 1, &textureId );
	}

	template<>
	void ReleaseResourceInternal< ShaderRef >( uint32 shaderId )
	{
		OpenGLContext::DeleteShader( shaderId );
	}

	template<>
	void ReleaseResourceInternal< BufferRef >( uint32 bufferId )
	{
		OpenGLContext::DeleteBuffers( 1, &bufferId );
	}

	template<>
	void ReleaseResourceInternal< InputLayoutRef >( uint32 vaoId )
	{
		OpenGLContext::DeleteVertexArrays( 1, &vaoId );
	}

	template<>
	void ReleaseResourceInternal< RenderStateRef >( uint32 framebufferId, uint32 programId )
	{
		OpenGLContext::DeleteFramebuffers( 1, &framebufferId );
		OpenGLContext::DeleteProgram( programId );
	}


#define DEFINE_DEFAULT_ADDREF_FUNCTION( ResRefType, ResContainer )								\
	void AddRef( const ResRefType& ref )														\
	{																							\
		auto& deviceData = GetDeviceData();														\
																								\
		JF_ASSERT( !deviceData.bDeviceShutDone, "Device is shut." );							\
		JF_ASSERT( deviceData.ResContainer.IsInUse( ref ), "Resource not used" );				\
																								\
		deviceData.ResContainer.IncRefCount( ref );												\
	}


#define DEFINE_DEFAULT_RELEASE_FUNCTION1( ResRefType, ResContainer, Res1 )						\
	int32 Release( const ResRefType& ref )														\
	{																							\
		auto& deviceData = GetDeviceData();														\
																								\
		JF_ASSERT( !deviceData.bDeviceShutDone, "Device is shut." );							\
		JF_ASSERT( deviceData.ResContainer.IsInUse( ref ), "Resource not used." );				\
																								\
		auto res1 = deviceData.ResContainer.Data( ref ).Res1;									\
		int32 newCount = deviceData.ResContainer.DecRefCount( ref );							\
																								\
		if ( newCount == 0 )																	\
		{																						\
			ReleaseResourceInternal< ResRefType >( res1 );										\
			deviceData.ResContainer.Destroy( ref );												\
		}																						\
																								\
		return newCount;																		\
	}

#define DEFINE_DEFAULT_RELEASE_FUNCTION2( ResRefType, ResContainer, Res1, Res2 )				\
	int32 Release( const ResRefType& ref )														\
	{																							\
		auto& deviceData = GetDeviceData();														\
																								\
		JF_ASSERT( !deviceData.bDeviceShutDone, "Device is shut." );							\
		JF_ASSERT( deviceData.ResContainer.IsInUse( ref ), "Resource not used." );				\
																								\
		auto res1 = deviceData.ResContainer.Data( ref ).Res1;									\
		auto res2 = deviceData.ResContainer.Data( ref ).Res2;									\
																								\
		int32 newCount = deviceData.ResContainer.DecRefCount( ref );							\
																								\
		if ( newCount == 0 )																	\
		{																						\
			ReleaseResourceInternal< ResRefType >( res1, res2 );								\
			deviceData.ResContainer.Destroy( ref );												\
		}																						\
																								\
		return newCount;																		\
	}

#define DEFINE_DEFAULT_RELEASE_FUNCTION3( ResRefType, ResContainer, Res1, Res2, Res3 )			\
	int32 Release( const ResRefType& ref )														\
	{																							\
		auto& deviceData = GetDeviceData();														\
																								\
		JF_ASSERT( !deviceData.bDeviceShutDone, "Device is shut." );							\
		JF_ASSERT( deviceData.ResContainer.IsInUse( ref ), "Resource not used." );				\
																								\
		auto res1 = deviceData.ResContainer.Data( ref ).Res1;									\
		auto res2 = deviceData.ResContainer.Data( ref ).Res2;									\
		auto res3 = deviceData.ResContainer.Data( ref ).Res3;									\
																								\
		int32 newCount = deviceData.ResContainer.DecRefCount( ref );							\
																								\
		if ( newCount == 0 )																	\
		{																						\
			ReleaseResourceInternal< ResRefType >( res1, res2, res3 );							\
			deviceData.ResContainer.Destroy( ref );												\
		}																						\
																								\
		return newCount;																		\
	}

#define DEFINE_DEFAULT_RELEASE_FUNCTION4( ResRefType, ResContainer, Res1, Res2, Res3, Res4 )	\
	int32 Release( const ResRefType& ref )														\
	{																							\
		auto& deviceData = GetDeviceData();														\
																								\
		JF_ASSERT( !deviceData.bDeviceShutDone, "Device is shut." );							\
		JF_ASSERT( deviceData.ResContainer.IsInUse( ref ), "Resource not used." );				\
																								\
		auto res1 = deviceData.ResContainer.Data( ref ).Res1;									\
		auto res2 = deviceData.ResContainer.Data( ref ).Res2;									\
		auto res3 = deviceData.ResContainer.Data( ref ).Res3;									\
		auto res4 = deviceData.ResContainer.Data( ref ).Res4;									\
																								\
		int32 newCount = deviceData.ResContainer.DecRefCount( ref );							\
																								\
		if ( newCount == 0 )																	\
		{																						\
			ReleaseResourceInternal< ResRefType >( res1, res2, res3, res4 );					\
			deviceData.ResContainer.Destroy( ref );												\
		}																						\
																								\
		return newCount;																		\
	}

#define DEFINE_DEFAULT_REF_FUNCTIONS1( ResRefType, ResContainer, Res1 )							\
	DEFINE_DEFAULT_ADDREF_FUNCTION( ResRefType,   ResContainer )								\
	DEFINE_DEFAULT_RELEASE_FUNCTION1( ResRefType,  ResContainer, Res1 )

#define DEFINE_DEFAULT_REF_FUNCTIONS2( ResRefType, ResContainer, Res1, Res2 )					\
	DEFINE_DEFAULT_ADDREF_FUNCTION( ResRefType,   ResContainer )								\
	DEFINE_DEFAULT_RELEASE_FUNCTION2( ResRefType,  ResContainer, Res1, Res2 )

#define DEFINE_DEFAULT_REF_FUNCTIONS3( ResRefType, ResContainer, Res1, Res2, Res3 )				\
	DEFINE_DEFAULT_ADDREF_FUNCTION( ResRefType,   ResContainer )								\
	DEFINE_DEFAULT_RELEASE_FUNCTION3( ResRefType,  ResContainer, Res1, Res2, Res3 )

#define DEFINE_DEFAULT_REF_FUNCTIONS4( ResRefType, ResContainer, Res1, Res2, Res3, Res4 )		\
	DEFINE_DEFAULT_ADDREF_FUNCTION( ResRefType,   ResContainer )								\
	DEFINE_DEFAULT_RELEASE_FUNCTION4( ResRefType,  ResContainer, Res1, Res2, Res3, Res4 )



	DEFINE_DEFAULT_REF_FUNCTIONS1( BufferRef, BufferResources, Resource )

	DEFINE_DEFAULT_REF_FUNCTIONS1( TextureRef, TextureResources, Resource )
	DEFINE_DEFAULT_REF_FUNCTIONS1( InputLayoutRef, InputLayouts, Resource )
	DEFINE_DEFAULT_REF_FUNCTIONS1( ShaderRef, ShaderResources, Resource )

	DEFINE_DEFAULT_REF_FUNCTIONS2( RenderStateRef, RenderStateResources, FramebufferResource, ProgramResource )


	// #todo - implement other apis later
	
	//DEFINE_DEFAULT_REF_FUNCTIONS(SamplerRef, TextureResources)
	


#undef DEFINE_DEFAULT_REF_FUNCTIONS

}