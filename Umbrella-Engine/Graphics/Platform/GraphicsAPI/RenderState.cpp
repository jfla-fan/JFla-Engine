#include "RenderState.h"



namespace UE::Graphics::GpuApi
{

	void CRenderState::Release()
	{
		GpuApi::SafeRelease( this->Descriptor );
	}

	CRenderState::CRenderState( RenderStateRef ref, bool cooked, bool setup )
		: Descriptor( ref ), bCooked( cooked ), bSetup( setup)
	{
	}


	CRenderState::CRenderState( const String& debugName )
		: bCooked( false )
		, bSetup( false )
	{
		Descriptor = GpuApi::CreateRenderState( debugName );

		JF_ASSERT( Descriptor, "Failed to create render state." );
	}

	CRenderState::CRenderState( const SRenderSetup& setup, const String& debugName )
		: bCooked( false )
		, bSetup( false )
	{
		Descriptor = GpuApi::CreateRenderState( setup, debugName );

		JF_ASSERT( Descriptor, "Failed to create render state." );
	}

	CRenderState::CRenderState( CRenderState&& another ) NOEXCEPT
	{
		Release();

		this->Descriptor = another.Descriptor;
		this->bCooked = another.bCooked;
		this->bSetup = another.bSetup;

		another.Descriptor = RenderStateRef::Null();
		another.bCooked = false;
		another.bSetup = false;
	}

	CRenderState& CRenderState::operator = ( CRenderState&& another ) NOEXCEPT
	{
		if ( this == &another )
		{
			return *this;
		}

		Release();

		this->Descriptor = another.Descriptor;
		this->bCooked = another.bCooked;
		this->bSetup = another.bSetup;

		another.Descriptor = RenderStateRef::Null();
		another.bCooked = false;
		another.bSetup = false;

		return *this;
	}

	CRenderState::~CRenderState()
	{
		Release();
	}


	Ref< CRenderState > CRenderState::Clone( const CRenderState& renderState )
	{
		RenderStateRef clone = GpuApi::CloneRenderState( renderState.Descriptor );
	
		if ( !clone )
		{
			return NullPtr;
		}

		return Ref< CRenderState >( JF_NEW( CRenderState ) ( clone, renderState.bCooked, renderState.bSetup ) );
	}

	bool CRenderState::IsReadyForCooking() const
	{
		return GpuApi::VerifyRenderState( Descriptor );
	}

	bool CRenderState::Cook()
	{
		if ( !GpuApi::VerifyRenderState( Descriptor) )
		{
			// #todo HALT
			return false;
		}

		bCooked = GpuApi::CookRenderState( Descriptor );
		return bCooked;
	}

	bool CRenderState::Setup()
	{
		if ( !bCooked )
		{
			// #todo HALT
			return false;
		}

		bSetup = GpuApi::SetupRenderState( Descriptor );
		return bSetup;
	}

	String	CRenderState::GetCookingFailureReason() const
	{
		return GpuApi::GetRenderStateCookingFailureReason( Descriptor );
	}

	SRenderStateDesc CRenderState::GetDescription() const
	{
		return GpuApi::GetRenderStateDesc( Descriptor );
	}

	void CRenderState::SetInputLayout( InputLayoutRef inputLayout )
	{
		GpuApi::SetRenderStateInputLayout( Descriptor, inputLayout );
	}

	InputLayoutRef CRenderState::GetInputLayout() const
	{
		return GpuApi::GetRenderStateInputLayout( Descriptor );
	}

	void CRenderState::SetShader( ShaderRef shader )
	{
		GpuApi::SetRenderStateShaders( Descriptor, Array< const ShaderRef, 1 > { shader } );
	}

	ShaderRef CRenderState::GetShader( EShaderType type ) const
	{
		return GpuApi::GetRenderStateShader( Descriptor, type );
	}

	void CRenderState::SetShaders( JSpan< const ShaderRef > shaders )
	{
		GpuApi::SetRenderStateShaders( Descriptor, shaders );
	}

	JVector< ShaderRef > CRenderState::GetShaders( JSpan< const EShaderType > types )
	{
		JVector< ShaderRef > shaders;
		shaders.reserve( types.size() );

		for ( const auto type : types )
		{
			shaders.push_back( GpuApi::GetRenderStateShader( Descriptor, type ) );
		}

		return shaders;
	}

	void CRenderState::SetColorTargets( JSpan< const RenderTargetUnit > targets )
	{
		GpuApi::SetRenderStateColorTargets( Descriptor, targets );
	}

	JSpan< const RenderTargetUnit > CRenderState::GetColorTargets() const
	{
		return GpuApi::GetRenderStateColorTargets( Descriptor );
	}

	void CRenderState::SetDepthTarget( RenderTargetUnit target )
	{
		GpuApi::SetRenderStateDepthTarget( Descriptor, target );
	}

	RenderTargetUnit CRenderState::GetDepthTarget() const
	{
		return GpuApi::GetRenderStateDepthTarget( Descriptor );
	}

	void CRenderState::SetStencilTarget( RenderTargetUnit target )
	{
		GpuApi::SetRenderStateStencilTarget( Descriptor, target );
	}

	RenderTargetUnit CRenderState::GetStencilTarget() const
	{
		return GpuApi::GetRenderStateStencilTarget( Descriptor );
	}

	bool CRenderState::SetVertexBuffers( JSpan< const BufferUnit > buffers )
	{
		return GpuApi::SetRenderStateVertexBuffers( Descriptor, buffers );
	}
	JSpan< const BufferUnit > CRenderState::GetVertexBuffers() const
	{
		return GpuApi::GetRenderStateVertexBuffers( Descriptor );
	}

	void CRenderState::SetIndexBuffer( BufferRef buffer )
	{
		GpuApi::SetRenderStateIndexBuffer( Descriptor, buffer );
	}

	BufferRef CRenderState::GetIndexBuffer() const
	{
		return GpuApi::GetRenderStateIndexBuffer( Descriptor );
	}

	URenderState CRenderState::GetRenderStatus() const
	{
		return GpuApi::GetRenderStateStatus( Descriptor );
	}

	URenderParams CRenderState::GetRenderParams() const
	{
		return GpuApi::GetRenderStateParams( Descriptor );
	}

	String	CRenderState::GetDebugName() const
	{
		return GpuApi::GetRenderStateDebugName( Descriptor );
	}

	void CRenderState::SetDebugName( const String& debugName )
	{
		GpuApi::SetRenderStateDebugName( Descriptor, debugName );
	}

	void CRenderState::UseDefaultRenderTargets( bool use )
	{
		GpuApi::UseRenderStateDefaultRenderTargets( Descriptor, use );
	}



}
