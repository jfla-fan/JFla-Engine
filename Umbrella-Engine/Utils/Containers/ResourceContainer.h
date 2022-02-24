#pragma once
#include "../../Core.h"



namespace UE::Utils
{

	/**
	* 
	 * Container for keeping GPU resources.
	 * Make sure _ResRef type is a resource ( see Graphics/Platform/GraphicsAPI/gpuResourceRef.inl )
	 * 
	 */
	template < typename _ResRef, typename _Value, SIZE_T MAX_COUNT = UINT8_MAX >
	class ResourceContainer
	{
	public:

		static_assert( MAX_COUNT <= UINT16_MAX, "Too many resources in a container." );

		enum { MaxResCount = MAX_COUNT, ZeroId = 0 };

	public:

		ResourceContainer( TMutex& mutex )
			: UnusedIndicesCount( 0 )
			, Mutex( mutex )
		{
			UnusedIndicesCount = MaxResCount;

			for (uint32 i = 0; i < MaxResCount; ++i)
			{
				UnusedDefaultIndices[i] = i;
			}
		}
		
		~ResourceContainer()
		{
			DestroyAll();
		}

		/// Is given id in use (0 id is never in use)
		bool IsInUse( const _ResRef& id ) const
		{
			return IsValidId( id ) && IdToIndex( id ) < MaxResCount && int32( Resources[IdToIndex(id)].RefCount ) >= 0;
		}

		/// Is container empty?
		bool IsEmpty() const
		{
			JF_ASSERT( UnusedIndicesCount <= MaxResCount, "Unexpected unusedIndices count." );
			return ( MaxResCount == UnusedIndicesCount );
		}

		bool IsCapableToCreate( uint32 count )
		{
			uint32 unused = UnusedIndicesCount;
			return count && unused - count >= 0;
		}

		const _ResRef Create( int32 initialRefCount = 1 )
		{
			JF_ASSERT( initialRefCount >= 0, "Reference count cannot be negative." );
			
			// lock mutex
			JF_SCOPED_LOCK( Mutex );

			uint32 index = MaxResCount;
			if ( UnusedIndicesCount > 0 )
			{
				index = UnusedDefaultIndices[--UnusedIndicesCount];
				JF_ASSERT( index < MaxResCount, "Unexpected index in resource container. Abort." );
			}

			// failed to create a resource
			if ( index >= MaxResCount )
			{
				// #TODO WARN ABOUT FAILURE

				return _ResRef::Null();
			}

			// build id from index
			const _ResRef newId = { IndexToId(index) };

			SData& data = Resources[index];

			JF_ASSERT( data.RefCount == -1, "Unexpected ref count in recently allocated resource." );
			data.RefCount = initialRefCount;

			JF_ASSERT( IsInUse(newId), "Recently created resource is not in use. Strange..." );

			return newId;
		}

		void Destroy( const _ResRef& ref )
		{
			JF_SCOPED_LOCK( Mutex );

			JF_ASSERT( IsInUse(ref), "Resource container: reference you are going to delete must be in use." );

			uint32 index = IdToIndex( ref );

			JF_ASSERT( Resources[index].RefCount == 0, "Cannot delete resource that is still in use." );

			if ( UnusedIndicesCount >= MaxResCount )
			{
				// #TODO HUGE WARNING: SHIT IS GOING TO HAPPEN
			}

			UnusedDefaultIndices[++UnusedIndicesCount - 1] = index;

			// reset resource
			
			Resources[index].RefCount = -1;
			Resources[index].Data.~_Value();

			new ( &Resources[index].Data ) _Value();	// initialize with default value
		
			// post check
			JF_ASSERT( !IsInUse( ref ), "" );
		}

		// shutdown
		void DestroyAll()
		{
			if ( !IsEmpty() )
			{
				for ( uint32 i = 0; i < MaxResCount; ++i )
				{
					const _ResRef ref { IndexToId( i ) };
					
					if ( IsInUse( ref ) )
					{

						SData& data = Resources[IdToIndex( ref )];
						data.RefCount = -1;

						data.Data.~_Value();
						new ( &data.Data ) _Value();	// initialize with a default value

						// give back unused index
						const uint32 unusedIndex = UnusedIndicesCount++;
						JF_ASSERT( unusedIndex < MaxResCount, "Resource container memory will get corrupted." );
						UnusedDefaultIndices[unusedIndex] = i;

					}

					JF_ASSERT( !IsInUse( ref ), "Resource should've been destroyed." );
				}
			}

			JF_ASSERT( IsEmpty(), "Container must've been cleared." );
		}

		_Value& Data( const _ResRef& ref )
		{
			JF_ASSERT( IsInUse( ref ), "Resource does not exist." );
			return Resources[IdToIndex( ref )].Data;
		}

		const _Value& Data( const _ResRef& ref ) const
		{
			JF_ASSERT( IsInUse( ref ), "Resource does not exist." );
			return Resources[IdToIndex( ref )].Data;
		}

		int32 IncRefCount( const _ResRef& ref )
		{
			JF_ASSERT( IsInUse( ref ), "Cannot increment invalid resource." );
			SData& data = Resources[IdToIndex( ref )];

			int32 newRefCount = ++data.RefCount;
			
			JF_ASSERT( newRefCount > 1, "Addref'ing a resource ref that was already sent to destruction." );
			
			return newRefCount;
		}

		int32 DecRefCount( const _ResRef& ref )
		{
			JF_ASSERT( IsInUse( ref ), "Cannot increment invalid resource." );
			SData& data = Resources[IdToIndex( ref )];

			int32 newRefCount = --data.RefCount;

			JF_ASSERT( newRefCount >= 0, "Decref'ing a resource ref too many times." );

			return newRefCount;
		}

		int32 GetRefCount( const _ResRef& ref )
		{
			JF_ASSERT( IsInUse( ref ), "Resource must be in use." );
			return int32( Resources[IdToIndex( ref )].RefCount );
		}

		uint32 GetUnusedCount() const
		{
			return UnusedIndicesCount;
		}

		uint32 GetUsedCount() const
		{
			return MaxResCount - UnusedIndicesCount;
		}

	protected:

		static FORCEINLINE bool IsValidId( uint32 id ) { return id != ZeroId; }
		static FORCEINLINE bool IsValidId( const _ResRef& ref ) { return ref.ID() != ZeroId; }

		static FORCEINLINE uint32 IdToIndex( uint32 id ) { JF_ASSERT( id > 0, "" ); return id - 1; }
		static FORCEINLINE uint32 IdToIndex( const _ResRef& ref ) { JF_ASSERT( ref.ID() > 0, "" ); return ref.ID() - 1; }

		static FORCEINLINE uint32 IndexToId( uint32 index ) { return index + 1; }

	private:

		struct SData
		{
			SData()
				: RefCount( -1 )
			{
			}

			Atomic::TAtomic< int32 > RefCount;		// -1 - unused reference
			_Value Data;
		};

		TMutex&						Mutex;

		Atomic::TAtomic< int32 >	UnusedIndicesCount;

		SData						Resources[MaxResCount];				// Resource[0] is reserved 
		uint32						UnusedDefaultIndices[MaxResCount];	//
	};

}