#pragma once

#include "GpuApiRenderState.h"


namespace UE::Graphics::GpuApi
{
	USE_RENDER_STATE_TYPES()



	class CRenderState
	{
	private:

		RenderStateRef		Descriptor;

		bool				bCooked;
		
		bool				bSetup;

		void Release();

	protected:

		CRenderState( RenderStateRef ref, bool cooked, bool setup );

	public:

		CRenderState( const String& debugName = "" );

		CRenderState( const SRenderSetup& setup, const String& debugName = "" );

		CRenderState( const CRenderState& ) = delete;
		CRenderState& operator = ( const CRenderState& ) = delete;

		CRenderState( CRenderState&& another ) NOEXCEPT;
		CRenderState& operator = ( CRenderState&& another ) NOEXCEPT;

		~CRenderState();


		static Ref< CRenderState >	Clone( const CRenderState& renderState );

		bool						IsReadyForCooking() const;

		bool						Cook();

		bool						Setup();

		String						GetCookingFailureReason() const;

		SRenderStateDesc			GetDescription() const;

		void						SetInputLayout( InputLayoutRef inputLayout );
		InputLayoutRef				GetInputLayout() const;

		void						SetShader( ShaderRef shader );
		ShaderRef					GetShader( EShaderType type ) const;

		void						SetShaders( JSpan< const ShaderRef > shaders );
		JVector< ShaderRef >		GetShaders( JSpan< const EShaderType > types );

		void						SetColorTargets( JSpan< const RenderTargetUnit > targets );
		
		JSpan
		< const RenderTargetUnit >	GetColorTargets() const;

		void						SetDepthTarget( RenderTargetUnit target );
		RenderTargetUnit			GetDepthTarget() const;

		void						SetStencilTarget( RenderTargetUnit target );
		RenderTargetUnit			GetStencilTarget() const;

		bool						SetVertexBuffers( JSpan< const BufferUnit > buffers );
		JSpan< const BufferUnit >	GetVertexBuffers() const;

		void						SetIndexBuffer( BufferRef buffer );
		BufferRef					GetIndexBuffer() const;

		URenderState				GetRenderStatus() const;
		URenderParams				GetRenderParams() const;

		String						GetDebugName() const;
		void						SetDebugName( const String& debugName );

		void						UseDefaultRenderTargets( bool use = true );

	};

}

