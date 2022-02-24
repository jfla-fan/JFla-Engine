#pragma once

#include "../../RenderResource.h"
#include "../../../Common/Common.h"


namespace UE::Graphics::GpuApi
{
	struct TextureRef;
}


namespace UE::Graphics
{


	class JTextureResource : public JResource
	{
	public:

		using TRenderPtr = Ref< JTextureResource >;

	public:

		JTextureResource() { }

		virtual ~JTextureResource() { }


		virtual GpuApi::TextureRef	GetDescriptor() const = 0;
		
		virtual TRenderPtr			GetRenderResource() const = 0;

		virtual void				CreateRenderResource() = 0;

		virtual void				ReleaseRenderResource() = 0;

		virtual bool				IsRenderResourceCreated() = 0;

	};

}
