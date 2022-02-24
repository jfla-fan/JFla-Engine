#pragma once
#include "../../../../../Utils/Time/TimeSpan.h"
#include "../../../../../Utils/Containers/ResourceContainer.h"
#include "OpenGLGpuApi.h"




namespace UE::Graphics::GpuApi
{
	using namespace Utils;

	struct SDeviceCapabilities
	{

		// #TODO What else can GPU support ?! That's a good question!

		bool bRayTracingSupport;

		SDeviceCapabilities()
			: bRayTracingSupport( false )
		{
		}
	};



	struct SDeviceData
	{
		
		/** #TODO - USE SPECIFIC MEMORY POOL */


		SDeviceData()
			: bDeviceInitDone( false )
			, bDeviceShutDone( false )
			, eDevice( EDeviceType::UNKNOWN )
			, eGPUVendor( EGPUVendor::UNKNOWN )
			, Capabilities()
			, BufferResources( AccessMutex )
			, TextureResources( AccessMutex )
			, InputLayouts( AccessMutex )
			, RenderStateResources( AccessMutex )
			, ShaderResources( AccessMutex )
			//, VertexArrayResources( AccessMutex )
			//, TextureResources( AccessMutex )
		{
		}


		bool													bDeviceInitDone;
		bool													bDeviceShutDone;


		SDeviceCapabilities										Capabilities;

		EDeviceType												eDevice;
		EGPUVendor												eGPUVendor;


		// #TODO
		// BufferLayouts, Samplers, FrameBuffers (aka swap chains), RenderBuffers etc ...
		// Consider Pixel Buffers (useful for streaming images)


		ResourceContainer< BufferRef,		SBufferData >		BufferResources;
		ResourceContainer< RenderStateRef,  SRenderStateData >	RenderStateResources;
		//ResourceContainer< VertexArrayRef,	JVertexArray >		VertexArrayResources;
		ResourceContainer< ShaderRef,		SShaderData >		ShaderResources;
		ResourceContainer< TextureRef,		STextureData >		TextureResources;
		ResourceContainer< InputLayoutRef,  SInputLayoutData >  InputLayouts;
		// ResourceContainer< GPU FENCES

		TMutex													AccessMutex;
	};

	extern SDeviceData* GDeviceData;

}
