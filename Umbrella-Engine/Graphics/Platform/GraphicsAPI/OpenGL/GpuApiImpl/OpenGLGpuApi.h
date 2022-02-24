#pragma once

#include "../../GpuTypes.h"
#include "../../GpuApiCommon.h"
#include "../OpenGLContext.h"



namespace UE::Graphics::GpuApi
{

	struct SDeviceData;	// GpuDeviceData.h



	struct SBufferData
	{

		SBufferDesc Description;

		GLuint		Resource;		// GPU descriptor

		bool		bIsBound;

		SBufferData()
			: Description()
			, Resource( OpenGLContext::GInvalidGLResource )
			, bIsBound( false )
		{
		}

	};

	struct SShaderData
	{

		SShaderDesc Description;

		GLuint Resource;

		String CompilationFailureMessage;

		SShaderData()
			: Description()
			, Resource( OpenGLContext::GInvalidGLResource )
			, CompilationFailureMessage( "" )
		{
		}

	};
	
	struct STextureData
	{

		STextureDesc	Description;

		GLuint			Resource;

		GLuint			ActiveId;

		bool			bIsBound;

		STextureData()
			: Description()
			, Resource( OpenGLContext::GInvalidGLResource )
			, ActiveId( 0 )
			, bIsBound( false )
		{
		}

	};
	
	struct SInputLayoutData
	{
		GLuint Resource;				// Vertex Array Object

		uint32 AttributeIndex;

		bool   bLoaded;

		SInputLayoutDesc Description;
	
		SInputLayoutData()
			: Resource( OpenGLContext::GInvalidGLResource )
			, AttributeIndex( 0 )
			, bLoaded( false )
			, Description()
		{
		}
	};

	struct SRenderStateData
	{
		SRenderStateDesc	Description;

		GLuint				FramebufferResource;		// FBO - framebuffer object

		GLuint				ProgramResource;			// Program object - result of compiling and linking shaders.

		String				CookingFailureReason;
	};


	ShaderRef	AllocateShader( EShaderType type );

	BufferRef	AllocateBuffer();

	TextureRef	AllocateTexture();

	InputLayoutRef AllocateInputLayout();

	RenderStateRef AllocateRenderState();
}


