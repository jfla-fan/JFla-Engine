#pragma once

#include "GpuApiLimits.h"

#include "../../../Utils/Time/TimeSpan.h"
#include "../../../Memory/Memory.h"
#include "../../../Math/Math.h"
#include "../../../Misc/ResourceView.h"

#include <list>
#include <ranges>



#define USE_GPU_TYPE( type_name )													\
	using UE::Graphics::GpuApi::type_name;

#define USE_GPU_SAME_TYPE( type_name )												\
	using type_name = UE::Graphics::GpuApi::type_name;

#define USE_GPU_SPECIALIZED_TYPE( name, type )										\
	using name = UE::Graphics::GpuApi::type;



#define USE_TEXTURE_TYPES()															\
	USE_GPU_TYPE( TextureRef )														\
	USE_GPU_TYPE( ETextureWrappingMode )											\
	USE_GPU_TYPE( ETextureWrappingAxis )											\
	USE_GPU_TYPE( ETextureFilter )													\
	USE_GPU_TYPE( ETextureChannel )													\
	USE_GPU_TYPE( ETextureFormat )													\
	USE_GPU_TYPE( EPixelType )														\
	USE_GPU_TYPE( ETextureType )													\
	USE_GPU_TYPE( ETextureAccessBits )												\
	USE_GPU_TYPE( TextureModeParam )												\
	USE_GPU_TYPE( STextureSubDataInfo )												\
	USE_GPU_TYPE( STextureSizeInfo )												\
	USE_GPU_TYPE( STextureDesc )													\
	USE_GPU_TYPE( STextureInitData )


#define USE_BUFFER_TYPES()															\
	USE_GPU_TYPE( BufferRef )														\
	USE_GPU_TYPE( EBufferUsage )													\
	USE_GPU_TYPE( EBufferType )														\
	USE_GPU_TYPE( EBufferAccessBits )												\
	USE_GPU_TYPE( SBufferDesc )														\
	USE_GPU_TYPE( SBufferInitData )


#define USE_SHADER_TYPES()															\
	USE_GPU_TYPE( ShaderRef )														\
	USE_GPU_TYPE( EShaderType )														\
	USE_GPU_TYPE( SPipelineStageInfo )


#define USE_INPUT_LAYOUT_TYPES()													\
	USE_GPU_TYPE( InputLayoutRef )													\
	USE_GPU_TYPE( EInputElementFormat )												\
	USE_GPU_TYPE( SInputElement	)													\
	USE_GPU_TYPE( SInputLayoutDesc )												\
	USE_GPU_SPECIALIZED_TYPE( LayoutSpan, SInputLayoutDesc::LayoutSpan )			\
	USE_GPU_SPECIALIZED_TYPE( LayoutContainer, SInputLayoutDesc::LayoutContainter )	\
	USE_GPU_SPECIALIZED_TYPE( LayoutUnit, SInputLayoutDesc::LayoutUnit )			\
	USE_GPU_SPECIALIZED_TYPE( SlotUnit, SInputLayoutDesc::SlotUnit )
		

#define USE_RENDER_STATE_TYPES()													\
	USE_GPU_TYPE( RenderStateRef )													\
	USE_GPU_TYPE( SRenderStateDesc )												\
	USE_GPU_TYPE( SRenderSetup )													\
	USE_GPU_SPECIALIZED_TYPE( ERenderState, SRenderStateDesc::ERenderState )		\
	USE_GPU_SPECIALIZED_TYPE( BufferUnit, SRenderStateDesc::BufferUnit )			\
	USE_GPU_SPECIALIZED_TYPE( RenderTargetUnit, SRenderStateDesc::RenderTargetUnit)	\
	USE_GPU_SPECIALIZED_TYPE( URenderParams, SRenderStateDesc::URenderParams )		\
	USE_GPU_SPECIALIZED_TYPE( URenderState, SRenderStateDesc::URenderState )





namespace UE::Graphics::GpuApi
{
	using namespace Math;

	struct TextureRef;
	struct ShaderRef;
	struct BufferRef;
	struct SamplerRef;
	struct InputLayoutRef;
	struct RenderStateRef;


#define RES_TYPE TextureRef
#include "gpuResourceRef.inl"

#define RES_TYPE ShaderRef				// #todo IMPLEMENT THIS
#include "gpuResourceRef.inl"

#define RES_TYPE BufferRef
#include "gpuResourceRef.inl"

#define RES_TYPE SamplerRef				// #todo IMPLEMENT THIS
#include "gpuResourceRef.inl"

#define RES_TYPE InputLayoutRef
#include "gpuResourceRef.inl"

#define RES_TYPE RenderStateRef			// #todo IMPLEMENT THIS
#include "gpuResourceRef.inl"



	/************************************************************************/
	/*						DEVICE SPECIFIC STUFF							*/
	/************************************************************************/

	enum class EDeviceType : uint8
	{
		UNKNOWN,
		PC,					// the only one supported for now
		XBOX,
		PLAY_STATION_PRO,
		PLAY_STATION_5,
	};

	enum class EGPUVendor : uint8
	{
		UNKNOWN,
		NVIDIA,
		AMD,
		INTEL
	};


	struct SDeviceData;			// see GpuDeviceData.h

	struct SDeviceCapabilities; // see GpuDeviceData.h


	/************************************************************************/
	/*						INPUT LAYOUT SPECIFIC STUFF                     */
	/************************************************************************/

