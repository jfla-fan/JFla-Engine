#include "OpenGLTexture.h"
#include "../../../../Image/ImageLoader.h"
#include "../GpuApi.h"



namespace UE::Graphics
{
	using namespace Utils;

	USE_TEXTURE_TYPES()


	static_assert( ( SIZE_T )ETextureFormat::AUTO - ( SIZE_T )ETextureFormat::R >= 5, "Add texture formats." );


	static ETextureFormat Map( ERawImageFormat format )
	{

#define CASE_LABEL( format )					\
		case ERawImageFormat::format:			\
			return ETextureFormat::format

#define CUSTOM_CASE_LABEL( imFormat, texFormat)	\
		case ERawImageFormat::imFormat:			\
			return ETextureFormat::texFormat;

		switch ( format )
		{
		case ERawImageFormat::L8:
		case ERawImageFormat::LA8:

			CASE_LABEL( R8 );
			CASE_LABEL( RG8 );
			CASE_LABEL( RGB8 );
			CASE_LABEL( RGBA8 );

			CUSTOM_CASE_LABEL( RH, R16F );
			CUSTOM_CASE_LABEL( RGBH, RGB16F );
			CUSTOM_CASE_LABEL( RGBAH, RGBA16F );
			
			CUSTOM_CASE_LABEL( RF, R32F );
			CUSTOM_CASE_LABEL( RGBF, RGB32F );
			CUSTOM_CASE_LABEL( RGBAF, RGBA32F );
			
			default:
				return ETextureFormat::AUTO;
		}

#undef CASE_LABEL
#undef CUSTOM_CASE_LABEL

	}

	static ERawImageFormat Map( ETextureFormat format )
	{
#define CASE_LABEL( format )					\
		case ETextureFormat::format:			\
			return ERawImageFormat::format

#define CUSTOM_CASE_LABEL( imFormat, texFormat)	\
		case ETextureFormat::imFormat:			\
			return ERawImageFormat::texFormat;

		switch( format )
		{
			CASE_LABEL( R8 );

			CUSTOM_CASE_LABEL( R, R8 );
			CUSTOM_CASE_LABEL( R16, RH );
			CUSTOM_CASE_LABEL( R16F, RH );
			CUSTOM_CASE_LABEL( R32F, RF);

			CASE_LABEL( RG8 );
			CUSTOM_CASE_LABEL( RG, RG8);

			CASE_LABEL( RGB8 );
			CUSTOM_CASE_LABEL( RGB, RGB8 );
			CUSTOM_CASE_LABEL( RGB16, RGBH );
			CUSTOM_CASE_LABEL( RGB16F, RGBH );
			CUSTOM_CASE_LABEL( RGB32F, RGBF );

			CASE_LABEL( RGBA8 );
			CUSTOM_CASE_LABEL( RGBA, RGBA8 );
			CUSTOM_CASE_LABEL( RGBA16F, RGBAH );
			CUSTOM_CASE_LABEL( RGBA32F, RGBAF );
			
			CUSTOM_CASE_LABEL( DEPTH, L8 );

			default:
				FATAL_ASSERT( false, "Failed to convert texture format to image format [not supported]." );

		}

#undef CASE_LABEL
#undef CUSTOM_CASE_LABEL

	}

	static ETextureWrappingMode Map( GLenum mode )
	{

#define CASE_LABEL(format_type)\
		case GL_##format_type:\
			return ETextureWrappingMode::format_type

		switch ( mode )
		{
			CASE_LABEL(CLAMP_TO_EDGE);
			CASE_LABEL(CLAMP_TO_BORDER);
			CASE_LABEL(MIRRORED_REPEAT);
			CASE_LABEL(REPEAT);
			CASE_LABEL(MIRROR_CLAMP_TO_EDGE);

		default:
			// #todo LOG_ERROR
			break;
		}

#undef CASE_LABEL

		return ETextureWrappingMode::REPEAT;
	}


