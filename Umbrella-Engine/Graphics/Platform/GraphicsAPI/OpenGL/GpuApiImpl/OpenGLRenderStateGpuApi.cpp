#include "GpuDeviceData.h"

#include "../../GpuApiRenderState.h"
#include "../../GpuApiInputLayout.h"
#include "../../GpuApiTexture.h"
#include "../../GpuApiBuffer.h"
#include "../../GpuApiShader.h"



#define VERTEX_SHADER_NOT_SET_ERR "Vertex shader is not set."
#define PIXEL_SHADER_NOT_SET_ERR "Pixel shader is not set."
#define VERTEX_PIXEL_SHADERS_NOT_SET_ERR "Vertex and pixel shaders are not set."
#define HULL_SHADER_NOT_SET_ERR "Hull shader is not set."
#define GEOMETRY_SHADER_NOT_SET_ERR "Geometry shader is not set."

#define VERTEX_SHADER_COMPILATION_ERR "Failed to compile vertex shader."
#define PIXEL_SHADER_COMPILATION_ERR "Failed to compile pixel shader."
#define HULL_SHADER_COMPILATION_ERR "Failed to compile hull shader."
#define GEOMETRY_SHADER_COMPILATION_ERR "Failed to compile geometry shader."

#define COLOR_TARGETS_NOT_SET_ERR "Color targets must be set to proceed."
#define DEPTH_TARGET_NOT_SET_ERR "Depth targets must be set to proceed."
#define STENCIL_TARGET_NOT_SET_ERR "Stencil targets must be set to proceed."

#define NULL_DEPTH_TARGET_ERR "Attaching null depth target when UseDefaultRenderTargets flag is unset."
#define NULL_STENCIL_TARGET_ERR "Attaching null stencil target when UseDefaultRenderTargets flag is unset."
#define NULL_COLOR_TARGET_ERR "Attaching null color target when UseDefaultRenderTargets flag is unset."

#define INPUT_LAYOUT_NOT_SET_ERR "Input layout must be set before cooking vertex buffers."
#define INPUT_LAYOUT_UNLOAD_ERR "Try to reload input layout."
#define INPUT_LAYOUT_NOT_COOKED_ERR "Input layout must be loaded before cooking vertex buffers."

#define PROGRAM_LINK_ERR "Failed to link shader program."
#define PROGRAM_VALIATION_ERR "Program validation failed."

#define VERTEX_BUFFERS_NOT_SET_ERR "Vertex buffers must be set."
#define VERTEX_BUFFERS_NOT_COOKED_ERR "Vertex buffers must be cooked."

#define RENDER_TARGETS_AMBIGUITY_ERR "Either UseDefaultRenderTargets flag or one of render targets must be set for rendering."

#define PIPELINE_NOT_COOKED_ERR "Rendering pipeline is not ready."




namespace UE::Graphics::GpuApi::Err
{
	static String RenderStateErrorLog = "";
}

namespace UE::Graphics::GpuApi
{

	static GLenum Map( EShaderType type )
	{
		static HashMap< EShaderType, GLenum > ShaderTypeTable =
		{
			{ EShaderType::VERTEX_SHADER, GL_VERTEX_SHADER },
			{ EShaderType::PIXEL_SHADER, GL_FRAGMENT_SHADER },
			{ EShaderType::TESSELATION_SHADER, GL_TESS_CONTROL_SHADER },	// #todo DOESN'T MAKE SENSE FOR NOW
			{ EShaderType::COMPUTE_SHADER, GL_COMPUTE_SHADER },
			{ EShaderType::GEOMETRY_SHADER, GL_GEOMETRY_SHADER },
		};

		return ShaderTypeTable[type];
	}

