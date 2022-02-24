#pragma once
#include "GpuTypes.h"



namespace UE::Graphics::GpuApi
{


	/************************************************************************/
	/*							BUFFERS                                     */
	/************************************************************************/



	BufferRef	CreateBuffer( EBufferType type = EBufferType::NONE,
							  EBufferUsage usage = EBufferUsage::NONE,
							  EBufferAccessBits access = EBufferAccessBits::ALL,
							  const String& debugName = "" );

	BufferRef	CreateBuffer( const SBufferInitData& initData );


	void		DestroyBuffer( BufferRef ref );

	bool		IsResourceAllocated( BufferRef ref );

	bool		IsBufferStorageAllocated( BufferRef ref );

	void		BindBuffer( BufferRef ref );

	void		UnbindBuffer( BufferRef ref );

	bool		IsBufferBound( BufferRef ref );

	SBufferDesc	GetBufferDesc( BufferRef ref );

	/**
	 * Read size bytes of memory with a given offset. Returns allocated memory or null
	 * in case of failure.
	 * Buffer must be readable (have EBufferAccessBits::READ flag).
	 *
	 * \param ref
	 * \param offset
	 * \param size
	 */
	MemPtr		ReadBufferData( BufferRef ref, uint32 offset, uint32 size );

	void		ReadBufferData( BufferRef ref, uint32 offset, uint32 size, MemPtr storage );

	/**
	 * Read the whole buffer with a given offset.
	 * Buffer must be readable (have EBufferAccessBits::READ flag).
	 *
	 * \param ref
	 * \param size
	 */
	MemPtr		ReadBufferData( BufferRef ref, uint32 offset );

	void		ReadBufferData( BufferRef ref, uint32 offset, MemPtr storage );

	/**
	 * Read the whole buffer data. Returns null in case of failure or if data is not allocated.
	 * Buffer must be readable (have EBufferAccessBits::READ flag).
	 *
	 * \param ref
	 */
	MemPtr		ReadBufferData( BufferRef ref );

	void		ReadBufferData( BufferRef ref, MemPtr storage );


	void		WriteBufferData( BufferRef ref, uint32 offset, uint32 size, MemPtr source );

	void		WriteBufferData( BufferRef ref, uint32 size, MemPtr source );


	// make other functions - write buffer sub data

	void		CopyBufferData( BufferRef srcRef, BufferRef dstRef, uint32 srcOffset, uint32 dstOffset, uint32 size );

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
	void		AllocateBufferStorage( BufferRef ref, uint32 size,
									   uint32 offset = 0, MemPtr data = NullPtr,
									   EBufferAccessBits accessBits = EBufferAccessBits::ALL );

	void		SetBufferDebugName( BufferRef ref, const String& name );

	String		GetBufferDebugName( BufferRef ref );


	uint32		GetBufferNativeResourceDEBUG( BufferRef ref );
}
