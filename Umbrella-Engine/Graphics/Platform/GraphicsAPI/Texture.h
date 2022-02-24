#pragma once
#include "GpuTypes.h"
#include "../../../Math/Math.h"


namespace UE::Utils
{
	class Image;

	using ImagePtr = Ref< Image >;
}

namespace UE::Graphics
{
	USE_TEXTURE_TYPES()

	using Utils::Image;
	using Utils::ImagePtr;

	using Math::Vector4;


	class CTexture 
	{
	public:

		CTexture() : Descriptor() { }

		virtual ~CTexture() { }

		CTexture( const CTexture& another ) = delete;
		CTexture& operator = ( const CTexture& another ) = delete;

		TextureRef GetDescriptor() const { return Descriptor; }

		virtual void Release() = 0;

		virtual bool IsValid() const = 0;

		virtual void Bind() const = 0;
		virtual void Bind( uint32 id ) const = 0;
		virtual void Unbind() const = 0;

		
		virtual bool					IsLoaded() const = 0;
		virtual bool					IsAllocated() const = 0;
		virtual bool					IsMultisampled() const = 0;
		virtual bool					IsFloatingPoint() const = 0;
		virtual bool					IsDepthOnly() const = 0;


		virtual void					SetMinLOD( float lod ) = 0;
		virtual void					SetMaxLOD( float lod ) = 0;
		virtual void					SetBiasLOD( float bias ) = 0;
		virtual void					SetBorderColor( Vector4 color ) = 0;
		virtual void					SetWrappingMode( ETextureWrappingMode mode, ETextureWrappingAxis axis ) = 0;
		virtual void					SetFilter( ETextureFilter filter ) = 0;


		virtual float					GetMaxLOD() const = 0;
		virtual ImagePtr				GetImage() const = 0;
		virtual void					GenerateMipmapChain() const = 0;
		virtual Vector4					GetBorderColor() const = 0; 
		virtual uint32					GetPixelSize() const = 0;
		virtual ETextureWrappingMode	GetWrapMode( ETextureWrappingAxis axis = ETextureWrappingAxis::S ) const = 0;
		virtual STextureSizeInfo		GetSizeInfo() const = 0;
		virtual system::FilePath		GetFilePath() const = 0;
		virtual float					GetBiasLOD() const = 0;
		virtual ETextureFilter			GetFilter() const = 0;

	protected:

		TextureRef Descriptor;
	};

}