	// Direct3D formats
	enum class EInputElementFormat : uint8
	{
		UNKNOWN,
		R32G32B32A32_TYPELESS,
		R32G32B32A32_FLOAT,
		R32G32B32A32_UINT,
		R32G32B32A32_SINT,
		R32G32B32_TYPELESS,
		R32G32B32_FLOAT,
		R32G32B32_UINT,
		R32G32B32_SINT,
		R16G16B16A16_TYPELESS,
		R16G16B16A16_FLOAT,
		R16G16B16A16_UNORM,
		R16G16B16A16_UINT,
		R16G16B16A16_SNORM,
		R16G16B16A16_SINT,
		R32G32_TYPELESS,
		R32G32_FLOAT,
		R32G32_UINT,
		R32G32_SINT,
		R8G8B8A8_TYPELESS,
		R8G8B8A8_UNORM,
		R8G8B8A8_UNORM_SRGB,
		R8G8B8A8_UINT,
		R8G8B8A8_SNORM,
		R8G8B8A8_SINT,
		R16G16_TYPELESS,
		R16G16_FLOAT,
		R16G16_UNORM,
		R16G16_UINT,
		R16G16_SNORM,
		R16G16_SINT,
		R32_TYPELESS,
		D32_FLOAT,
		R32_FLOAT,
		R32_UINT,
		R32_SINT,
		R8G8_TYPELESS,
		R8G8_UNORM,
		R8G8_UINT,
		R8G8_SNORM,
		R8G8_SINT,
		R16_TYPELESS,
		R16_FLOAT,
		D16_UNORM,
		R16_UNORM,
		R16_UINT,
		R16_SNORM,
		R16_SINT,
		R8_TYPELESS,
		R8_UNORM,
		R8_UINT,
		R8_SNORM,
		R8_SINT,
	};


	struct SInputElement
	{
		EInputElementFormat Format;

		uint8				SizeInBytes;	//< Size of all the elements

		uint8				ElementsCount;

		uint8				EntriesCount;

		constexpr SInputElement()
			: Format( EInputElementFormat::UNKNOWN )

			, SizeInBytes( 0 )
			, ElementsCount( 0 )
			, EntriesCount( 0 )
		{
		}

		constexpr SInputElement( EInputElementFormat format,
								 uint8 sizeInBytes,
								 uint8 elementsCount, 
								 uint8 entriesCount )
			: Format( format )
			, SizeInBytes( sizeInBytes )
			, ElementsCount( elementsCount )
			, EntriesCount( entriesCount )
		{
		}


		static constexpr SInputElement Empty() { return SInputElement(); }

		constexpr bool operator == (const SInputElement& another) { return Format == another.Format &&
																	SizeInBytes == another.SizeInBytes &&
																	ElementsCount == another.ElementsCount &&
																	EntriesCount == another.EntriesCount; }

		constexpr bool IsNull() const { return *this == Empty(); }

		constexpr FORCEINLINE explicit operator bool() { return IsNull(); }

		template< EInputElementFormat _Format >
		constexpr static SInputElement Acquire();

	};


	struct SInputLayoutDesc
	{

		enum 
		{
			ElementsCount = Limits::MAX_INPUT_LAYOUT_ELEMENTS,
			SlotsCount	  = Limits::MAX_INPUT_LAYOUT_INPUT_SLOTS
		};

		struct LayoutUnit
		{
			SInputElement	Element;

			uint8			InputSlot;

			LayoutUnit()
				: Element()
				, InputSlot( 0 )
			{
			}

			LayoutUnit( SInputElement element,
						uint8 inputSlot )
				: Element( element )
				, InputSlot( inputSlot )
			{
			}
		};

		struct SlotUnit
		{
			uint32 Offset;

			uint32 Stride;


			SlotUnit()
				: Offset( 0 )
				, Stride( 0 )
			{
			}

			SlotUnit( uint32 offset,
					  uint32 stride )
				: Offset( offset )
				, Stride( stride )
			{
			}
		};

		using LayoutSpan = JSpan< LayoutUnit, ElementsCount >;
		using LayoutContainter = Array< LayoutUnit, ElementsCount >;
		using SlotContainer = Array< SlotUnit, SlotsCount >;


		static constexpr auto ZeroElementCallback = []( LayoutUnit& unit ) { unit.Element = SInputElement::Empty(); unit.InputSlot = 0; };
		
		static constexpr auto ZeroSlotCallback = []( SlotUnit& unit ) { unit.Offset = 0; unit.Stride = 0; };


		uint32				BaseOffset;		// #TODO USELESS THING FOR NOW

		LayoutContainter	Elements;

		SlotContainer		InputSlots;

		String				DebugName;


		SInputLayoutDesc()
			: BaseOffset( 0 )
			, Elements()
			, InputSlots()
			, DebugName()
		{
		}

		SInputLayoutDesc( uint32 stride,
						  uint32 baseOffset,
						  LayoutContainter&& elements,
						  SlotContainer&& slots,
						  const String& debugName )
			: BaseOffset( baseOffset )
			, Elements( elements )
			, InputSlots( slots )
			, DebugName( debugName )
		{
		}

		void Clear()
		{
			Algo::SequentialFor( Elements.begin(), Elements.end(), ZeroElementCallback );

			Algo::SequentialFor( InputSlots.begin(), InputSlots.end(), ZeroSlotCallback );
		}

		void CalcStrides()
		{
			Algo::SequentialFor( Elements.begin(), Elements.end(), [this]( const LayoutUnit& unit ) { this->InputSlots[ unit.InputSlot ].Stride += unit.Element.SizeInBytes; } );
		}

	};

	struct SInputLayoutData;


	/************************************************************************/
	/*						RENDER STATE SPECIFIC STUFF                     */
	/************************************************************************/

	struct SRenderStateDesc
	{

		enum
		{
			MAX_RENDER_TARGETS = Limits::MAX_COLOR_RENDER_TARGES,
			MAX_VERTEX_BUFFERS = Limits::MAX_DEVICE_CONTEXT_VERTEX_BUFFERS
		};