	static bool CheckFramebufferCompleteness( GLuint framebuffer, GLenum Target )
	{
		GLenum status = OpenGLContext::CheckNamedFramebufferStatus( framebuffer, Target );

#define CASE_LABEL( invalid_code )																				\
		case JF_CONCATENATE2( GL_, invalid_code ):																\
			Err::RenderStateErrorLog = JF_STRINGIFY( "Framebuffer is incomplete. Error: ", #invalid_code );		\
			return false

#define CASE_LABEL_SUCCESS( code )																				\
		case JF_CONCATENATE2( GL_, code ):																		\
			return true

		switch ( status )
		{
			CASE_LABEL( FRAMEBUFFER_UNDEFINED );
			CASE_LABEL( FRAMEBUFFER_INCOMPLETE_ATTACHMENT );
			CASE_LABEL( FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT );
			CASE_LABEL( FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER );
			CASE_LABEL( FRAMEBUFFER_INCOMPLETE_READ_BUFFER );
			CASE_LABEL( FRAMEBUFFER_UNSUPPORTED );
			CASE_LABEL( FRAMEBUFFER_INCOMPLETE_MULTISAMPLE );
			CASE_LABEL( FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS );

		default:
			return true;
		}

#undef CASE_LABEL
#undef CASE_LABEL_SUCCESS

	}

	static bool LogGLProgramErrors( GLuint resource, GLenum request )
	{
		GLint result;
		OpenGLContext::GetProgramiv( resource, request, &result );

		if ( result == GL_FALSE )
		{
			GLint length;
			OpenGLContext::GetProgramiv( resource, GL_INFO_LOG_LENGTH, &length );

			String log;
			log.resize( length );

			OpenGLContext::GetProgramInfoLog( resource, length, &length, &log[0] );

			Err::RenderStateErrorLog = std::move( log );

			return true;
		}

		return false;
	}

	static bool CookInputLayoutInternal( SRenderStateData& stateData )
	{
		stateData.Description.RenderState.InputLayoutCooked = 0;

		if ( !stateData.Description.RenderState.InputLayoutSet )
		{
			stateData.CookingFailureReason = INPUT_LAYOUT_NOT_SET_ERR;
			return false;
		}

		if ( !GpuApi::IsInputLayoutLoaded( stateData.Description.InputLayout ) )
		{
			stateData.CookingFailureReason = INPUT_LAYOUT_UNLOAD_ERR;
			return false;
		}

		stateData.Description.RenderState.InputLayoutCooked = 1;

		return true;
	}

	static bool CookColorTargetsInternal( SRenderStateData& stateData )
	{
		auto& dd = GetDeviceData();

		if ( stateData.Description.RenderState.UseDefaultRenderTargets )
		{
			stateData.Description.RenderState.ColorTargetsCooked = 1;
			return true;
		}

		OpenGLContext::BindFramebuffer( GL_FRAMEBUFFER, stateData.FramebufferResource );

		for ( uint32 i = 0; i < stateData.Description.ColorTargets.size(); ++i )
		{
			auto& target = stateData.Description.ColorTargets[i];

			if ( !target.RenderTarget )
			{
				break;
			}

			const auto& resource = dd.TextureResources.Data( target.RenderTarget ).Resource;

			OpenGLContext::NamedFramebufferTexture( stateData.FramebufferResource, GL_COLOR_ATTACHMENT0 + i, resource, 0);
		}

		OpenGLContext::UnbindFramebuffer( GL_FRAMEBUFFER );

		stateData.Description.RenderState.ColorTargetsCooked = 1;

		return true;
	}

	static bool CookColorTargetInternal( SRenderStateData& stateData, uint8 index )
	{
		auto& dd = GetDeviceData();

		JF_ASSERT( index < SRenderStateDesc::MAX_RENDER_TARGETS, "index exceeds maximum render targets count" );

		auto renderTarget = stateData.Description.ColorTargets[index].RenderTarget;

		if ( renderTarget )
		{
			auto textureResource = dd.TextureResources.Data( renderTarget ).Resource;

			// 2D IMAGES ONLY
			OpenGLContext::NamedFramebufferTexture( stateData.FramebufferResource, GL_COLOR_ATTACHMENT0 + index, textureResource, 0 );
		}
		
		return true;
	}

	static bool CookDepthTargetInternal( SRenderStateData& stateData )
	{
		auto& dd = GetDeviceData();
	
		stateData.Description.RenderState.DepthTargetCooked = 0;

		if ( stateData.Description.DepthTarget.RenderTarget )
		{
			auto targetResource = dd.TextureResources.Data( stateData.Description.DepthTarget.RenderTarget ).Resource;

			OpenGLContext::NamedFramebufferTexture( stateData.FramebufferResource, GL_DEPTH_ATTACHMENT, targetResource, 0 );
			stateData.Description.RenderState.DepthTargetCooked = 1;
		}

		stateData.Description.RenderState.DepthTargetCooked = 1;

		return true;
	}

	static bool CookStencilTargetInternal( SRenderStateData& stateData )
	{
		auto& dd = GetDeviceData();

		stateData.Description.RenderState.StencilTargetCooked = 0;

		if ( stateData.Description.StencilTarget.RenderTarget )
		{
			auto targetResource = dd.TextureResources.Data( stateData.Description.StencilTarget.RenderTarget ).Resource;

			OpenGLContext::NamedFramebufferTexture( stateData.FramebufferResource, GL_DEPTH_ATTACHMENT, targetResource, 0 );
			stateData.Description.RenderState.StencilTargetCooked = 1;
		}

		stateData.Description.RenderState.StencilTargetCooked = 1;

		return true;
	}


	template< class _Range > requires ( FwdIterableSizedRange< _Range, SRenderStateDesc::RenderTargetUnit > )
	static bool SetColorTargetsInternal( SRenderStateData& stateData, _Range&& targets, bool cook )
	{
		auto& deviceData = GetDeviceData();

		JF_ASSERT( targets.size() > 0, "Trying to set 0 colors." );
		JF_ASSERT( targets.size() <= stateData.Description.MAX_RENDER_TARGETS, "Too many color targets" );

		if ( cook ) OpenGLContext::BindFramebuffer( GL_FRAMEBUFFER, stateData.FramebufferResource );

		for ( uint32 i = 0; i < targets.size(); ++i )
		{
			if ( !IsResourceAllocated( targets[i].RenderTarget ) )
			{
				// #todo HALT
				continue;
			}

			GpuApi::SafeRelease( stateData.Description.ColorTargets[i].RenderTarget );

			stateData.Description.ColorTargets[i] = targets[i];
			GpuApi::AddRef( targets[i].RenderTarget );
			
			if ( cook )
			{
				auto textureResource = deviceData.TextureResources.Data( targets[i].RenderTarget ).Resource;

				// 2D IMAGES ONLY
				OpenGLContext::NamedFramebufferTexture( stateData.FramebufferResource, GL_COLOR_ATTACHMENT0 + i, textureResource, 0 );
			}
		}

		stateData.Description.RenderState.ColorTargetsSet = 1;

		if ( cook )
		{
			OpenGLContext::UnbindFramebuffer( GL_FRAMEBUFFER );

			stateData.Description.RenderState.ColorTargetsCooked = 1;
		}
		else
		{
			stateData.Description.RenderState.ColorTargetsCooked = 0;
		}


		return true;
	}

	static bool SetDepthTargetInternal( SRenderStateData& stateData, SRenderStateDesc::RenderTargetUnit target, bool cook )
	{
		auto& dd = GetDeviceData();

		GpuApi::SafeRelease( stateData.Description.DepthTarget.RenderTarget );

		stateData.Description.DepthTarget = target;
		GpuApi::AddRefIfValid( stateData.Description.DepthTarget.RenderTarget );

		stateData.Description.RenderState.DepthTargetSet = 1;

		stateData.Description.RenderState.DepthTargetCooked = 0;

		if ( cook )
		{
			if ( IsResourceAllocated( target.RenderTarget ) )
			{
				const auto targetResource = dd.TextureResources.Data( target.RenderTarget ).Resource; 
				OpenGLContext::NamedFramebufferTexture( stateData.FramebufferResource, GL_DEPTH_ATTACHMENT, targetResource, 0 );
				stateData.Description.RenderState.DepthTargetCooked = 1;
				stateData.Description.RenderState.UseDefaultRenderTargets = 0;
			}
			else
			{
				// #todo HALT - attaching empty depth target
				// detach depth buffer
				OpenGLContext::NamedFramebufferTexture( stateData.FramebufferResource, GL_DEPTH_ATTACHMENT, 0, 0 );
				stateData.Description.RenderState.DepthTargetCooked = 0;
				stateData.CookingFailureReason = NULL_DEPTH_TARGET_ERR;

				return false;
			}
		}

		return true;
	}

	static bool SetStencilTargetInternal( SRenderStateData& stateData, SRenderStateDesc::RenderTargetUnit target, bool cook )
	{
		auto& dd = GetDeviceData();

		GpuApi::SafeRelease( stateData.Description.StencilTarget.RenderTarget );

		stateData.Description.StencilTarget = target;
		GpuApi::AddRefIfValid( stateData.Description.StencilTarget.RenderTarget );

		stateData.Description.RenderState.StencilTargetSet = 1;

		stateData.Description.RenderState.StencilTargetCooked = 0;

		if ( cook )
		{
			if ( IsResourceAllocated( target.RenderTarget ) )
			{
				const auto targetResource = dd.TextureResources.Data( target.RenderTarget ).Resource;
				OpenGLContext::NamedFramebufferTexture( stateData.FramebufferResource, GL_STENCIL_ATTACHMENT, targetResource, 0 );
				stateData.Description.RenderState.StencilTargetCooked = 1;
				stateData.Description.RenderState.UseDefaultRenderTargets = 0;
			}
			else
			{
				// #todo HALT - attaching empty stencil target
				// detach depth buffer
				OpenGLContext::NamedFramebufferTexture( stateData.FramebufferResource, GL_STENCIL_ATTACHMENT, 0, 0 );
				stateData.Description.RenderState.StencilTargetCooked = 0;
				stateData.CookingFailureReason = NULL_STENCIL_TARGET_ERR;

				return false;
			}
		}


		return true;
	}


	static bool CompileShadersInternal( SRenderStateData& data )
	{

#define COMPILE_SHADER_REQUIRED( descName, macroName )													\
		do																								\
		{																								\
			if ( !data.Description.descName )															\
			{																							\
				Err::RenderStateErrorLog = JF_CONCATENATE2( macroName, _NOT_SET_ERR );					\
				data.Description.RenderState.ShadersCompiled = 0;										\
				return false;																			\
			}																							\
			else																						\
			{																							\
				if ( !IsShaderCompiled( data.Description.descName ) &&									\
					 !CompileShader( data.Description.descName ) )										\
				{																						\
					Err::RenderStateErrorLog = JF_CONCATENATE2( macroName, _COMPILATION_ERR );			\
					data.Description.RenderState.ShadersCompiled = 0;									\
					return false;																		\
				}																						\
			}																							\
		} while (0)

#define COMPILE_SHADER_OPTIONAL( descName, macroName )													\
		do																								\
		{																								\
			if ( data.Description.descName &&															\
				 !IsShaderCompiled( data.Description.descName ) &&										\
				 !CompileShader( data.Description.descName ) )											\
			{																							\
				Err::RenderStateErrorLog = JF_CONCATENATE2( macroName, _COMPILATION_ERR );				\
				data.Description.RenderState.ShadersCompiled = 0;										\
				return false;																			\
			}																							\
		} while (0)


		COMPILE_SHADER_REQUIRED( VertexShader, VERTEX_SHADER );
		COMPILE_SHADER_REQUIRED( PixelShader, PIXEL_SHADER );
		
		COMPILE_SHADER_OPTIONAL( GeometryShader, GEOMETRY_SHADER );
		COMPILE_SHADER_OPTIONAL( HullShader, HULL_SHADER );
		
		
#undef COMPILE_SHADER_REQUIRED
#undef COMPILE_SHADER_OPTIONAL

		data.Description.RenderState.ShadersCompiled = 1;

		return true;
	}

	static bool CookShadersInternal( SRenderStateData& stateData )
	{
		auto& dd = GetDeviceData();

		if ( !stateData.Description.VertexShader && !stateData.Description.PixelShader )
		{
			stateData.CookingFailureReason = VERTEX_PIXEL_SHADERS_NOT_SET_ERR;
		}
		else if ( !stateData.Description.VertexShader )
		{
			stateData.CookingFailureReason = VERTEX_SHADER_NOT_SET_ERR;
		}
		else if ( !stateData.Description.PixelShader )
		{
			stateData.CookingFailureReason = PIXEL_SHADER_NOT_SET_ERR;
		}

		if ( !stateData.Description.RenderState.ShadersSet )
		{
			return false;
		}


		if ( !CompileShadersInternal( stateData ) )
		{
			stateData.CookingFailureReason = std::move( Err::RenderStateErrorLog );
			return false;
		}

		// attach shaders to created program

		stateData.Description.RenderState.ShadersCooked = 0;

		const auto shaderList = { stateData.Description.VertexShader,
								  stateData.Description.GeometryShader,
								  stateData.Description.TessControlShader,
								  stateData.Description.HullShader,
								  stateData.Description.ComputeShader,
								  stateData.Description.PixelShader };

		for ( const auto& shaderRef : shaderList )
		{
			if ( !shaderRef.IsNull() )	// #todo make normal bool conversion
			{
				const auto shaderResource = dd.ShaderResources.Data( shaderRef ).Resource;
				OpenGLContext::AttachShader( stateData.ProgramResource, shaderResource );
			}
		}

		OpenGLContext::LinkProgram( stateData.ProgramResource );

		if ( LogGLProgramErrors( stateData.ProgramResource, GL_LINK_STATUS ) )
		{
			stateData.CookingFailureReason = std::move( Err::RenderStateErrorLog );
			return false;
		}

		OpenGLContext::ValidateProgram( stateData.ProgramResource );

		if ( LogGLProgramErrors( stateData.ProgramResource, GL_VALIDATE_STATUS ) )
		{
			stateData.CookingFailureReason = std::move( Err::RenderStateErrorLog );
			return false;
		}

		// detach shaders from cooked program

		for ( const auto& shaderRef : shaderList )
		{
			if ( !shaderRef.IsNull() )	// #todo make normal boolean conversion
			{
				const auto shaderResource = dd.ShaderResources.Data( shaderRef ).Resource;
				OpenGLContext::DetachShader( stateData.ProgramResource, shaderResource );
			}
		}

		stateData.Description.RenderState.ShadersCooked = 1;

		return true;
	}

	template< class _Range > requires ( FwdIterableSizedRange< _Range, ShaderRef > )
	static bool SetShadersInternal( SRenderStateData& stateData, _Range&& shaders, bool cook )
	{
		auto& dd = GetDeviceData();

		for ( const auto shaderRef : shaders )
		{
			const auto shaderDesc = GetShaderDesc( shaderRef );
			auto shaderType = shaderDesc.Type;

			switch ( shaderType )
			{
				case EShaderType::VERTEX_SHADER:
				{
					GpuApi::SafeRelease( stateData.Description.VertexShader );
					
					stateData.Description.VertexShader = shaderRef;

					GpuApi::AddRefIfValid( stateData.Description.VertexShader );
					
					break;
				}

				case EShaderType::PIXEL_SHADER:
				{
					GpuApi::SafeRelease(stateData.Description.PixelShader );

					stateData.Description.PixelShader = shaderRef;

					GpuApi::AddRefIfValid( stateData.Description.PixelShader );
					
					break;
				}

				case EShaderType::GEOMETRY_SHADER:
				{
					GpuApi::SafeRelease( stateData.Description.GeometryShader );

					stateData.Description.GeometryShader = shaderRef;
					
					GpuApi::AddRefIfValid( stateData.Description.GeometryShader );
					
					break;
				}

				case EShaderType::HULL_SHADER:
				{
					GpuApi::SafeRelease( stateData.Description.HullShader );

					stateData.Description.HullShader = shaderRef;
					
					GpuApi::AddRefIfValid( stateData.Description.HullShader );
					
					break;
				}

				// #todo and other types when certain features will be supported

				default:
					break;
			}

		}

		if ( !stateData.Description.VertexShader || !stateData.Description.PixelShader )
		{
			stateData.Description.RenderState.ShadersSet = 0;
		}
		else
		{
			stateData.Description.RenderState.ShadersSet = 1;
		}

		if ( cook )
		{
			if ( !stateData.Description.VertexShader && !stateData.Description.PixelShader )
			{
				stateData.CookingFailureReason = VERTEX_PIXEL_SHADERS_NOT_SET_ERR;
			}
			else if ( !stateData.Description.VertexShader )
			{
				stateData.CookingFailureReason = VERTEX_SHADER_NOT_SET_ERR;
			}
			else
			{
				stateData.CookingFailureReason = PIXEL_SHADER_NOT_SET_ERR;
			}
		}

		if ( !stateData.Description.RenderState.ShadersSet )	// no matter cook we or not, shaders should be set
		{
			return false;
		}

		if ( !cook )
		{
			return true;
		}


		return CookShadersInternal( stateData );
	}

	static bool CookVertexBuffersInternal( SRenderStateData& data )
	{
		if ( !data.Description.RenderState.InputLayoutCooked )
		{
			data.CookingFailureReason = INPUT_LAYOUT_NOT_COOKED_ERR;
			return false;
		}

		if ( !data.Description.RenderState.VertexBuffersSet )
		{
			data.CookingFailureReason = VERTEX_BUFFERS_NOT_SET_ERR;
			return false;
		}

		data.Description.RenderState.VertexBuffersCooked = 0;

		auto& dd = GetDeviceData();

		for ( uint32 i = 0; i < data.Description.VertexBuffers.size(); ++i )
		{
			auto& bufferUnit = data.Description.VertexBuffers[i];

			if ( !bufferUnit.Buffer )
			{
				continue;
			}

			const auto& layoutData = dd.InputLayouts.Data( data.Description.InputLayout );

			const auto& bufData = dd.BufferResources.Data( bufferUnit.Buffer );

			OpenGLContext::VertexArrayVertexBuffer( layoutData.Resource, bufferUnit.InputSlot, bufData.Resource, bufferUnit.Offset,
													layoutData.Description.InputSlots[ bufferUnit.InputSlot ].Stride );
		}

		data.Description.RenderState.VertexBuffersCooked = 1;

		return true;
	}


	// #todo TURN-OFF AUTO-MIPMAP GENERATIONS IF NEEDED

	RenderStateRef AllocateRenderState()
	{
		auto& deviceData = GetDeviceData();

		RenderStateRef ref = deviceData.RenderStateResources.Create();

		if ( !ref )
		{
			// #todo HALT
			return ref;
		}

		auto& data = deviceData.RenderStateResources.Data( ref );

		OpenGLContext::CreateFramebuffers( 1, &data.FramebufferResource );
		/*
		OpenGLContext::BindFramebuffer( GL_FRAMEBUFFER, data.FramebufferResource );

		OpenGLContext::UnbindFramebuffer( GL_FRAMEBUFFER );*/

		data.ProgramResource = OpenGLContext::CreateProgram();

		if ( data.FramebufferResource == OpenGLContext::GInvalidGLResource )
		{
			// #todo HALT
			return RenderStateRef::Null();
		}

		if ( data.ProgramResource == OpenGLContext::GInvalidGLResource )
		{
			// #todo HALT
			return RenderStateRef::Null();
		}


		return ref;
	}

	RenderStateRef CreateRenderState( const String& debugName )
	{
		auto& deviceData = GetDeviceData();

		RenderStateRef ref = AllocateRenderState();

		if ( !ref )
		{
			return ref;
		}

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		stateData.Description.DebugName = debugName;

		return ref;
	}

	RenderStateRef CreateRenderState( SRenderSetup setup, const String& debugName )
	{
		auto& deviceData = GetDeviceData();

		RenderStateRef ref = CreateRenderState( debugName );

		if ( !ref )
		{
			return ref;
		}

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		UpdateRenderState( ref, setup, false );
		
		return ref;
	}


	bool IsResourceAllocated( RenderStateRef ref )
	{
		return ref && GetDeviceData().RenderStateResources.IsInUse( ref );
	}


	void DestroyRenderState( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		SafeRelease( stateData.Description.IndexBuffer );
		SafeRelease( stateData.Description.InputLayout );

		for ( auto shaderRef : { stateData.Description.VertexShader,
								 stateData.Description.GeometryShader,
								 stateData.Description.TessControlShader,
								 stateData.Description.HullShader,
								 stateData.Description.ComputeShader,
								 stateData.Description.PixelShader,
							   } )
		{
			SafeRelease( shaderRef );
		}

		for ( auto colorTarget : stateData.Description.ColorTargets )
		{
			SafeRelease( colorTarget.RenderTarget );
		}

		SafeRelease( stateData.Description.DepthTarget.RenderTarget );
		SafeRelease( stateData.Description.StencilTarget.RenderTarget );

		stateData.Description.RenderParams.Data = 0;
		stateData.Description.RenderState.Data = 0;

		int32 newCount = SafeRelease( ref );
		JF_ASSERT( newCount == 0, "Failed to destroy render state. It is used somewhere else." );
	}


	RenderStateRef CloneRenderState( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		SRenderSetup setup
		(
			stateData.Description.InputLayout,
			
			Array< ShaderRef, 6 > { stateData.Description.VertexShader,
								     stateData.Description.GeometryShader, 
								     stateData.Description.TessControlShader, 
								     stateData.Description.HullShader, 
								     stateData.Description.ComputeShader, 
								     stateData.Description.PixelShader },

			( bool )stateData.Description.RenderState.UseDefaultRenderTargets,
			stateData.Description.ColorTargets,
			stateData.Description.DepthTarget,
			stateData.Description.StencilTarget,
			stateData.Description.VertexBuffers,
			stateData.Description.IndexBuffer
		);

		RenderStateRef clone = CreateRenderState( setup, stateData.Description.DebugName );

		if ( !clone )
		{
			// #todo HALT
			return clone;
		}

		auto& cloneData = deviceData.RenderStateResources.Data( clone );

		cloneData.Description.RenderParams = stateData.Description.RenderParams;
		cloneData.Description.RenderState = stateData.Description.RenderState;
		
		if ( stateData.Description.eState == stateData.Description.COOKED )
		{
			if ( !CookRenderState( clone ) )
			{
				// #todo HALT
			}
		}


		return clone;
	}

	bool UpdateRenderState( RenderStateRef ref, SRenderSetup setup, bool cookInPlace )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		SetRenderStateInputLayout( ref, setup.InputLayout );

#define CHECK( instruction )\
		do { if ( !instruction ) { return false; } } while( 0 )

#define CHECK_LOG( data, instruction )\
		do { if ( !instruction ) { data.CookingFailureReason = std::move( Err::RenderStateErrorLog ); return false; } } while( 0 )

		if ( !setup.bUseDefaultTarges )
		{
			CHECK( SetColorTargetsInternal( stateData, setup.ColorTargets, cookInPlace ) );
			
			// #todo make it out whether these below should be optional or not

			// depth target is optional
			if ( setup.DepthTarget.RenderTarget )
			{
				CHECK( SetDepthTargetInternal( stateData, setup.DepthTarget, cookInPlace ) );
			}
			
			// stencil target is optional
			if ( setup.StencilTarget.RenderTarget )
			{
				CHECK( SetStencilTargetInternal( stateData, setup.StencilTarget, cookInPlace ) );
			}
		
			stateData.Description.RenderState.UseDefaultRenderTargets = 0;
		}

		CHECK( SetShadersInternal( stateData, setup.Shaders, cookInPlace ) );

#undef CHECK
#undef CHECK_LOG


		// all the previous checks passed so we can safely draw the final conclusion
		stateData.Description.eState = ( cookInPlace ) ? SRenderStateDesc::COOKED : SRenderStateDesc::UNCOOKED;
		
		return true;
	}