	OpenGLTexture::OpenGLTexture( ETextureType textureType, ETextureFormat textureFormat, const String& debugName )
		: bLoaded( false )
		, bHasMipChain( false )
		, bAllocated( false )
		, eType( textureType )
		, eFormat( textureFormat )
		, eAccess( ETextureAccessBits::ALL )
		, DebugName( debugName )
		, ResourcePath( "" )
	{
		Descriptor = GpuApi::CreateTexture( textureType, textureFormat, ETextureAccessBits::ALL, debugName );

		JF_ASSERT( Descriptor, "Failed to create a texture." );
	}

	OpenGLTexture::OpenGLTexture( const system::FilePath& path, ETextureType textureType,
								  ETextureAccessBits access, bool genMipChain, const String& debugName )
		: OpenGLTexture( textureType, ETextureFormat::AUTO, debugName )
	{
		this->Load( path, ETextureFormat::AUTO, access );

		if ( genMipChain )
		{
			this->GenerateMipmapChain();
		}
	}

	OpenGLTexture::OpenGLTexture( CMemPtr data, ETextureFormat dataFormat, STextureSizeInfo info, ETextureType textureType,
								  ETextureFormat textureFormat, ETextureAccessBits access, bool genMipChain, const String& debugName )
		: OpenGLTexture( textureType, textureFormat, debugName )
	{
		this->Load( data, info, dataFormat, textureFormat, access );

		if ( genMipChain )
		{
			this->GenerateMipmapChain();
		}
	}

	OpenGLTexture::~OpenGLTexture()
	{
		this->Release();
	}

	void OpenGLTexture::Release()
	{
		GpuApi::SafeRelease( Descriptor );
	}

	void OpenGLTexture::Bind() const
	{
		GpuApi::BindTexture( Descriptor );
	}

	void OpenGLTexture::Bind( uint32 id ) const
	{
		GpuApi::BindTexture( Descriptor, id );
		this->Active = id;
	}

	void OpenGLTexture::Unbind() const
	{
		GpuApi::UnbindTexture( Descriptor );
	}

	void OpenGLTexture::GenerateMipmapChain() const
	{
		if (IsValid())
		{
			if ( GpuApi::GenerateMipmaps( Descriptor ) )
			{
				bHasMipChain = true;
			}
			else
			{
				// #todo HALT
			}

		}
	}

	bool OpenGLTexture::Load( CMemPtr data, STextureSizeInfo info, ETextureFormat dataFormat,
							  ETextureFormat textureFormat, ETextureAccessBits access )
	{
		GpuApi::AllocateTextureStorage( Descriptor, info, 0, data, dataFormat, textureFormat, access );

		SizeInfo = info;
		eFormat = ( textureFormat == ETextureFormat::AUTO ) ? dataFormat : textureFormat;
		eAccess = access;

		bAllocated = true;
		bLoaded = true;

		return true;
	}

	bool OpenGLTexture::Load( const system::FilePath& path, ETextureFormat format, ETextureAccessBits access )
	{
		auto img = ImageLoader::Load( path, true );
	
		if ( !img || !img->IsInitialized() )
		{
			// #todo log it
			return false;
		}

		STextureSizeInfo dataInfo =
		{
			img->GetWidth(),
			img->GetHeight(),
			0,
			img->GetChannelsCount(),
			img->GetBytesPerChannel()
		};


		return Load( img->RawData(), dataInfo, Map( img->GetFormat() ), format, access );
	}


	bool OpenGLTexture::Load( const Image& image, ETextureFormat format, ETextureAccessBits access )
	{
		if ( !image.IsInitialized() )
		{
			// #todo WARN
			return false;
		}

		STextureSizeInfo info =
		{
			image.GetWidth(),
			image.GetHeight(),
			0,
			image.GetChannelsCount(),
			image.GetBytesPerChannel()
		};

		return this->Load( image.RawData(), info, Map( image.GetFormat() ), format, access );
	}