		enum ERenderState
		{
			UNCOOKED,
			COOKED
		}
		eState;				// render state condition

		struct BufferUnit
		{
			BufferRef Buffer;

			uint32 InputSlot;

			uint32 Offset;

			BufferUnit()
				: Buffer()
				, InputSlot( 0 )
				, Offset( 0 )
			{
			}

			BufferUnit( BufferRef buffer,
						uint32 inputSlot, 
						uint32 offset )
				: Buffer( buffer )
				, InputSlot( inputSlot )
				, Offset( offset )
			{
			}
		};

		struct RenderTargetUnit
		{
			TextureRef RenderTarget = TextureRef::Null();

			int32 slice = -1;								// -1 for 2D textures
		};

		// #TODO temporary solution. Make support for uniform buffer objects.
		class ConstantCache
		{
		public:

			struct ConstantInfo
			{
				int32 Location;

			};

		public:

			ConstantCache() { }
			~ConstantCache() { }

			ConstantInfo& GetInfo( const String& name ) { return Cache[name]; }

			//const ConstantInfo& GetInfo( const String& name ) const { return Cache[name]; }

		private:

			HashMap< String, ConstantInfo > Cache;

		};


		Array< BufferUnit, MAX_VERTEX_BUFFERS > VertexBuffers;


		OPTIONAL BufferRef		IndexBuffer;

		REQUIRED InputLayoutRef InputLayout;


		REQUIRED ShaderRef VertexShader;

		OPTIONAL ShaderRef GeometryShader;				// not supported yet

		OPTIONAL ShaderRef TessControlShader;			// not supported yet

		OPTIONAL ShaderRef HullShader;					// not invented yet

		OPTIONAL ShaderRef ComputeShader;				// not supported yet

		REQUIRED ShaderRef PixelShader;


		// #TODO ALSO VIEWPORT HERE!!!


		OPTIONAL Array< RenderTargetUnit, MAX_RENDER_TARGETS > ColorTargets;	// use default render target if all the color targets are null

		OPTIONAL RenderTargetUnit DepthTarget;			// use default depth target if null

		OPTIONAL RenderTargetUnit StencilTarget;		// use default stencil target if null

	
		String					  DebugName;

		union URenderParams
		{
			struct 
			{
				uint8 UseBlending : 1;
				uint8 UseCulling : 1;
				// etc...

			};

			uint8 Data;
		}
		RenderParams;
	
		union URenderState
		{
			struct  
			{
				uint32 ShadersSet				: 1;		// 1 if both vertex and pixels shaders set
				uint32 ShadersCompiled			: 1;		// 0 if at least 1 of the shaders is not compiled
				uint32 VertexBuffersSet			: 1;		// buffers set?
				uint32 InputLayoutSet			: 1;		// is input layout presented?
				uint32 ColorTargetsSet			: 1;		// are color targets set (at least 1) ?
				uint32 DepthTargetSet			: 1;		// is depth target set?
				uint32 StencilTargetSet			: 1;		// is stencil target set?
				uint32 UseDefaultRenderTargets	: 1;		// render to native framebuffer. ColorTargetsCooked, DepthTargetCooked, StencilTargetCooked do not make sense if this flag is set
				uint32 ShadersCooked			: 1;		// are shaders linked into 1 program?
				uint32 ColorTargetsCooked		: 1;		// color buffers attached to the framebuffer?
				uint32 DepthTargetCooked		: 1;		// depth buffer attached to the framebuffer? / default depth target set?
				uint32 StencilTargetCooked		: 1;		// stencil buffer attached to the framebuffer? / default stencil target set>
				uint32 InputLayoutCooked		: 1;		// is input layout properly loaded on to the graphics card?
				uint32 VertexBuffersCooked		: 1;		// buffers attached to input slots ?
				
				uint32 RenderPipelineCooked		: 1;
			};

			uint32 Data;
		}
		RenderState;


		void Reset()
		{
			InputLayout			= InputLayoutRef::Null();
			
			VertexShader		= ShaderRef::Null();
			GeometryShader		= ShaderRef::Null();
			TessControlShader	= ShaderRef::Null();
			HullShader			= ShaderRef::Null();
			ComputeShader		= ShaderRef::Null();
			PixelShader			= ShaderRef::Null();

			Algo::SequentialFor( ColorTargets.begin(), ColorTargets.end(), []( RenderTargetUnit& target ) { target = { TextureRef::Null(), -1 }; } );

			DepthTarget			= { TextureRef::Null(), -1 };
			StencilTarget		= { TextureRef::Null(), -1 };

			RenderParams.Data = 0;
			RenderState.Data = 0;

			Algo::SequentialFor( VertexBuffers.begin(), VertexBuffers.end(), []( BufferUnit& unit ) { unit.Buffer = BufferRef::Null(); unit.InputSlot = 0; unit.Offset = 0; } );


			IndexBuffer			= BufferRef::Null();
			DebugName.clear();

			eState = UNCOOKED;
		}

		SRenderStateDesc()
		{
			Reset();
		}

		SRenderStateDesc( InputLayoutRef layoutRef, 
						  ShaderRef vertexShaderRef, 
						  ShaderRef geometryShaderRef,
						  ShaderRef tessControlShaderRef,
						  ShaderRef hullShader,
						  ShaderRef computeShader,
						  ShaderRef pixelShaderRef,
						  RenderTargetUnit depthTarget,
						  RenderTargetUnit stencilTarget,
						  BufferRef	indexBuffer,
						  ERenderState state,
						  const String& debugName,
						  JSpan< RenderTargetUnit, MAX_RENDER_TARGETS > colorTargets )
			: InputLayout( layoutRef )
			, VertexShader( vertexShaderRef )
			, GeometryShader( geometryShaderRef )
			, TessControlShader( tessControlShaderRef )
			, HullShader( hullShader )
			, ComputeShader( computeShader )
			, PixelShader( pixelShaderRef )
			, DepthTarget( depthTarget )
			, StencilTarget( stencilTarget )
			, IndexBuffer( indexBuffer )
			, DebugName( debugName )
			, eState( state )
		{
			for ( uint32 i = 0; i < MAX_RENDER_TARGETS; ++i )
			{
				ColorTargets[i] = colorTargets[i];
			}

			RenderParams.Data = 0;
			RenderState.Data = 0;
		}
	};