	SRenderStateDesc GetRenderStateDesc( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		return stateData.Description;
	}

	bool CookRenderState( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		stateData.Description.RenderState.RenderPipelineCooked = 0;

		/**
		 * 1. Cook input layout.
		 * 2. Cook shaders.
		 * 3. Cook vertex buffers.
		 * 4. Cook framebuffer is needed.
		 * 
		 * 
		 */

		if ( !stateData.Description.RenderState.InputLayoutCooked && !CookInputLayoutInternal( stateData ) )
		{
			return false;
		}

		if ( !stateData.Description.RenderState.ShadersCooked && !CookShadersInternal( stateData ) )
		{
			return false;
		}

		if ( !stateData.Description.RenderState.VertexBuffersCooked && !CookVertexBuffersInternal( stateData ) )
		{
			return false;
		}

		if ( !stateData.Description.RenderState.UseDefaultRenderTargets &&
				(
					( !stateData.Description.RenderState.ColorTargetsCooked  && !CookColorTargetsInternal( stateData ) ) ||
					( !stateData.Description.RenderState.DepthTargetCooked   && !CookDepthTargetInternal( stateData ) )  ||
					( !stateData.Description.RenderState.StencilTargetCooked && !CookStencilTargetInternal( stateData ) )
				)
		   )
		{
			return false;
		}

		stateData.Description.RenderState.RenderPipelineCooked = 1;

		return true;
	}

