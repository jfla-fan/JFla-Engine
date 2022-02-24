#include "GpuDeviceData.h"
#include "../../GpuApiTexture.h"




namespace UE::Graphics::GpuApi
{

	static GLenum Map( ETextureFormat format )
	{

#define CASE_LABEL( format_type )\
case ETextureFormat::format_type:\
		return GL_##format_type

#define CUSTOM_CASE_LABEL( format_type, gl_type )\
		case ETextureFormat::format_type:\
			return gl_type

		switch ( format )
		{
			CUSTOM_CASE_LABEL( R, GL_RED );

			CASE_LABEL( R8 );
			CASE_LABEL( R16 );
			CASE_LABEL( R16F );
			CASE_LABEL( R32F );

			CASE_LABEL( RG );
			CASE_LABEL( RG8 );
			CASE_LABEL( RG16 );
			CASE_LABEL( RG16F );
			CASE_LABEL( RG32F );

			CASE_LABEL( RGB );
			CASE_LABEL( RGB8 );
			CASE_LABEL( RGB16 );
			CASE_LABEL( RGB16F );
			CASE_LABEL( RGB32F );

			CASE_LABEL( RGBA );
			CASE_LABEL( RGBA8 );
			CASE_LABEL( RGBA16F );
			CASE_LABEL( RGBA32F );

			CUSTOM_CASE_LABEL( AUTO, GL_RGB );

			CUSTOM_CASE_LABEL( DEPTH, GL_DEPTH_COMPONENT );

		default:
			// #todo WARN!
			return GL_RGB;
		
		}

#undef CASE_LABEL
#undef CUSTOM_CASE_LABEL

	}

	static GLenum Map( ETextureType type )
	{
		switch( type )
		{
			case ETextureType::TEXTURE_1D:
				return GL_TEXTURE_1D;
			case ETextureType::TEXTURE_2D:
				return GL_TEXTURE_2D;
			case ETextureType::TEXTURE_3D:
				return GL_TEXTURE_3D;
			case ETextureType::TEXTURE_CUBE:
				return GL_TEXTURE_CUBE_MAP;
			default:
				FATAL_ASSERT( false, "Unsupported texture type." );
		}
	}

	static GLenum Map( EPixelType type )
	{
		static HashMap< EPixelType, GLenum > PixelToGLTypeTable =
		{
			{ EPixelType::BYTE, GL_BYTE },
			{ EPixelType::UBYTE, GL_UNSIGNED_BYTE },
			{ EPixelType::SHORT, GL_SHORT },
			{ EPixelType::UBYTE, GL_UNSIGNED_SHORT },
			{ EPixelType::INT, GL_INT },
			{ EPixelType::UINT, GL_UNSIGNED_INT },
			{ EPixelType::FLOAT, GL_FLOAT },
			{ EPixelType::HFLOAT, GL_HALF_FLOAT },
		};

		if ( !PixelToGLTypeTable.contains( type ) )
		{
			// #todo HALT
			return GL_UNSIGNED_BYTE;
		}

		return PixelToGLTypeTable[type];
	}

	static GLenum Map( ETextureWrappingAxis axis )
	{
		switch( axis )
		{
			case ETextureWrappingAxis::S:
				return GL_TEXTURE_WRAP_S;
			case ETextureWrappingAxis::T:
				return GL_TEXTURE_WRAP_T;
			case ETextureWrappingAxis::R:
				return GL_TEXTURE_WRAP_R;
			default:
				// #todo HALT
				return 0;
		}
	}

	static std::pair< GLenum, GLenum > Map( ETextureFilter filter )
	{
		switch( filter )
		{
			case ETextureFilter::MIG_MAG_MIP_POINT:
				return { GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST };
			case ETextureFilter::MIN_MAG_POINT_MIP_LINEAR:
				return { GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST };
			case ETextureFilter::MIN_POINT_MAG_LINEAR_MIP_POINT:
				return { GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR };
			case ETextureFilter::MIN_POINT_MAG_MIP_LINEAR:
				return { GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR };
			case ETextureFilter::MIN_LINEAR_MAG_MIP_POINT:
				return { GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST };
			case ETextureFilter::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
				return { GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST };
			case ETextureFilter::MIN_MAG_LINEAR_MIP_POINT:
				return { GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR };
			case ETextureFilter::MIN_MAG_MIP_LINEAR:
				return { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR };
			default:
				FATAL_ASSERT( false, "Unknown texture filter." );
		}
	}