	struct SRenderSetup
	{
		using RenderTargetUnit	= SRenderStateDesc::RenderTargetUnit;
		using BufferUnit		= SRenderStateDesc::BufferUnit;


		OPTIONAL InputLayoutRef						InputLayout;

		OPTIONAL JSpan< const ShaderRef >			Shaders;

		REQUIRED bool								bUseDefaultTarges;

		/////////////////////////////////////////////////

		OPTIONAL JSpan< const RenderTargetUnit >	ColorTargets;

		OPTIONAL RenderTargetUnit					DepthTarget;
	
		OPTIONAL RenderTargetUnit					StencilTarget;

		/////////////////////////////////////////////////
	
		OPTIONAL JSpan< const BufferUnit >			VertexBuffers;

		OPTIONAL BufferRef							IndexBuffer;
	
		// #todo should add instanced drawing parameters?

		SRenderSetup()
			: InputLayout()
			, Shaders()
			, bUseDefaultTarges( true )
			, ColorTargets()
			, DepthTarget()
			, StencilTarget()
			, VertexBuffers()
			, IndexBuffer()
		{
		}

		SRenderSetup( InputLayoutRef inputLayout,
					  JSpan< const ShaderRef > shaders,
					  bool useDefaultTargets,
					  JSpan< const RenderTargetUnit > colorTargets,
					  RenderTargetUnit depthTarget,
					  RenderTargetUnit stencilTarget,
					  JSpan< const BufferUnit > vertexBuffers,
					  BufferRef indexBuffer )
			: InputLayout( inputLayout )
			, Shaders( shaders )
			, bUseDefaultTarges( useDefaultTargets )
			, ColorTargets( colorTargets )
			, DepthTarget( depthTarget )
			, StencilTarget( stencilTarget )
			, VertexBuffers( vertexBuffers )
			, IndexBuffer( indexBuffer )
		{
		}

	};


	struct SRenderStateData;

	/************************************************************************/
	/*                      TEXTURE SPECIFIC STUFF                          */
	/************************************************************************/

	enum class ETextureWrappingMode : uint8
	{
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER,
		MIRRORED_REPEAT,
		REPEAT,
		MIRROR_CLAMP_TO_EDGE
	};


	enum class ETextureWrappingAxis : uint8
	{
		S,
		T,
		R
	};

	enum class ETextureFilter : uint8
	{
		MIG_MAG_MIP_POINT,				// point sampling for minification, magnification and mip-level sampling.
		MIN_MAG_POINT_MIP_LINEAR,		// point sampling for minification and magnification, linear interpolation for mip-level sampling.
		MIN_POINT_MAG_LINEAR_MIP_POINT,	// point sampling for minification, linear interpolation for magnification and point for mip-level sampling.
		MIN_POINT_MAG_MIP_LINEAR,
		MIN_LINEAR_MAG_MIP_POINT,
		MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		MIN_MAG_LINEAR_MIP_POINT,
		MIN_MAG_MIP_LINEAR,
	};

	enum class ETextureChannel : uint8
	{
		R = JF_BIT( 0 ),
		G = JF_BIT( 1 ),
		B = JF_BIT( 2 ),
		A = JF_BIT( 3 ),

		RG		= R | G,
		RB		= R | B,
		BG		= B | G,
		RGB		= R | G | B,
		RGBA	= R | G | B | A,
	};

	DECLARE_ENUM_FLAG_OPERATIONS( ETextureChannel )

	enum class ETextureFormat : uint8
	{
		R,
		R8,
		R16,
		R16F,
		R32F,

		RG,
		RG8,
		RG16,
		RG16F,
		RG32F,

		RGB,
		RGB8,
		RGB16,
		RGB16F,
		RGB32F,

		RGBA,
		RGBA8,
		RGBA16F,
		RGBA32F,

		DEPTH,


		AUTO
	};

	enum class EPixelType : uint8
	{
		BYTE,
		UBYTE,
		SHORT,
		USHORT,
		INT,
		UINT,
		HFLOAT,
		FLOAT
	};

	enum class ETextureType : uint8
	{
		TEXTURE_1D,
		TEXTURE_2D,		//< the only one supported right now
		TEXTURE_3D,
		TEXTURE_CUBE,
		TEXTURE_ARRAY,	//< not supported yet
	};

	enum class ETextureAccessBits : uint8
	{
		NONE		= 0,
		READ		= JF_BIT( 0 ),
		WRITE		= JF_BIT( 1 ),
		REALLOCATE	= JF_BIT( 2 ),	//< immutable storage if not set

		ALL = READ | WRITE | REALLOCATE
	};

	DECLARE_ENUM_FLAG_OPERATIONS( ETextureAccessBits )

	using TextureModeParam = std::pair< ETextureWrappingAxis, ETextureWrappingMode >;


	struct STextureSubDataInfo
	{
		uint32 Width;
		uint32 Height;			//< 0 if not used
		uint32 Depth;			//< 0 if not used

		uint32 WidthOffset;
		uint32 HeightOffset;	//< 0 if not used
		uint32 DepthOffset;		//< 0 if not used