	void OpenGLTexture::Upload( STextureSubDataInfo info, CMemPtr data )
	{
		GpuApi::UploadTextureData( Descriptor, info, data );
	}

	void OpenGLTexture::SetMinLOD( float lod )
	{
		GpuApi::SetTextureMinLod( Descriptor, lod );
	}

	void OpenGLTexture::SetMaxLOD(float lod)
	{
		GpuApi::SetTextureMaxLod( Descriptor, lod );
	}

	void OpenGLTexture::SetBorderColor(Vector4 color)
	{
		GpuApi::SetTextureBorderColor( Descriptor, color );
	}

	void OpenGLTexture::SetWrappingMode(ETextureWrappingMode mode, ETextureWrappingAxis axis)
	{
		GpuApi::SetTextureWrappingModes( Descriptor, new TextureModeParam( axis, mode ), 1 );
	}

	void OpenGLTexture::SetFilter( ETextureFilter filter )
	{
		GpuApi::SetTextureFilter( Descriptor, filter );
	}

	float OpenGLTexture::GetMaxLOD() const
	{
		return Math::Log2(Math::Max(SizeInfo.Width, SizeInfo.Height));
	}

	void OpenGLTexture::SetBiasLOD(float bias)
	{
		GpuApi::SetTextureLodBias( Descriptor, bias );
	}

	ImagePtr OpenGLTexture::GetImage() const
	{
		if (!this->bAllocated || !this->bLoaded)
		{
			return NullPtr;
		}

		auto img = Image( this->SizeInfo.Width, this->SizeInfo.Height, Map( eFormat ) );

		if ( !img.IsInitialized() )
		{
			return NullPtr;
		}

		STextureSubDataInfo dataInfo
		{
			img.GetWidth(),
			img.GetHeight(),
			0,
			0,
			0,
			0,
			img.GetBytesSize(),
			eFormat
		};

		GpuApi::GetTextureData( Descriptor, dataInfo, img.RawData() );

		img.MarkInitialized();

		ImagePtr ptr = MakeScoped< Image >( std::move( img ) );

		return ptr;
	}

	bool OpenGLTexture::IsLoaded() const
	{
		return bLoaded;
	}

	bool OpenGLTexture::IsAllocated() const
	{
		return bAllocated;
	}

	bool OpenGLTexture::IsMultisampled() const
	{
		return false;
	}

	bool OpenGLTexture::IsFloatingPoint() const
	{
		switch (eFormat)
		{
			case ETextureFormat::R16F:
			case ETextureFormat::R32F:
			case ETextureFormat::RG16F:
			case ETextureFormat::RGB16F:
			case ETextureFormat::RGBA16F:
				return true;

			default:
				return false;
		}
	}

	bool OpenGLTexture::IsDepthOnly() const
	{
		return eFormat == ETextureFormat::DEPTH;
	}

	Vector4	OpenGLTexture::GetBorderColor() const
	{
		return GpuApi::GetTextureBorderColor( Descriptor );
	}

	uint32 OpenGLTexture::GetPixelSize() const
	{
		return GpuApi::GetPixelSize( eFormat );
	}

	ETextureWrappingMode OpenGLTexture::GetWrapMode(ETextureWrappingAxis axis) const
	{
		for ( const auto& [ texAxis, texMode ] : GpuApi::GetTextureWrappingModes( Descriptor ) )
		{
			if ( texAxis == axis )
			{
				return texMode;
			}
		}
	
		return ETextureWrappingMode::REPEAT;	// default open GL mode
	}

	STextureSizeInfo OpenGLTexture::GetSizeInfo() const
	{
		return SizeInfo;
	}

	system::FilePath OpenGLTexture::GetFilePath() const
	{
		return ResourcePath;
	}

	float OpenGLTexture::GetBiasLOD() const
	{
		return GpuApi::GetTextureLodBias( Descriptor );
	}

	ETextureFilter OpenGLTexture::GetFilter() const
	{
		return GpuApi::GetTextureFilter( Descriptor );
	}
}