	bool VerifyRenderState( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		auto& status = stateData.Description.RenderState;

		if ( !status.InputLayoutSet )
		{
			stateData.CookingFailureReason = INPUT_LAYOUT_NOT_SET_ERR;
			return false;
		}

		if ( !status.ShadersSet )
		{
			if ( !stateData.Description.VertexShader && !stateData.Description.PixelShader )
			{
				stateData.CookingFailureReason = VERTEX_PIXEL_SHADERS_NOT_SET_ERR;
			}
			else if ( !stateData.Description.VertexShader )
			{
				stateData.CookingFailureReason = VERTEX_SHADER_NOT_SET_ERR;
			}
			else
			{
				stateData.CookingFailureReason = PIXEL_SHADER_NOT_SET_ERR;
			}

			return false;
		}

		if ( !status.VertexBuffersSet )
		{
			stateData.CookingFailureReason = VERTEX_BUFFERS_NOT_SET_ERR;
			return false;
		}

		bool ambiguityErr = !(
							   ( status.UseDefaultRenderTargets ||
							   // at least one of buffers should be set if use default UseDefaultRenderTargets flag is not set
							   ( status.ColorTargetsSet || status.DepthTargetSet || status.StencilTargetSet ) )
							 );


		if ( ambiguityErr )
		{
			stateData.CookingFailureReason = RENDER_TARGETS_AMBIGUITY_ERR;
			return false;
		}


		return true;
	}