		uint32 DataSize;		//< 0 for automatic determination

		ETextureFormat DataFormat;

		STextureSubDataInfo()
			: Width( 0 )
			, Height( 0 )
			, Depth( 0 )
			, WidthOffset( 0 )
			, HeightOffset( 0 )
			, DepthOffset( 0 )
			, DataSize( 0 )
			, DataFormat( ETextureFormat::AUTO )
		{
		}

		STextureSubDataInfo( uint32 width,
							 uint32 height,
							 uint32 depth,
							 uint32 widthOffset,
							 uint32 heightOffset,
							 uint32 depthOffset,
							 uint32 dataSize,
							 ETextureFormat format )
			: Width( width )
			, Height( height )
			, Depth( depth )
			, WidthOffset( widthOffset )
			, HeightOffset( heightOffset )
			, DepthOffset( depthOffset )
			, DataSize( dataSize )
			, DataFormat( format )
		{
		}

	};

	struct STextureSizeInfo
	{
		uint32 Width;
		uint32 Height;
		uint32 Depth;			//< No support for depth maps for now. #todo add this feature

		uint32 ChannelsCount;

		uint8  ChannelSizeInBytes;

		STextureSizeInfo()
			: Width( 0 )
			, Height( 0 )
			, Depth( 0 )
			, ChannelsCount( 0 )
			, ChannelSizeInBytes( 0 )
		{
		}

		STextureSizeInfo( uint32 width, uint32 height, uint32 depth,
						  uint32 channels, uint8 channelSize )
			: Width( width )
			, Height( height )
			, Depth( depth )
			, ChannelsCount( channels )
			, ChannelSizeInBytes( channelSize )
		{
		}
	};

	struct STextureDesc
	{
		STextureSizeInfo		SizeInfo;			//< Texture size.

		uint32					MipmapsCount;

		bool					bHasMipchain;
		bool					bIsGamma;
		bool					bAllocated;			//< Is texture data allocated on the gpu?
		bool					bIsBound;

		ETextureFormat			eFormat;			//< Texture data format
		ETextureType			eType;
		ETextureFilter			eFilter;			//< filtering method

		JVector
		< TextureModeParam >	Modes;				//< Wrapping modes for different axises.

		String					DebugName;

		Vector4					BorderColor;
		Vector2					LodRange;

		float					LodBias;

		ETextureChannel			eTextureChannels;	//< Used texture data format channels.
		ETextureAccessBits		eAccess;			//< Permissions

		STextureDesc()
			: SizeInfo()
			, MipmapsCount( 0 )
			, bHasMipchain( false )
			, bIsGamma( false )
			, bAllocated( false )
			, bIsBound( false )
			, eFormat( ETextureFormat::AUTO )
			, eType( ETextureType::TEXTURE_2D )
			, eFilter ( ETextureFilter::MIN_POINT_MAG_MIP_LINEAR )
			, Modes()
			, DebugName( "" )
			, BorderColor()
			, LodRange( 0.f, 0.f )
			, LodBias( 0.f )
			, eTextureChannels( ETextureChannel::RGBA )
			, eAccess( ETextureAccessBits::ALL )
		{
		}

		STextureDesc( STextureSizeInfo sizeInfo,
					  uint32 mipmapsCount,
					  bool mipchain,
					  bool gamma,
					  bool loaded,
					  bool bound,
					  ETextureFormat format,
					  ETextureType type,
					  ETextureFilter filter,
					  const String& debugName,
					  const Vector4& borderColor,
					  const Vector2& lodRange,
					  float lodBias,
					  ETextureChannel channels,
					  ETextureAccessBits access )
			: SizeInfo( sizeInfo )
			, MipmapsCount( mipmapsCount )
			, bHasMipchain( mipchain )
			, bIsGamma( gamma )
			, bAllocated( loaded )
			, eFormat( format )
			, eType( type )
			, eFilter( filter )
			, bIsBound( bound )
			, DebugName( debugName )
			, BorderColor( borderColor )
			, LodRange( lodRange )
			, LodBias( lodBias )
			, eTextureChannels( channels )
			, eAccess( access )
		{
		}


		STextureDesc( STextureSizeInfo sizeInfo,
					  uint32 mipmapsCount,
					  bool mipchain,
					  bool gamma,
					  bool loaded,
					  bool bound,
					  ETextureFormat format,
					  ETextureType type,
					  ETextureFilter filter,
					  const JVector< TextureModeParam >& modes,
					  const String& debugName,
					  const Vector4& borderColor,
					  const Vector2& lodRange,
					  float lodBias,
					  ETextureChannel channels,
					  ETextureAccessBits access )

			: STextureDesc( sizeInfo, mipmapsCount,
							mipchain, gamma, loaded,
							bound, format, type, filter, 
							debugName, borderColor, lodRange,
							lodBias, channels, access )
		{
			Modes = modes;
		}

	};

	struct STextureInitData
	{
		STextureSizeInfo	SizeInfo;

		bool				bHasMipchain;
		bool				bIsGamma;

		ETextureFormat		eFormat;
		ETextureAccessBits	eAccess;
		ETextureType		eType;

		MemPtr				Data;

		uint32				DataOffset;
		uint32				DataSize;

		String				DebugName;


		STextureInitData()
			: SizeInfo()
			, bHasMipchain( false )
			, eFormat( ETextureFormat::AUTO )
			, eAccess( ETextureAccessBits::ALL )
			, eType( ETextureType::TEXTURE_2D )
			, DataOffset( 0 )
			, Data( NullPtr )
			, bIsGamma( false )
			, DataSize( 0 )
			, DebugName( "" )
		{
		}

