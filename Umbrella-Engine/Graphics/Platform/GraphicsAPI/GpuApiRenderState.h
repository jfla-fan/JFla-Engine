#pragma once

#include "GpuTypes.h"



namespace UE::Graphics::GpuApi
{


	/************************************************************************/
	/*							RENDER STATES                               */
	/************************************************************************/




	RenderStateRef										CreateRenderState( const String& debugName = "" );

	RenderStateRef										CreateRenderState( SRenderSetup setup, const String& debugName = "" );

	bool												IsResourceAllocated( RenderStateRef ref );

	void												DestroyRenderState(RenderStateRef ref );

	/**
	 * Create a new render state with the same parameters.
	 * New render state will be cooked if a given one is cooked.
	 * 
	 * \param ref - Reference to the original render state.
	 */
	RenderStateRef										CloneRenderState( RenderStateRef ref );

	bool												UpdateRenderState( RenderStateRef ref, SRenderSetup setup, bool cookInPlace = false );

	SRenderStateDesc									GetRenderStateDesc( RenderStateRef ref );

	/**
	 * Compile program from given shaders, set flags, do api-specific job.
	 */
	bool												CookRenderState( RenderStateRef ref );

	//< Is render state ready for cooking?
	bool												VerifyRenderState( RenderStateRef ref );

	/**
	 * Does all necessary stuff before rendering.
	 */
	bool												SetupRenderState( RenderStateRef ref );

	bool												SetRenderStateShaders( RenderStateRef ref, JSpan< const ShaderRef > shaders, bool cookInPlace = false );

	ShaderRef											GetRenderStateShader( RenderStateRef ref, EShaderType type );

	void												SetRenderStateInputLayout( RenderStateRef ref, InputLayoutRef layout );

	InputLayoutRef										GetRenderStateInputLayout( RenderStateRef ref );

	bool												SetRenderStateColorTargets( RenderStateRef ref, JSpan< const SRenderStateDesc::RenderTargetUnit > targets, bool cookInPlace = false);

	bool												SetRenderStateColorTarget( RenderStateRef ref, SRenderStateDesc::RenderTargetUnit target, uint8 index, bool cookInPlace = false);

	SRenderStateDesc::RenderTargetUnit					GetRenderStateColorTarget( RenderStateRef ref, uint8 index );

	JSpan< const SRenderStateDesc::RenderTargetUnit >	GetRenderStateColorTargets( RenderStateRef ref );

	bool												SetRenderStateDepthTarget( RenderStateRef ref, SRenderStateDesc::RenderTargetUnit target, bool cookInPlace = false );

	SRenderStateDesc::RenderTargetUnit					GetRenderStateDepthTarget( RenderStateRef ref );

	bool												SetRenderStateStencilTarget( RenderStateRef ref, SRenderStateDesc::RenderTargetUnit target, bool cookInPlace = false );

	SRenderStateDesc::RenderTargetUnit					GetRenderStateStencilTarget( RenderStateRef ref );

	bool												SetRenderStateVertexBuffers( RenderStateRef ref, JSpan< const SRenderStateDesc::BufferUnit > buffers, bool cookInPlace = false );

	JSpan< const SRenderStateDesc::BufferUnit >			GetRenderStateVertexBuffers( RenderStateRef ref );

	void												SetRenderStateIndexBuffer( RenderStateRef ref, BufferRef buffer );

	BufferRef											GetRenderStateIndexBuffer( RenderStateRef ref);

	void												SetRenderStateDebugName( RenderStateRef ref, const String& debugName );

	String												GetRenderStateDebugName( RenderStateRef ref );

	SRenderStateDesc::URenderParams						GetRenderStateParams( RenderStateRef ref );

	SRenderStateDesc::URenderState						GetRenderStateStatus( RenderStateRef ref );

	String												GetRenderStateCookingFailureReason( RenderStateRef ref );

	void												UseRenderStateDefaultRenderTargets( RenderStateRef ref, bool use = true );

	//void												SetViewport( RenderStateRef ref, ViewportDesc viewport );
	
	//ViewportDesc										GetViewport( RenderStateRef ref );

}
