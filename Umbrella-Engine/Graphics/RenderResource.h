#pragma once

#include "../Misc/Resource.h"
#include "../STL/String.h"



namespace UE::Graphics
{

	class JRenderResource : public JResource
	{
	public:

		using TRenderPtr = Ref< JRenderResource >;

	protected:

		virtual ~JRenderResource() { };

	public:

		JRenderResource() { }

		virtual JString GetDebugName() = 0;

		// calculate video memory used by resource
		virtual uint32 GetUsedVideoMemory() = 0;
	};

}



