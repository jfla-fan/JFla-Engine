#pragma once
#include "../Texture.h"
#include "OpenGLContext.h"



namespace UE::Utils
{
	class Image;

	using ImagePtr = Ref< Image >;
}

namespace UE::Graphics
{
	using namespace Math;

	using Utils::Image;
	using Utils::ImagePtr;

	USE_TEXTURE_TYPES()


	
	/**
	 * 2D Texture Class.
	 */
	class OpenGLTexture final : public CTexture
	{
	public:

		/**
		 * #todo IMPLEMENT MEMORY POOLS IN FUTURE TO USE THEM LIKE THIS: new (size_t, MemoryPool& )
		 * 
		 */

	private:
		
		mutable bool bLoaded;					// is data loaded on the GPU
		
		mutable bool bAllocated;				// is resource valid and meta data properly initialized
		
		mutable bool bHasMipChain;				// Does this texture have mip maps?
		
		STextureSizeInfo	SizeInfo;
		
		ETextureFormat		eFormat;

		ETextureType		eType;

		ETextureAccessBits	eAccess;

		system::FilePath	ResourcePath;

		String				DebugName;

		mutable uint32		Active = 0;	// Slot


	public:

		OpenGLTexture( ETextureType textureType, ETextureFormat textureFormat = ETextureFormat::AUTO, const String& debugName = "" );

		OpenGLTexture( const system::FilePath& path, ETextureType textureType, ETextureAccessBits access = ETextureAccessBits::ALL,
					   bool genMipChain = false, const String& debugName = "" );

		OpenGLTexture( CMemPtr data, ETextureFormat dataFormat, STextureSizeInfo info, ETextureType textureType, ETextureFormat textureFormat = ETextureFormat::AUTO,
					   ETextureAccessBits access = ETextureAccessBits::ALL, bool genMipChain = false, const String& debugName = "" );

		~OpenGLTexture();

		OpenGLTexture( const OpenGLTexture& another ) = delete;
		OpenGLTexture& operator = ( const OpenGLTexture& another ) = delete;

		OpenGLTexture( OpenGLTexture&& another ) NOEXCEPT;
		OpenGLTexture& operator = ( OpenGLTexture&& another ) NOEXCEPT;

		void Release() override;


		void Bind() const override;
		void Bind( uint32 id ) const override;
		void Unbind() const override;

		bool IsValid() const override { return !Descriptor.IsNull(); }


		bool Load( CMemPtr data, STextureSizeInfo info, ETextureFormat dataFormat = ETextureFormat::AUTO,
				   ETextureFormat textureFormat = ETextureFormat::AUTO, ETextureAccessBits access = ETextureAccessBits::ALL );
	
		bool Load( const system::FilePath& path, ETextureFormat format = ETextureFormat::AUTO, ETextureAccessBits access = ETextureAccessBits::ALL );

		bool Load( const Image& image, ETextureFormat format = ETextureFormat::AUTO, ETextureAccessBits access = ETextureAccessBits::ALL );


		void Upload( STextureSubDataInfo info, CMemPtr data );


		// #TODO in future TRenderPtr CreateResource, GetResource, ReleaseResource, GetImage [DONE], etc ...

		bool					IsMutable() const;
		bool					IsLoaded() const override;
		bool					IsAllocated() const override;
		bool					IsMultisampled() const override;
		bool					IsFloatingPoint() const override;
		bool					IsDepthOnly() const override;


		void					SetMinLOD( float lod ) override;
		void					SetMaxLOD( float lod ) override;
		void					SetBiasLOD( float bias ) override;
		void					SetBorderColor( Vector4 color ) override;
		void					SetWrappingMode( ETextureWrappingMode mode, ETextureWrappingAxis axices ) override;
		void					SetFilter( ETextureFilter filter ) override;


		float					GetMaxLOD() const override;
		ImagePtr				GetImage() const override;
		void					GenerateMipmapChain() const override;
		Vector4					GetBorderColor() const override;
		uint32					GetPixelSize() const;
		ETextureWrappingMode	GetWrapMode( ETextureWrappingAxis axis = ETextureWrappingAxis::S ) const override;
		STextureSizeInfo		GetSizeInfo() const override;
		system::FilePath		GetFilePath() const override;
		float					GetBiasLOD() const override;
		ETextureFilter			GetFilter() const override;


	};


}