		STextureInitData( STextureSizeInfo sizeInfo,
						  bool mipChain,
						  bool isGamma,
						  ETextureFormat format,
						  ETextureAccessBits access,
						  ETextureType type,
						  MemPtr data,
						  uint32 offset,
						  uint32 size )
			: SizeInfo( sizeInfo )
			, bHasMipchain( mipChain )
			, bIsGamma(isGamma )
			, eFormat( format )
			, eAccess( access )
			, eType( type )
			, Data( data )
			, DataOffset( offset )
			, DataSize( size )
		{
		}

	};


	/************************************************************************/
	/*						BUFFER SPECIFIC STUFF                           */
	/************************************************************************/

	// Assume that EBufferUsage constants match their direct OpenGL constants
	// and there is no need for tricky conversions to OpenGL enums.
	enum class EBufferUsage
	{
		NONE			= 0,					// Unknown
		STREAM_DRAW		= 0x88E0,				// GL_STREAM_DRAW
		STREAM_READ		= 0x88E1,				// GL_STREAM_READ		
		STREAM_COPY		= 0x88E2,				// GL_STREAM_COPY
		STATIC_DRAW		= 0x88E4,				// GL_STATIC_DRAW
		STATIC_READ		= 0x88E5,				// GL_STATIC_READ
		STATIC_COPY		= 0x88E6,				// GL_STATIC_COPY
		DYNAMIC_DRAW	= 0x88E8,				// GL_DYNAMIC_DRAW
		DYNAMIC_READ	= 0x88E9,				// GL_DYNAMIC_READ
		DYNAMIC_COPY	= 0x88EA				// GL_DYNAMIC_COPY
	};

	// Assume that EBufferType constants match their direct OpenGL constants
	// and there is no need for tricky conversions to OpenGL enums.
	enum class EBufferType
	{
		NONE						= 0,						// Unknown
		ARRAY_BUFFER				= 0x8892,					// Vertex attributes
		ATOMIC_COUNTER_BUFFER		= 0x92C0,					// Atomic counter storage
		COPY_READ_BUFFER			= 0x8F36,					// Buffer copy source
		COPY_WRITE_BUFFER			= 0x8F37,					// Buffer copy destination
		DISPATCH_INDIRECT_BUFFER	= 0x90EE,					// Indirect compute dispatch commands
		DRAW_INDIRECT_BUFFER		= 0x8F3F,					// Indirect command arguments
		ELEMENT_ARRAY_BUFFER		= 0x8893,					// Vertex array indices
		PIXEL_PACK_BUFFER			= 0x88EB,					// Pixel read target
		PIXEL_UNPACK_BUFFER			= 0x88EC,					// Texture data source
		QUERY_BUFFER				= 0x9192,					// Query result buffer
		SHADER_STORAGE_BUFFER		= 0x90D2,					// Read - write storage for shaders
		TEXTURE_BUFFER				= 0x8C2A,					// Texture data buffer
		TRANSFORM_FEEDBACK_BUFFER	= 0x8C8E,					// Transform feedback buffer
		UNIFORM_BUFFER				= 0x8A11,					// Uniform block storage
	};

	enum class EBufferAccessBits : uint8
	{
		NONE			= 0,
		READ			= JF_BIT( 0 ),
		WRITE			= JF_BIT( 1 ),	// GL_DYNAMIC_STORAGE_BIT
		DYNAMIC_READ	= JF_BIT( 3 ),	// GL_MAP_READ_BIT				#TODO implement GPU pointers! GL_MAP_PERSISTENT_BIT??
		DYNAMIC_WRITE	= JF_BIT( 4 ),	// GL_MAP_WRITE_BIT
		REALLOCATE		= JF_BIT( 5 ),	// Immutable storage

		ALL = READ | WRITE | DYNAMIC_READ | DYNAMIC_WRITE | REALLOCATE
	};

	// Immutable buffer - Flags & EBufferAccessBits::WRITE == 0

	DECLARE_ENUM_FLAG_OPERATIONS( EBufferAccessBits );


	struct SBufferDesc
	{

		EBufferType			eType;
		EBufferUsage		eUsage;

		bool				bBoundToCurrentContext;
		bool				bInvalidated;
		bool				bStorageAllocated;

		uint32				Size;
		String				DebugName;

		EBufferAccessBits	eAccess;


		SBufferDesc()
			: eType( EBufferType::NONE )
			, eUsage( EBufferUsage::NONE )
			, bBoundToCurrentContext( false )
			, bInvalidated( false )
			, bStorageAllocated( false )
			, Size( 0 )
			, DebugName()
			, eAccess( EBufferAccessBits::NONE )
		{
		}

	};

	struct SBufferInitData
	{
		EBufferType			eType;		//< buffer type
		EBufferUsage		eUsage;		//< buffer usage

		uint32				DataOffset;	//< data offset
		String				DebugName;	//< debug name (maybe unused)

		uint32				Size;		//< data size
		MemPtr				Data;		//< pointer to data (maybe null)

		EBufferAccessBits	eAccess;	//< buffer access flags

		SBufferInitData()
			: eType( EBufferType::NONE )
			, eUsage( EBufferUsage::NONE )
			, eAccess( EBufferAccessBits::NONE )
			, Size( 0 )
			, DataOffset( 0 )
			, DebugName()
			, Data( NullPtr )
		{
		}

		SBufferInitData( EBufferType		type,
						 EBufferUsage		usage,
						 uint32				offset,
						 const String&		debugName,
						 uint32				size,
						 MemPtr				data,
						 EBufferAccessBits	access )
			: eType( type )
			, eUsage( usage )
			, eAccess( access )
			, Size( size )
			, DataOffset( offset )
			, DebugName( debugName )
			, Data( data )
		{
		}
	};

	/************************************************************************/
	/*					SHADER SPECIFIC STUFF                               */
	/************************************************************************/

