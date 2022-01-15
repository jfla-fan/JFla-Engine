#pragma once

#include "../../../Core.h"
#include "../../../Utils/Time/TimeSpan.h"



namespace J::Graphics::GpuApi
{
	struct TextureRef;
	struct ShaderRef;
	struct BufferRef;
	struct VertexArrayRef;


#define RES_TYPE TextureRef
#include "gpuResourceRef.inl"

#define RES_TYPE ShaderRef
#include "gpuResourceRef.inl"

#define RES_TYPE BufferRef
#include "gpuResourceRef.inl"

#define RES_TYPE VertexArrayRef
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
		S = 1,
		T = 2,
		R = 4,
		ST = S | T,
		SR = S | R,
		TR = T | R,
		All = S | T | R
	};

	DECLARE_ENUM_FLAG_OPERATIONS(ETextureWrappingAxis)


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

		RGBA,
		RGBA8,
		RGBA16F,

		DEPTH,


		AUTO
	};


	struct STextureSizeInfo
	{
		uint32 Width;
		uint32 Height;
		uint32 Depth;

		uint32 ChannelsCount;

		STextureSizeInfo()
			: Width(0), Height(0), Depth(0), ChannelsCount(0)
		{
		}

		STextureSizeInfo(uint32 width, uint32 height, uint32 depth, uint32 channels)
			: Width(Width), Height(Height), Depth(Depth), ChannelsCount(channels)
		{
		}
	};


	/************************************************************************/
	/*						BUFFER SPECIFIC STUFF                           */
	/************************************************************************/

	// Assume that EBufferUsage constants are equal to their direct OpenGL constants
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

	// Assume that EBufferType constants are equal to their direct OpenGL constants
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
		READ			= JF_BIT(0),
		WRITE			= JF_BIT(1),	// GL_DYNAMIC_STORAGE_BIT
		DYNAMIC_READ	= JF_BIT(3),	// GL_MAP_READ_BIT				#TODO implement GPU pointers! GL_MAP_PERSISTENT_BIT??
		DYNAMIC_WRITE	= JF_BIT(4),	// GL_MAP_WRITE_BIT
		REALLOCATE		= JF_BIT(5),	// Immutable storage

		ALL = READ | WRITE | DYNAMIC_READ | DYNAMIC_WRITE | REALLOCATE
	};

	// Immutable buffer - Flags & (EBufferAccessBits::READ || EBufferAccessBits::WRITE) == 0

	DECLARE_ENUM_FLAG_OPERATIONS(EBufferAccessBits);


	/************************************************************************/
	/*					SHADER SPECIFIC STUFF                               */
	/************************************************************************/

	enum class EShaderType : uint8
	{
		VERTEX_SHADER = 0,
		TESSELATION_SHADER,
		COMPUTE_SHADER,
		GEOMETRY_SHADER,
		FRAGMENT_SHADER,
		STAGES_COUNT
	};


	struct SPipelineStageInfo;	// see OpenGLShader.h


	/************************************************************************/
	/*					VERTEX ARRAY SPECIFIC STUFF                         */
	/************************************************************************/




	/************************************************************************/
	/*							GPU FENCE                                   */
	/************************************************************************/


	struct SGpuFence
	{

		virtual void Lock() = 0;
		virtual bool Lock(Utils::TimeSpan timeout) = 0;

		virtual void Unlock() = 0;

		virtual bool IsLocked() const = 0;

		// #TODO should lock automatically?
		virtual bool WaitForLock() = 0;
		virtual bool WaitForLock(Utils::TimeSpan timeout) = 0;

	};

	struct SGpuFenceDesc
	{
		// #TODO MAKE IT
	};


	/************************************************************************/
	/*						GPU API SPECIFIC DATA                           */
	/************************************************************************/


	struct STextureInitData
	{
		// ...
	};

	struct SShaderDesc
	{

	};

	struct SBufferDesc
	{

		EBufferType			eType;
		EBufferUsage		eUsage;

		bool				bBoundToCurrentContext;
		bool				bInvalidated;
		bool				bStorageAllocated;

		SIZE_T				Size;
		SIZE_T				Offset;						//< buffer offset (when the actual data on the gpu starts)
		JString				DebugName;

		EBufferAccessBits	eAccess;

		SBufferDesc()
			: eType(EBufferType::NONE)
			, eUsage(EBufferUsage::NONE)
			, bBoundToCurrentContext(false)
			, bInvalidated(false)
			, bStorageAllocated(false)
			, Size(0)
			, Offset(0)
			, DebugName()
			, eAccess(EBufferAccessBits::NONE)
		{
		}

	};

	struct SBufferInitData
	{
		EBufferType			eType;		//< buffer type
		EBufferUsage		eUsage;		//< buffer usage

		SIZE_T				Offset;		//< buffer offset
		JString				DebugName;	//< debug name (maybe unused)

		SIZE_T				Size;		//< data size
		MemPtr				Data;		//< pointer to data (maybe null)

		EBufferAccessBits	eAccess;	//< buffer access flags

		SBufferInitData()
			: eType( EBufferType::NONE )
			, eUsage( EBufferUsage::NONE )
			, eAccess( EBufferAccessBits::NONE )
			, Size( 0 )
			, Offset( 0 )
			, DebugName()
			, Data( NullPtr )
		{
		}

		SBufferInitData( EBufferType	type,
						 EBufferUsage usage,
						 SIZE_T offset,
						 const JString& debugName,
						 SIZE_T				size,
						 MemPtr				data,
						 EBufferAccessBits	access )
			: eType( type )
			, eUsage( usage )
			, eAccess( access )
			, Size( size )
			, Offset( offset )
			, DebugName( debugName )
			, Data( data )
		{
		}
	};

}