	static GLenum Map( ETextureWrappingMode mode )
	{
#define GLTABLE_ENTRY( mode )\
		{ ETextureWrappingMode::mode, GL_##mode }

		static HashMap<ETextureWrappingMode, GLenum> TextureWrappingModeTable =
		{
			GLTABLE_ENTRY( CLAMP_TO_EDGE ),
			GLTABLE_ENTRY( CLAMP_TO_BORDER),
			GLTABLE_ENTRY( MIRRORED_REPEAT ),
			GLTABLE_ENTRY( REPEAT ),
			GLTABLE_ENTRY( MIRROR_CLAMP_TO_EDGE )
		};

#undef GLTABLE_ENTRY

		return TextureWrappingModeTable[mode];
	}


	/************************************************************************/
	/*						TEXTURES GPU API                                */
	/************************************************************************/

	TextureRef AllocateTexture()
	{
		auto& deviceData = GetDeviceData();

		TextureRef ref = deviceData.TextureResources.Create();

		if ( ref.IsNull() )
		{
			return ref;
		}

		auto& texData = deviceData.TextureResources.Data( ref );

		OpenGLContext::GenTextures( 1, &texData.Resource );
	
		if ( texData.Resource == OpenGLContext::GInvalidGLResource )
		{
			// #todo HALT
			deviceData.TextureResources.Destroy( ref );
			return TextureRef::Null();
		}

		return ref;
	}


	TextureRef CreateTexture( ETextureType type,
							  ETextureFormat format,
							  ETextureAccessBits access,
							  const String& debugName )
	{
		TextureRef ref = AllocateTexture();

		if ( ref.IsNull() )
		{
			// #todo HALT
			return ref;
		}

		auto& deviceData = GetDeviceData();

		auto& texData = deviceData.TextureResources.Data( ref );

		texData.Description.eFormat = format;
		texData.Description.eAccess = access;
		texData.Description.eType = type;
		texData.Description.eFilter = ETextureFilter::MIN_POINT_MAG_MIP_LINEAR;
		texData.Description.DebugName = debugName;
		texData.Description.LodRange = { -1000.f, 1000.f };
		texData.Description.LodBias = 0.f;

		OpenGLContext::BindTexture( Map( type ), texData.Resource );
		
		if ( !debugName.empty() )
		{
			OpenGLContext::ObjectLabel( GL_TEXTURE, texData.Resource, debugName.length(), debugName.c_str() );
		}


		return ref;
	}

	TextureRef CreateTexture( const STextureInitData& initData, ETextureFormat wishedFormat )
	{
		auto& deviceData = GetDeviceData();

		TextureRef ref = CreateTexture( initData.eType, initData.eFormat, initData.eAccess, initData.DebugName );

		if ( ref.IsNull() )
		{
			return ref;
		}

		auto& texData = deviceData.TextureResources.Data( ref );

		texData.Description.bHasMipchain = initData.bHasMipchain;
		texData.Description.bIsGamma = initData.bIsGamma;
		texData.Description.eTextureChannels = GetChannels( initData.eFormat );
	
		texData.Description.SizeInfo = initData.SizeInfo;

		// #TODO MAKE SUPPORT FOR DEPTH-ONLY TEXTURES!

		// if user intends to allocate data
		if ( initData.SizeInfo.Width > 0 && initData.SizeInfo.Height > 0 )
		{

			JF_ASSERT( GetChannelsCount( texData.Description.eTextureChannels ) == initData.SizeInfo.ChannelsCount, "Unmatched channels count" );
			JF_ASSERT( GetChannelSize( texData.Description.eFormat ) == initData.SizeInfo.ChannelSizeInBytes, "Unmatched pixel size." );
			JF_ASSERT( !( initData.eFormat == ETextureFormat::AUTO && wishedFormat == ETextureFormat::AUTO ), "Texture format is not specified." );

			ETextureFormat internalFormat = ( wishedFormat == ETextureFormat::AUTO ) ? initData.eFormat : wishedFormat;
			ETextureFormat dataFormat = ( initData.eFormat == ETextureFormat::AUTO ) ? internalFormat : initData.eFormat; 
			EPixelType pixelType = GetPixelType( dataFormat );

			// if user intends to upload texture data size must be at least equal to image size.
			JF_ASSERT( !initData.Data || ( initData.DataSize >= initData.SizeInfo.Width * initData.SizeInfo.Height * GetPixelSize( dataFormat ) ),
					   "Data size is less that required." );

			// first allocation must not be blocked because of the flags
			texData.Description.eAccess |= ETextureAccessBits::REALLOCATE;

			AllocateTextureStorage( ref, initData.SizeInfo, initData.DataOffset, initData.Data,
									dataFormat, internalFormat, initData.eAccess );

			// should we generate mipmaps?
			if ( initData.bHasMipchain && initData.Data )
			{
				GenerateMipmaps( ref );
			}
			else
			{
				texData.Description.bHasMipchain = false;
				texData.Description.MipmapsCount = 0;
			}

		}
		else
		{
			// #TODO HALT - failed to allocate data

			texData.Description.bAllocated = false;
			
			if ( initData.bHasMipchain )
			{
				// #todo HALT - cannot generate mip chain if data is not loaded.
				texData.Description.bHasMipchain = false;
				texData.Description.MipmapsCount = 0;
			}
		}

		return ref;
	}

	void DestroyTexture( TextureRef ref )
	{
		int32 newCount = SafeRelease( ref );
		JF_ASSERT( newCount == 0, "Failed to destroy texture. It is used somewhere else." );
	}

	bool IsResourceAllocated( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();

		return deviceData.TextureResources.IsInUse( ref );
	}


	uint32 GetTextureActiveUnit( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		return texData.ActiveId;
	}

	STextureDesc GetTextureDesc( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		return deviceData.TextureResources.Data( ref ).Description;
	}

	void BindTexture( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		OpenGLContext::ActiveTexture( GL_TEXTURE0 + texData.ActiveId );
		OpenGLContext::BindTexture( Map( texData.Description.eType ), texData.Resource );

		texData.Description.bIsBound = true;
	}

	void BindTexture( TextureRef ref, uint32 activeId ) //< binds to a given id and stores it as current texture unit.
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		JF_ASSERT( activeId < OpenGLContext::GetMaxCombinedTextureUnits(), "active texture is too big" );

		texData.ActiveId = activeId;

		OpenGLContext::ActiveTexture( GL_TEXTURE0 + texData.ActiveId );
		OpenGLContext::BindTexture( Map( texData.Description.eType ), texData.Resource );

		texData.Description.bIsBound = true;
	}