	enum class EShaderType : uint8
	{
		VERTEX_SHADER = 0,
		TESSELATION_SHADER,
		COMPUTE_SHADER,
		GEOMETRY_SHADER,
		HULL_SHADER,
		PIXEL_SHADER
	};
	

	struct SShaderDesc
	{
		EShaderType		Type;

		ResourceView	ByteCode;

		bool			bCompiled;

		String			DebugName;

		SShaderDesc()
			: Type( EShaderType::VERTEX_SHADER )
			, ByteCode()
			, bCompiled( false )
			, DebugName( "" )
		{
		}

		SShaderDesc( EShaderType type,
					 ResourceView code,
					 bool compiled,
					 const String& debugName )
			: Type( type )
			, ByteCode( code )
			, bCompiled( compiled )
			, DebugName( debugName )
		{
		}
	};


	struct SShaderData;


	struct SShaderInitData
	{
		ResourceView	Code;

		SShaderInitData()
			: Code()
		{
		}

		SShaderInitData( ResourceView code )
			: Code( code )
		{
		}

	};

	struct SPipelineStageInfo;	// see OpenGLShader.h


	/************************************************************************/
	/*							GPU FENCE                                   */
	/************************************************************************/


	struct SGpuFence
	{

		virtual void Lock() = 0;
		virtual bool Lock( Utils::TimeSpan timeout ) = 0;

		virtual void Unlock() = 0;

		virtual bool IsLocked() const = 0;

		// #TODO should lock automatically?
		virtual bool WaitForLock() = 0;
		virtual bool WaitForLock( Utils::TimeSpan timeout ) = 0;

	};

	struct SGpuFenceDesc
	{
		// #TODO MAKE IT
	};


	/************************************************************************/
	/*						GPU API SPECIFIC DATA                           */
	/************************************************************************/



}


namespace UE::Graphics::GpuApi
{
	// HELPERS

	INLINE uint32 GetPixelSize( ETextureFormat format )
	{
		switch( format )
		{
			case ETextureFormat::R:
			case ETextureFormat::R8:
			case ETextureFormat::DEPTH:
				return 1;

			case ETextureFormat::R16:
			case ETextureFormat::R16F:
			case ETextureFormat::RG:
			case ETextureFormat::RG8:
				return 2;

			case ETextureFormat::RGB:
			case ETextureFormat::RGB8:
				return 3;

			case ETextureFormat::RG16:
			case ETextureFormat::RG16F:
			case ETextureFormat::R32F:
			case ETextureFormat::RGBA:
			case ETextureFormat::RGBA8:
				return 4;

			case ETextureFormat::RGB16:
			case ETextureFormat::RGB16F:
				return 6;

			case ETextureFormat::RG32F:
			case ETextureFormat::RGBA16F:
				return 8;

			default:
				// #todo LOG THIS
				return 0;
		}
	}


	INLINE uint32 GetChannelSize( ETextureFormat format )
	{
		switch( format )
		{

			case ETextureFormat::R:
			case ETextureFormat::R8:
			case ETextureFormat::RG:
			case ETextureFormat::RG8:
			case ETextureFormat::RGB:
			case ETextureFormat::RGB8:
			case ETextureFormat::RGBA:
			case ETextureFormat::RGBA8:
				return 1;

			case ETextureFormat::R16:
			case ETextureFormat::R16F:
			case ETextureFormat::RG16:
			case ETextureFormat::RG16F:
			case ETextureFormat::RGB16:
			case ETextureFormat::RGB16F:
			case ETextureFormat::RGBA16F:
				return 2;

			case ETextureFormat::R32F:
			case ETextureFormat::RG32F:
				return 4;

			default:
				// #todo HALT
				return 0;

		}
	}

	INLINE uint32 GetChannelsCount( ETextureFormat format )
	{
		switch ( format )
		{
			case ETextureFormat::R:
			case ETextureFormat::R8:
			case ETextureFormat::R16:
			case ETextureFormat::R16F:
			case ETextureFormat::R32F:
				return 1;

			case ETextureFormat::RG:
			case ETextureFormat::RG8:
			case ETextureFormat::RG16:
			case ETextureFormat::RG16F:
			case ETextureFormat::RG32F:
				return 2;

			case ETextureFormat::RGB:
			case ETextureFormat::RGB8:
			case ETextureFormat::RGB16:
			case ETextureFormat::RGB16F:
				return 3;

			case ETextureFormat::RGBA:
			case ETextureFormat::RGBA8:
			case ETextureFormat::RGBA16F:
				return 4;

			default:
			// #todo HALT
				return 0;
		}
	}

	INLINE uint32 GetChannelsCount( ETextureChannel channels )
	{
		switch( channels )
		{
			case ETextureChannel::RGBA:
				return 4;

			case ETextureChannel::RGB:
				return 3;

			case ETextureChannel::RG:
			case ETextureChannel::RB:
			case ETextureChannel::BG:
				return 2;

			case ETextureChannel::R:
			case ETextureChannel::G:
			case ETextureChannel::B:
			case ETextureChannel::A:
				return 1;

			default:
				// #todo HALT
				return 0;
		}
	}

	INLINE ETextureChannel GetChannels( ETextureFormat format )
	{
		switch ( format )
		{
			case ETextureFormat::R:
			case ETextureFormat::R8:
			case ETextureFormat::R16:
			case ETextureFormat::R16F:
			case ETextureFormat::R32F:
				return ETextureChannel::A;

			case ETextureFormat::RG:
			case ETextureFormat::RG8:
			case ETextureFormat::RG16:
			case ETextureFormat::RG16F:
			case ETextureFormat::RG32F:
				return ETextureChannel::RG;

			case ETextureFormat::RGB:
			case ETextureFormat::RGB8:
			case ETextureFormat::RGB16:
			case ETextureFormat::RGB16F:
				return ETextureChannel::RGB;

			case ETextureFormat::RGBA:
			case ETextureFormat::RGBA8:
			case ETextureFormat::RGBA16F:
				return ETextureChannel::RGBA;

			default:
				// #todo HALT
				return ETextureChannel::RGB;
		}
	}