	/**
	 * Does all necessary stuff before rendering.
	 */
	bool SetupRenderState( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		/**
		 * 1. Check if pipeline is ready: RenderPipelineCooked.
		 * 2. Bind VAO.
		 * 3. Upload shader program onto the gpu.
		 * 4. Bind textures ( samples ).
		 * 5. Bind custom framebuffer or default one. 
		 * 6. Bind index buffer if presented.
		 * 7. Set general params if needed ( depth testing, blending, etc ... ).
		 */

		if ( !stateData.Description.RenderState.RenderPipelineCooked )
		{
			stateData.CookingFailureReason = PIPELINE_NOT_COOKED_ERR;
			return false;
		}

		const auto& layoutData = deviceData.InputLayouts.Data( stateData.Description.InputLayout );

		OpenGLContext::BindVertexArray( layoutData.Resource );

		OpenGLContext::UseProgram( stateData.ProgramResource );

		// #todo bind samples
		
		if ( stateData.Description.RenderState.UseDefaultRenderTargets )
		{
			OpenGLContext::UnbindFramebuffer( GL_FRAMEBUFFER );
		}
		else
		{
			OpenGLContext::BindFramebuffer( GL_FRAMEBUFFER, stateData.FramebufferResource );
		}

		if ( stateData.Description.IndexBuffer )
		{
			GpuApi::BindBuffer( stateData.Description.IndexBuffer );
		}

		// #todo general params

		return true;
	}

