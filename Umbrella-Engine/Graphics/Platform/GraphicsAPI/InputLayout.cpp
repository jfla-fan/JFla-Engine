#include "InputLayout.h"



namespace UE::Graphics
{

	CInputLayout::CInputLayout( uint32 baseOffset, const String& debugName )
		: bLoaded( false )
	{
		Descriptor = GpuApi::CreateInputLayout( debugName );
		this->BaseOffset = baseOffset;

		FATAL_ASSERT( Descriptor, "Failed to create input layout." );
	}

	CInputLayout::CInputLayout( uint32 baseOffset, JSpan< const LayoutUnit > elements, const String& debugName )
		: CInputLayout( baseOffset, debugName )
	{
		Load( baseOffset, elements );
	}

	CInputLayout::~CInputLayout()
	{
		GpuApi::SafeRelease( Descriptor );
	}

	SInputLayoutDesc CInputLayout::GetDescription() const { return GpuApi::GetInputLayoutDesc( Descriptor ); }

	void CInputLayout::Clear()
	{
		GpuApi::ClearInputLayout( Descriptor );
		bLoaded = false;
	}

	void CInputLayout::Load(uint32 baseOffset, JSpan< const LayoutUnit > elements)
	{
		this->BaseOffset = baseOffset;
		GpuApi::UpdateInputLayout( Descriptor, baseOffset, elements );
		bLoaded = true;
	}


}
