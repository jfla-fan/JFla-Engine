#pragma once

#include "../../../Core.h"
#include "GpuApiInputLayout.h"




namespace UE::Graphics
{
	USE_INPUT_LAYOUT_TYPES()


	class CInputLayout
	{
	public:

	/*	static constexpr uint32 ElementsCount	= SInputLayoutDesc::ElementsCount;

		static constexpr uint32 SlotsCount		= SInputLayoutDesc::SlotsCount;*/

	public:

		CInputLayout( uint32 baseOffset = 0, const String& debugName = "" );

		CInputLayout( uint32 baseOffset, JSpan< const LayoutUnit > elements, const String& debugName = "" );

		~CInputLayout();

		SInputLayoutDesc GetDescription() const;

		void Clear();

		bool IsLoaded() const { return bLoaded; }

		void Load( uint32 baseOffset, JSpan< const LayoutUnit > elements );

		InputLayoutRef GetDescriptor() const { return Descriptor; }

		LayoutSpan GetView() const { return GpuApi::GetInputLayoutView( Descriptor ); }


	private:

		InputLayoutRef Descriptor;

		bool bLoaded;

		uint32 BaseOffset;

	};

}
