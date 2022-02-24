#include "GpuDeviceData.h"
#include "../../GpuApiInputLayout.h"



namespace UE::Graphics::GpuApi
{

	static void LoadInputLayout( SInputLayoutData& data )
	{
		JF_ASSERT( data.Resource != OpenGLContext::GInvalidGLResource, "Invalid GPU resource." );

		for ( const auto unit : data.Description.Elements )
		{
			if ( unit.Element.IsNull() )
			{
				break;
			}

			for ( uint8 i = 0; i < unit.Element.EntriesCount; ++i )
			{
				OpenGLContext::EnableVertexArrayAttrib( data.Resource, data.AttributeIndex );

				// #TODO MAKE OPENGL MAPPING FOR ELEMENT FORMAT AND ITS NORMALIZE FLAG
				OpenGLContext::VertexArrayAttribFormat( data.Resource, data.AttributeIndex, unit.Element.ElementsCount, GL_FLOAT, GL_FALSE,
														data.Description.InputSlots[ unit.InputSlot ].Offset + data.Description.BaseOffset );
				
				data.Description.InputSlots[ unit.InputSlot ].Offset += unit.Element.SizeInBytes / unit.Element.EntriesCount;
				
				OpenGLContext::VertexArrayAttribBinding( data.Resource, data.AttributeIndex++, ( uint32 )unit.InputSlot );
			}
		}

		data.bLoaded = true;
	}

	static void UnloadInputLayout( SInputLayoutData& data )
	{
		JF_ASSERT( data.Resource != OpenGLContext::GInvalidGLResource, "Invalid GPU resource." );

		for ( uint32 i = 0; i < data.AttributeIndex; ++i )
		{
			OpenGLContext::DisableVertexArrayAttrib( data.Resource, i );
		}

		data.AttributeIndex = 0;

		data.bLoaded = false;
	}

	static void ClearInputLayoutIfNecessary(SInputLayoutData& data)
	{
		if (data.bLoaded)
		{
			UnloadInputLayout(data);
			data.Description.Clear();
		}
	}


	InputLayoutRef AllocateInputLayout()
	{
		auto& deviceData = GetDeviceData();

		InputLayoutRef ref = deviceData.InputLayouts.Create();

		if ( !ref )
		{
			// #todo HALT
			return ref;
		}

		auto& layoutData = deviceData.InputLayouts.Data( ref );

		OpenGLContext::CreateVertexArrays( 1, &layoutData.Resource );

		// Have we allocated real resource on the graphics card?
		if ( layoutData.Resource == OpenGLContext::GInvalidGLResource )
		{
			// #TODO HALT
			deviceData.InputLayouts.Destroy( ref );
			return InputLayoutRef::Null();
		}


		return ref;
	}

	InputLayoutRef CreateInputLayout( const String& debugName )
	{
		auto& dd = GetDeviceData();

		InputLayoutRef ref = AllocateInputLayout();

		if ( !ref )
		{
			return ref;
		}

		if ( !debugName.empty() )
		{
			auto& layoutData = dd.InputLayouts.Data( ref );
			layoutData.Description.DebugName = debugName;
			OpenGLContext::ObjectLabel( GL_VERTEX_ARRAY, layoutData.Resource, ( GLsizei )debugName.length(), debugName.c_str() );
		}

		return ref;
	}


	InputLayoutRef CreateInputLayout( uint32 baseOffset, JSpan< const SInputLayoutDesc::LayoutUnit > elements, const String& debugName )
	{
		auto& deviceData = GetDeviceData();

		InputLayoutRef ref = CreateInputLayout( debugName );

		if ( !ref )
		{
			// #todo HALT
			return ref;
		}

		auto& layoutData = deviceData.InputLayouts.Data( ref );

		UpdateInputLayout( ref, baseOffset, elements );

		return ref;
	}


	// expecting that all the render states do not use this layout
	void DestroyInputLayout( InputLayoutRef ref )
	{
		ClearInputLayout( ref );

		int32 newCount = SafeRelease( ref );
		JF_ASSERT( newCount == 0, "Failed to destroy input layout. It is used somewhere else." );
	}


	void ClearInputLayout( InputLayoutRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Layout does not exist" );

		auto& layoutData = deviceData.InputLayouts.Data( ref );
	
		ClearInputLayoutIfNecessary( layoutData );
	}

	bool IsResourceAllocated( InputLayoutRef ref )
	{
		return ref && GetDeviceData().InputLayouts.IsInUse( ref );
	}

	SInputLayoutDesc GetInputLayoutDesc( InputLayoutRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Layout does not exist" );

		auto& layoutData = deviceData.InputLayouts.Data( ref );
	
		return layoutData.Description;
	}

	uint32 GetInputLayoutOffset( InputLayoutRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Layout does not exist" );

		auto& layoutData = deviceData.InputLayouts.Data( ref );

		return layoutData.Description.BaseOffset;
	}

	void UpdateInputLayout( InputLayoutRef ref, uint32 baseOffset, JSpan< const SInputLayoutDesc::LayoutUnit > elements )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Layout does not exist" );

		auto& layoutData = deviceData.InputLayouts.Data( ref );

		ClearInputLayoutIfNecessary( layoutData );

		layoutData.Description.BaseOffset = baseOffset;

		uint32 Count = Math::Min( ( uint32 ) elements.size(), ( uint32 )SInputLayoutDesc::ElementsCount );

		for ( uint32 i = 0; i < Count; ++i )
		{
			JF_ASSERT( elements[i].InputSlot < SInputLayoutDesc::SlotsCount, "Too big slot" );
			
			layoutData.Description.Elements[i] = elements[i];
		}

		layoutData.Description.CalcStrides();
		
		LoadInputLayout( layoutData );
	}

	bool IsInputLayoutLoaded( InputLayoutRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Layout does not exist" );

		auto& layoutData = deviceData.InputLayouts.Data( ref );

		return layoutData.bLoaded;
	}

	SInputLayoutDesc::LayoutSpan GetInputLayoutView( InputLayoutRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Layout does not exist" );

		auto& layoutData = deviceData.InputLayouts.Data( ref );

		return SInputLayoutDesc::LayoutSpan( layoutData.Description.Elements.begin(), SInputLayoutDesc::ElementsCount );
	}

}