	bool SetRenderStateShaders( RenderStateRef ref, JSpan< const ShaderRef > shaders, bool cookInPlace )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		return SetShadersInternal( stateData, shaders, cookInPlace );
	}

	void SetRenderStateInputLayout( RenderStateRef ref, InputLayoutRef layout )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		GpuApi::SafeRelease( stateData.Description.InputLayout );

		stateData.Description.InputLayout = layout;

		GpuApi::AddRefIfValid( stateData.Description.InputLayout );

		if ( !IsResourceAllocated( layout ) )
		{
			stateData.Description.eState = SRenderStateDesc::UNCOOKED;
			stateData.Description.RenderState.InputLayoutSet = 0;
		}
		else
		{
			stateData.Description.RenderState.InputLayoutSet = 1;
		}
	}

	bool SetRenderStateColorTargets( RenderStateRef ref, JSpan< const SRenderStateDesc::RenderTargetUnit > targets, bool cookInPlace )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		return SetColorTargetsInternal( stateData, targets, cookInPlace );
	}


	bool SetRenderStateColorTarget( RenderStateRef ref, SRenderStateDesc::RenderTargetUnit target, uint8 index, bool cookInPlace )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		JF_ASSERT( index < SRenderStateDesc::MAX_RENDER_TARGETS, "index exceeds maximum render targets count" );

		GpuApi::SafeRelease( stateData.Description.ColorTargets[index].RenderTarget );

		stateData.Description.ColorTargets[index] = target;
		
		GpuApi::AddRefIfValid( stateData.Description.ColorTargets[index].RenderTarget );

		if ( !cookInPlace )
		{
			return true;
		}


		return CookColorTargetInternal( stateData, index );
	}

	bool SetRenderStateDepthTarget( RenderStateRef ref, SRenderStateDesc::RenderTargetUnit target, bool cookInPlace )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		return SetDepthTargetInternal( stateData, target, cookInPlace );
	}

	bool SetRenderStateStencilTarget( RenderStateRef ref, SRenderStateDesc::RenderTargetUnit target, bool cookInPlace )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		return SetStencilTargetInternal( stateData, target, cookInPlace );
	}

	bool SetRenderStateVertexBuffers( RenderStateRef ref, JSpan< const SRenderStateDesc::BufferUnit > buffers, bool cookInPlace )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		// #todo add input slots validation like in directX somehow ?

		JF_ASSERT( buffers.size() < SRenderStateDesc::MAX_VERTEX_BUFFERS, "Too many vertex buffers" );

		if ( cookInPlace && !stateData.Description.RenderState.InputLayoutSet )
		{
			stateData.CookingFailureReason = INPUT_LAYOUT_NOT_SET_ERR;
			return false;
		}

		if ( cookInPlace && !stateData.Description.RenderState.InputLayoutCooked )
		{
			stateData.CookingFailureReason = INPUT_LAYOUT_NOT_COOKED_ERR;
			return false;
		}

		const auto& layoutData = deviceData.InputLayouts.Data( stateData.Description.InputLayout );

		for ( uint32 i = 0; i < buffers.size(); ++i )
		{
			auto bufferUnit = buffers[i];

			if ( !bufferUnit.Buffer )
			{
				// #todo WARNING LOG
				continue;
			}

			JF_ASSERT( bufferUnit.InputSlot < SInputLayoutDesc::SlotsCount, "Too big slot number." );

			GpuApi::SafeRelease( stateData.Description.VertexBuffers[i].Buffer );

			stateData.Description.VertexBuffers[i] = bufferUnit;
			
			GpuApi::AddRefIfValid( stateData.Description.VertexBuffers[i].Buffer );


			if ( cookInPlace )
			{
				const auto& bufData = deviceData.BufferResources.Data( bufferUnit.Buffer );

				OpenGLContext::VertexArrayVertexBuffer( layoutData.Resource, bufferUnit.InputSlot, bufData.Resource,
														layoutData.Description.InputSlots[ bufferUnit.InputSlot ].Offset,
														layoutData.Description.InputSlots[ bufferUnit.InputSlot ].Stride );
			}
		}

		stateData.Description.RenderState.VertexBuffersSet = 1;
		stateData.Description.RenderState.VertexBuffersCooked = uint32( cookInPlace );


		return true;
	}

	void SetRenderStateIndexBuffer( RenderStateRef ref, BufferRef buffer )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		GpuApi::SafeRelease( stateData.Description.IndexBuffer );

		stateData.Description.IndexBuffer = buffer;

		GpuApi::AddRefIfValid( stateData.Description.IndexBuffer );
	}

	void SetRenderStateDebugName( RenderStateRef ref, const String& debugName )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		stateData.Description.DebugName = debugName;
	}


	ShaderRef GetRenderStateShader( RenderStateRef ref, EShaderType type )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

