#pragma once
#include "../../../../../Utils/Time/TimeSpan.h"
#include "../../../../../Utils/Containers/ResourceContainer.h"
#include "OpenGLGpuApi.h"



namespace J::Graphics::GpuApi
{
	struct TextureRef;
	struct ShaderRef;
	struct BufferRef;
	struct VertexArrayRef;

	struct SBufferDesc;
	struct SGpuFenceDesc;

	struct SBufferData;

	enum class EDeviceType : uint8;
	enum class EGPUVendor : uint8;
}


namespace J::Graphics::GpuApi
{
	using namespace Utils;

	struct SDeviceCapabilities
	{

		// #TODO What else can GPU support ?! That's a good question!

		bool bRayTracingSupport;
	};



	struct SDeviceData
	{
		
		/** #TODO - USE SPECIFIC MEMORY POOL */


		SDeviceData()
			: bDeviceInitDone( false )
			, bDeviceShutDone( false )
			, eDevice(EDeviceType::UNKNOWN)
			, eGPUVendor(EGPUVendor::UNKNOWN)
			, BufferResources( AccessMutex )
			//, VertexArrayResources( AccessMutex )
			//, ShaderResources( AccessMutex )
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
		//ResourceContainer< VertexArrayRef,	JVertexArray >		VertexArrayResources;
		//ResourceContainer< ShaderRef,		JShader >			ShaderResources;
		//ResourceContainer< TextureRef,		JTexture >			TextureResources;
		// ResourceContainer< GPU FENCES

		TMutex													AccessMutex;
	};


	SDeviceData* GDeviceData = NullPtr;

}
