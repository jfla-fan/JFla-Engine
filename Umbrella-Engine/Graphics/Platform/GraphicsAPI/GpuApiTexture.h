#pragma once

#include "GpuTypes.h"



namespace UE::Graphics::GpuApi
{
	/************************************************************************/
	/*								TEXTURES                                */
	/************************************************************************/

	// #TODO ONLY SUPPORT FOR 2D TEXTURE FOR NOW. EXTEND THIS TO 1D, 3D TEXTURES AND ARRAYS OF TEXTURES, SLICES ETC ...


	TextureRef				CreateTexture( ETextureType type = ETextureType::TEXTURE_2D,
										   ETextureFormat format = ETextureFormat::AUTO,
										   ETextureAccessBits access = ETextureAccessBits::ALL,
										   const String& debugName = "" );

	TextureRef				CreateTexture( const STextureInitData& initData, ETextureFormat wishedFormat = ETextureFormat::AUTO );

	void					DestroyTexture( TextureRef ref );

	bool					IsResourceAllocated( TextureRef ref );


	uint32					GetTextureActiveUnit( TextureRef ref );	//< OpenGL specific :(

	STextureDesc			GetTextureDesc( TextureRef ref );

	void					BindTexture( TextureRef ref );	//< binds to current active texture unit.

	void					BindTexture( TextureRef ref, uint32 activeId ); //< binds to a given id and stores it as current texture unit.

	void					UnbindTexture( TextureRef ref );

	bool					IsTextureBound( TextureRef ref );

	bool					IsTextureStorageAllocated( TextureRef ref );

	bool					GenerateMipmaps( TextureRef ref );


	template< class _Range > requires ( FwdIterableSizedRange< _Range, TextureModeParam > )
	void					SetTextureWrappingModes( TextureRef ref, _Range&& params );

	void					SetTextureWrappingModes( TextureRef ref, const TextureModeParam* params, uint32 size );

	const JVector
	< TextureModeParam >&	GetTextureWrappingModes( TextureRef ref );


	void					SetTextureBorderColor( TextureRef ref, const Math::Vector4& borderColor );

	Math::Vector4			GetTextureBorderColor( TextureRef ref );


	void					SetTextureLodRange( TextureRef ref, const Math::Vector2& lodRange );

	Math::Vector2			GetTextureLodRange( TextureRef ref );


	void					SetTextureMinLod( TextureRef ref, float lod );

	float					GetTextureMinLod( TextureRef ref );


	void					SetTextureMaxLod( TextureRef ref, float lod );

	float					GetTextureMaxLod( TextureRef ref );


	void					SetTextureLodBias( TextureRef ref, float bias );

	float					GetTextureLodBias( TextureRef ref );

	void					SetTextureFilter( TextureRef ref, ETextureFilter filter );

	ETextureFilter			GetTextureFilter( TextureRef ref );

	void					SetTextureDebugName( TextureRef ref, const String& debugName );

	String					GetTextureDebugName( TextureRef ref );


	/**
	 * .
	 */
	void					AllocateTextureStorage( TextureRef ref,
													STextureSizeInfo textureSizeInfo,
													uint32 dataOffset = 0,
													CMemPtr data = NullPtr,
													ETextureFormat dataFormat = ETextureFormat::AUTO,
													ETextureFormat textureFormat = ETextureFormat::AUTO,
													ETextureAccessBits access = ETextureAccessBits::ALL );


	void					GetTextureData( TextureRef ref, STextureSubDataInfo info, MemPtr storage );

	void					UploadTextureData( TextureRef ref, STextureSubDataInfo info, CMemPtr data );


#if JF_DEBUG
	uint32					GetTextureNativeResourceDEBUG( TextureRef ref );
#endif



}