	void UnbindTexture( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		OpenGLContext::UnbindTexture( Map( texData.Description.eType ) );

		texData.Description.bIsBound = false;
	}

	bool IsTextureBound( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		return texData.Description.bIsBound;
	}

	bool IsTextureStorageAllocated( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		return texData.Description.bAllocated;
	}

	bool GenerateMipmaps( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		if ( !texData.Description.bAllocated )
		{
			// #TODO HALT NOT ALLOCATED
			return false;
		}

		OpenGLContext::BindTexture( Map( texData.Description.eType ), texData.Resource );
		OpenGLContext::GenerateMipmap( Map( texData.Description.eType ) );
		OpenGLContext::UnbindTexture( Map( texData.Description.eType ) );

		texData.Description.bHasMipchain = true;
		texData.Description.MipmapsCount = CalcMipmapsCount( texData.Description.SizeInfo.Width, texData.Description.SizeInfo.Height );

		return true;
	}


	template< class _Range > requires (FwdIterableSizedRange< _Range, TextureModeParam >)
	void SetTextureWrappingModes( TextureRef ref, _Range&& params )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		for ( const auto& [ axis, mode ] : params )
		{
			OpenGLContext::TextureParameteri( texData.Resource, Map( axis ), Map( mode ) );
		}
	}

	void SetTextureWrappingModes( TextureRef ref, const TextureModeParam* params, uint32 size )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		texData.Description.Modes = JVector< TextureModeParam >( params, params + size );

		for ( const auto& [ axis, mode ] : texData.Description.Modes )
		{
			OpenGLContext::TextureParameteri( texData.Resource, Map( axis ), Map( mode ) );
		}
	}

	const JVector< TextureModeParam >& GetTextureWrappingModes( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );
		
		return texData.Description.Modes;
	}


	void SetTextureBorderColor( TextureRef ref, const Math::Vector4& borderColor )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		auto normalized = Math::Clamp( borderColor, Math::MakeVector4( 0.f ), Math::MakeVector4( 1.f ) );

		OpenGLContext::TextureParameterfv( texData.Resource, GL_TEXTURE_BORDER_COLOR, &normalized[0] );

		texData.Description.BorderColor = borderColor;
	}

	Math::Vector4 GetTextureBorderColor( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		return texData.Description.BorderColor;
	}


	void SetTextureLodRange( TextureRef ref, const Math::Vector2& lodRange )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		OpenGLContext::TextureParameterf( texData.Resource, GL_TEXTURE_MIN_LOD, lodRange.x );
		OpenGLContext::TextureParameterf( texData.Resource, GL_TEXTURE_MAX_LOD, lodRange.y );

		texData.Description.LodRange = lodRange;
	}

	Math::Vector2 GetTextureLodRange( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );
		
		return texData.Description.LodRange;
	}


	void SetTextureMinLod( TextureRef ref, float lod )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		OpenGLContext::TextureParameterf( texData.Resource, GL_TEXTURE_MIN_LOD, lod );

		texData.Description.LodRange.x = lod;;
	}

	float GetTextureMinLod( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );
		
		return texData.Description.LodRange.x;
	}


	void SetTextureMaxLod( TextureRef ref, float lod )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		OpenGLContext::TextureParameterf( texData.Resource, GL_TEXTURE_MAX_LOD, lod );

		texData.Description.LodRange.y = lod;
	}

	float GetTextureMaxLod( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		return texData.Description.LodRange.y;
	}


	void SetTextureLodBias( TextureRef ref, float bias )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		OpenGLContext::TextureParameterf( texData.Resource, GL_TEXTURE_LOD_BIAS, bias );

		texData.Description.LodBias = bias;
	}

	float GetTextureLodBias( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		return texData.Description.LodBias;
	}


	void SetTextureFilter( TextureRef ref, ETextureFilter filter )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );
		
		const auto [ minFilter, magFilter ] = Map( filter );

		OpenGLContext::TextureParameteri( texData.Resource, GL_TEXTURE_MIN_FILTER, minFilter );

		OpenGLContext::TextureParameteri( texData.Resource, GL_TEXTURE_MAG_FILTER, magFilter );
	}

	ETextureFilter GetTextureFilter( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );
		
		return texData.Description.eFilter;
	}

	void SetTextureDebugName( TextureRef ref, const String& debugName )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		OpenGLContext::ObjectLabel( GL_TEXTURE, texData.Resource, debugName.length(), debugName.c_str() );
		texData.Description.DebugName = debugName;
	}

	String	GetTextureDebugName( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );
		
		return texData.Description.DebugName;
	}


	void AllocateTextureStorage( TextureRef ref,
								 STextureSizeInfo textureSize,
								 uint32 dataOffset,
								 CMemPtr data,
								 ETextureFormat dataFormat,
								 ETextureFormat textureFormat,
								 ETextureAccessBits access )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );

		JF_ASSERT( texData.Description.eAccess & ETextureAccessBits::REALLOCATE, "Not enough permissions. Texture storage is immutable." );
		JF_ASSERT( dataFormat != ETextureFormat::AUTO, "Data format must be specified explicitly." );
		JF_ASSERT( textureSize.Width > 0 && textureSize.Height > 0, "Invalid texture size info." );


		// if texture format is not explicitly specified and was not specified earlier, data format will be used as texture format
		ETextureFormat texFormat = ( textureFormat == ETextureFormat::AUTO ) ?
								   ( ( texData.Description.eFormat == ETextureFormat::AUTO) ?
									   dataFormat : texData.Description.eFormat ) : textureFormat;

		ETextureFormat pixelFormat = GetPixelFormat( dataFormat ) ;
		EPixelType pixelType = GetPixelType( pixelFormat );
		CMemPtr	dataPtr = ( data ) ? ( byte* )data + dataOffset : NullPtr;

		BindTexture( ref );
		
		// #TODO MAKE DEPTH-ONLY TEXTURES SUPPORT!

		if ( Any( access & ETextureAccessBits::REALLOCATE ) )
		{
			// allocating mutable storage

			OpenGLContext::TexImage2D( Map( texData.Description.eType ), 0, Map( texFormat ),
									   textureSize.Width, textureSize.Height, 0,
									   Map( pixelFormat ), Map( pixelType ), dataPtr );
		}
		else
		{
			// allocating immutable storage

			OpenGLContext::TextureStorage2D( texData.Resource, 1, Map( texFormat ), textureSize.Width, textureSize.Height );
			
			if ( dataPtr )
			{
				OpenGLContext::TextureSubImage2D( texData.Resource, 0, 0, 0, textureSize.Width,
												  textureSize.Height, Map( pixelFormat ), Map( pixelType ), dataPtr );
			}
		}

		UnbindTexture( ref );

		texData.Description.SizeInfo = textureSize;
		texData.Description.eFormat = texFormat;
		texData.Description.eAccess = access;
		texData.Description.bAllocated = true;
		texData.Description.eTextureChannels = GetChannels( texFormat );
	
	}

	void GetTextureData( TextureRef ref, STextureSubDataInfo info, MemPtr storage )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" ); 

		auto& texData = deviceData.TextureResources.Data( ref );

		JF_ASSERT( texData.Description.eAccess & ETextureAccessBits::READ, "Texture is not readable." );
		JF_ASSERT( !( texData.Description.eFormat == ETextureFormat::AUTO && info.DataFormat == ETextureFormat::AUTO ), "Cannot specify texture format" );
		JF_ASSERT( info.Width > 0 && info.Height > 0 && info.DataSize > 0, "Invalid texture info parameters." );
		
		ETextureFormat format = ( texData.Description.eFormat == ETextureFormat::AUTO ) ? info.DataFormat : texData.Description.eFormat;
		EPixelType pixel = GetPixelType( format );

		OpenGLContext::GetTextureSubImage( texData.Resource, 0, info.WidthOffset, info.HeightOffset,
										   info.DepthOffset, info.Width, info.Height, info.Depth,
										   Map( GetPixelFormat( format ) ), Map( pixel ), info.DataSize, storage );

	}

	void UploadTextureData( TextureRef ref, STextureSubDataInfo info, CMemPtr data )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		if ( !data )
		{
			// #TODO HALT
			return;
		}

		auto& texData = deviceData.TextureResources.Data( ref );

		JF_ASSERT( texData.Description.eAccess & ETextureAccessBits::WRITE, "Texture is not writable." );
		JF_ASSERT( !( texData.Description.eFormat == ETextureFormat::AUTO && info.DataFormat == ETextureFormat::AUTO ), "Unable specify texture format" );


		ETextureFormat dataFormat = ( info.DataFormat == ETextureFormat::AUTO ) ? texData.Description.eFormat : info.DataFormat;
		EPixelType pixelType = GetPixelType( dataFormat );

		OpenGLContext::TextureSubImage2D( texData.Resource, 0, info.WidthOffset, info.HeightOffset,
										  info.Width, info.Height, Map( dataFormat ), Map( pixelType ), data );
	
	}


	uint32 GetTextureNativeResourceDEBUG( TextureRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( ref, "Texture does not exist" );

		auto& texData = deviceData.TextureResources.Data( ref );
		
		return texData.Resource;
	}

}
