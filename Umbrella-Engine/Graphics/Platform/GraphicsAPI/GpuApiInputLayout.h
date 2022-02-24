#pragma once

#include "GpuTypes.h"



namespace UE::Graphics::GpuApi
{


	/************************************************************************/
	/*							INPUT LAYOUTS                               */
	/************************************************************************/



	InputLayoutRef		CreateInputLayout( const String& debugName = "" );

	InputLayoutRef		CreateInputLayout( uint32 baseOffset, JSpan< const SInputLayoutDesc::LayoutUnit > elements, const String& debugName = "" );

	void				DestroyInputLayout( InputLayoutRef ref );

	void				ClearInputLayout( InputLayoutRef ref );

	bool				IsResourceAllocated( InputLayoutRef ref );

	SInputLayoutDesc	GetInputLayoutDesc( InputLayoutRef ref );

	uint32				GetInputLayoutOffset( InputLayoutRef ref );

	void				UpdateInputLayout( InputLayoutRef ref, uint32 baseOffset, JSpan< const SInputLayoutDesc::LayoutUnit > elements );

	bool				IsInputLayoutLoaded( InputLayoutRef ref );

	SInputLayoutDesc::
	LayoutSpan			GetInputLayoutView( InputLayoutRef ref );


}