#define CASE_LABEL( type, name )				\
		case EShaderType::type:					\
			return stateData.Description.name


		switch ( type )
		{
			CASE_LABEL( VERTEX_SHADER, VertexShader );
			CASE_LABEL( PIXEL_SHADER, PixelShader );
			CASE_LABEL( TESSELATION_SHADER, TessControlShader );
			CASE_LABEL( GEOMETRY_SHADER, GeometryShader );
			CASE_LABEL( HULL_SHADER, HullShader );

		default:
			FATAL_ASSERT( false, "Unsupported shader type." );
		}

#undef CASE_LABEL

	}

	BufferRef GetRenderStateIndexBuffer( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		return stateData.Description.IndexBuffer;
	}

	JSpan< const SRenderStateDesc::RenderTargetUnit > GetRenderStateColorTargets( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		return JSpan< const SRenderStateDesc::RenderTargetUnit >( stateData.Description.ColorTargets.begin(),
																  SRenderStateDesc::MAX_RENDER_TARGETS );
	}

	JSpan< const SRenderStateDesc::BufferUnit > GetRenderStateVertexBuffers( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		return JSpan< const SRenderStateDesc::BufferUnit >( stateData.Description.VertexBuffers.begin(),
															SRenderStateDesc::MAX_VERTEX_BUFFERS );
	}

	SRenderStateDesc::URenderParams	GetRenderStateParams( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		return stateData.Description.RenderParams;
	}

	SRenderStateDesc::URenderState GetRenderStateStatus( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		return stateData.Description.RenderState;
	}

	InputLayoutRef GetRenderStateInputLayout( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		return stateData.Description.InputLayout;
	}

	SRenderStateDesc::RenderTargetUnit GetRenderStateColorTarget( RenderStateRef ref, uint8 index )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		JF_ASSERT( index < SRenderStateDesc::MAX_RENDER_TARGETS, "Too big index." );

		return stateData.Description.ColorTargets[index];
	}

	SRenderStateDesc::RenderTargetUnit GetRenderStateDepthTarget( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		return stateData.Description.DepthTarget;
	}

	SRenderStateDesc::RenderTargetUnit GetRenderStateStencilTarget( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT(deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist");

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		return stateData.Description.StencilTarget;
	}


	String	GetRenderStateCookingFailureReason( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );
		
		return stateData.CookingFailureReason;
	}


	void UseRenderStateDefaultRenderTargets( RenderStateRef ref, bool use )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		stateData.Description.RenderState.UseDefaultRenderTargets = ( uint32 )use;
	}

	String GetRenderStateDebugName( RenderStateRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.RenderStateResources.IsInUse( ref ), "render state does not exist" );

		auto& stateData = deviceData.RenderStateResources.Data( ref );

		return stateData.Description.DebugName;
	}
}


