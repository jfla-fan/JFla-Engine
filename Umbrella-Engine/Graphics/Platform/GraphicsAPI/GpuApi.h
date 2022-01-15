#pragma once

#include "GpuTypes.h"


// #TODO IMPLEMENT THIS AS GPU LAYER OVER OPENGL CONTEXT AND OPENGL ENTITIES (OpenGLTexture, OpenGLBuffer, etc ...)
// Current implementation: see Graphics/Platform/GraphicsAPI/OpenGL/OpenGLGpuApi.cpp


namespace J::Graphics::GpuApi
{
	// #TODO aliases for OpenGL types ?

	//class JTexture;
	//class JVertexBuffer;
	//class JIndexBuffer;
	//class JVertexArray;
	//class JShader;


	/************************************************************************/
	/*						GPU API INTERFACE                               */
	/************************************************************************/

	SDeviceData&				GetDeviceData();

	bool						InitDevice();

	EDeviceType					GetDeviceType();

	EGPUVendor					GetGPUVendor();


	// #todo implement functions related to drawing and other functionality

	TextureRef UploadTexture ( const STextureInitData& data );

	
	// #TODO CreateTexture, UploadTexture, CreateGpuFence, ...


	BufferRef					CreateBuffer( EBufferType type = EBufferType::NONE,
											  EBufferUsage usage = EBufferUsage::NONE,
											  EBufferAccessBits access = EBufferAccessBits::ALL,
											  const JString& debugName = "" );

	BufferRef					CreateBuffer( const SBufferInitData& initData );
	

	void						DestroyBuffer( BufferRef ref );

	void						BindBuffer( BufferRef ref );

	void						UnbindBuffer( BufferRef ref );

	bool						IsBufferBound( BufferRef ref );

	SBufferDesc					GetBufferDesc( BufferRef ref );

	/**
	 * Read size bytes of memory with a given offset. Returns allocated memory or null
	 * in case of failure.
	 * Buffer must be readable (have EBufferAccessBits::READ flag).
	 * 
	 * \param ref
	 * \param offset
	 * \param size
	 */
	MemPtr						ReadBufferData( BufferRef ref, SIZE_T offset, SIZE_T size );

	void						ReadBufferData( BufferRef ref, SIZE_T offset, SIZE_T size, MemPtr storage );

	/**
	 * Read size bytes of memory using internal buffer offset value.
	 * Buffer must be readable (have EBufferAccessBits::READ flag).
	 * 
	 * \param ref
	 * \param size
	 */
	MemPtr						ReadBufferData( BufferRef ref, SIZE_T size );

	void						ReadBufferData( BufferRef ref, SIZE_T size, MemPtr storage );

	/**
	 * Read the whole buffer data. Returns null in case of failure or if data is not allocated.
	 * Buffer must be readable (have EBufferAccessBits::READ flag).
	 * 
	 * \param ref
	 */
	MemPtr						ReadBufferData( BufferRef ref );

	void						ReadBufferData( BufferRef ref, MemPtr storage );


	void						WriteBufferData( BufferRef ref, SIZE_T offset, SIZE_T size, MemPtr source );

	void						WriteBufferData( BufferRef ref, SIZE_T size, MemPtr source );


	// make other functions - write buffer sub data

	void						CopyBufferData( BufferRef srcRef, BufferRef dstRef, SIZE_T srcOffset, SIZE_T dstOffset, SIZE_T size );

	/**
	 * Allocate a new buffer storage and upload data into it (no upload if data is null). 
	 * Does nothing if buffer was previously created with no REALLOCATE flag (immutable) or size is 0. 
	 * Otherwise, if given access bits do not contain REALLOCATE flag, allocates immutable storage
	 * with no future opportunity to reallocate this storage. 
	 * If accessBits contain REALLOCATE flag, allocates mutable storage with further possibility to change it.
	 * 
	 * \param ref			- buffer id.
	 * \param size			- new data size (maybe 0).
	 * \param offset		- buffer offset.
	 * \param data			- pointer to data (maybe null).
	 * \param accessBits	- access bits.
	 */
	void						AllocateBufferStorage( BufferRef ref, SIZE_T size,
													   SIZE_T offset = 0, MemPtr data = NullPtr,
													   EBufferAccessBits accessBits = EBufferAccessBits::ALL );

	void						SetBufferDebugName( BufferRef ref, const JString& name );

	JString						GetBufferDebugName( BufferRef ref );


}