	INLINE EPixelType GetPixelType( ETextureFormat format )
	{
		switch ( format )
		{

			case ETextureFormat::R:
			case ETextureFormat::R8:
			case ETextureFormat::RG:
			case ETextureFormat::RG8:
			case ETextureFormat::RGB:
			case ETextureFormat::RGB8:
			case ETextureFormat::RGBA:
			case ETextureFormat::RGBA8:
				return EPixelType::UBYTE;

			case ETextureFormat::R16:
			case ETextureFormat::R16F:
			case ETextureFormat::RG16:
			case ETextureFormat::RG16F:
			case ETextureFormat::RGB16:
			case ETextureFormat::RGB16F:
			case ETextureFormat::RGBA16F:
				return EPixelType::HFLOAT;

			case ETextureFormat::R32F:
			case ETextureFormat::RG32F:
				return EPixelType::FLOAT;

			default:
				// #todo HALT
				return EPixelType::UBYTE;

		}
	}


	INLINE ETextureFormat GetPixelFormat( ETextureFormat textureFormat )
	{
		switch ( textureFormat )
		{
			case ETextureFormat::R:
			case ETextureFormat::R8:
			case ETextureFormat::R16:
			case ETextureFormat::R16F:
			case ETextureFormat::R32F:
				return ETextureFormat::R;

			case ETextureFormat::RG:
			case ETextureFormat::RG8:
			case ETextureFormat::RG16:
			case ETextureFormat::RG16F:
			case ETextureFormat::RG32F:
				return ETextureFormat::RG;

			case ETextureFormat::RGB:
			case ETextureFormat::RGB8:
			case ETextureFormat::RGB16:
			case ETextureFormat::RGB16F:
			case ETextureFormat::RGB32F:
				return ETextureFormat::RGB;

			case ETextureFormat::RGBA:
			case ETextureFormat::RGBA8:
			case ETextureFormat::RGBA16F:
			case ETextureFormat::RGBA32F:
				return ETextureFormat::RGBA;

			case ETextureFormat::DEPTH:
				return ETextureFormat::DEPTH;

			default:
				return ETextureFormat::AUTO;
		}
	}


	INLINE uint32 CalcMipmapsCount( uint32 width, uint32 height )
	{
		return Math::Log2( Math::Max( width, height ) );
	}


	// #TODO IMPLEMENT OTHERS, GOTTA DEAL WITH REDEFINITION COMPILER ERROR. HANDLE IT SOMEHOW

	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::UNKNOWN >() { return { EInputElementFormat::UNKNOWN, 0, 0, 0 }; }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32G32B32A32_TYPELESS >() { return { EInputElementFormat::R32G32B32A32_TYPELESS, 16, 4, 1 }; }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32G32B32A32_FLOAT >() { return { EInputElementFormat::R32G32B32A32_FLOAT, 16, 4, 1 }; }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32G32B32A32_UINT >() { return { EInputElementFormat::R32G32B32A32_UINT, 16, 4, 1 }; }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32G32B32A32_SINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32G32B32_TYPELESS >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32G32B32_FLOAT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32G32B32_UINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32G32B32_SINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16G16B16A16_TYPELESS >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16G16B16A16_FLOAT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16G16B16A16_UNORM >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16G16B16A16_UINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16G16B16A16_SNORM >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16G16B16A16_SINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32G32_TYPELESS >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32G32_FLOAT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32G32_UINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32G32_SINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8G8B8A8_TYPELESS >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8G8B8A8_UNORM >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8G8B8A8_UNORM_SRGB >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8G8B8A8_UINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8G8B8A8_SNORM >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8G8B8A8_SINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16G16_TYPELESS >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16G16_FLOAT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16G16_UNORM >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16G16_UINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16G16_SNORM >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16G16_SINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32_TYPELESS >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::D32_FLOAT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32_FLOAT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32_UINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R32_SINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8G8_TYPELESS >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8G8_UNORM >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8G8_UINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8G8_SNORM >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8G8_SINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16_TYPELESS >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16_FLOAT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::D16_UNORM >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16_UNORM >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16_UINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16_SNORM >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R16_SINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8_TYPELESS >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8_UNORM >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8_UINT >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8_SNORM >() { }
	//template<> SInputElement SInputElement::Acquire< EInputElementFormat::R8_SINT >() { }

#include <type_traits>
#include <concepts>

	//	#TODO CHECK THIS OUT. IT DOES NOT WORK

	template< EInputElementFormat _Format >
	concept Validated = requires() { SInputElement::Acquire< _Format >(); };
	
	constexpr static bool _validated = Validated< EInputElementFormat::UNKNOWN >;
	constexpr static bool _validated2 = Validated< EInputElementFormat::R16_SINT >;

	//template< EInputElementFormat _Ty >
	//struct IsValidated
	//{
	//	template< EInputElementFormat _Format > requires ( SInputElement::Acquire< _Format >() -> SInputElement )
	//	constexpr static bool IsAcquired() { return true; }

	//	//template< EInputElementFormat _Format >
	//	//constexpr static bool IsAcquired() { return false; }

	//	constexpr static bool Value = IsAcquired< _Ty >();
	//};

	//constexpr static bool value = IsValidated< EInputElementFormat::UNKNOWN >::Value;

}
