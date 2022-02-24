#pragma once

#include "../Core.h"



namespace UE
{

	class ResourceView
	{
	public:

		constexpr ResourceView()
			: DataPtr( NullPtr )
			, DataSize( 0 )
		{
		}

		constexpr ResourceView( CMemPtr data, uint32 size )
			: DataPtr( data )
			, DataSize( size )
		{
		}
		
		constexpr ResourceView( CMemPtr startPtr, CMemPtr endPtr )
			: DataPtr( startPtr )
			, DataSize( static_cast< uint32 >( static_cast< const byte* >( endPtr ) - static_cast< const byte* >( startPtr ) ) )
		{
		}

		constexpr ResourceView( const ResourceView& ) = default;

		template< class _Ty >
		constexpr static ResourceView OfItem( const _Ty& item )
		{
			return ResourceView( std::addressof(item), sizeof( _Ty ) );
		}

		constexpr bool Empty() const
		{
			return DataPtr == NullPtr || DataSize == 0;
		}

		constexpr operator bool() const { return !Empty(); }

		constexpr uint32 Size() const
		{
			return DataSize;
		}

		constexpr uint32 MaxSize() const { return std::numeric_limits< uint32 >::max(); }

		constexpr CMemPtr Data( uint32 offset = 0 ) const
		{
			return static_cast< const byte* >( DataPtr ) + offset;
		}

		const byte& operator [] ( uint32 offset ) const
		{
			return *static_cast< const byte* >( Data( offset ) );
		}
		

		template< class _Ty >
		const _Ty* Pointer( uint32 offset = 0 ) const
		{
			return static_cast< const _Ty* >( Data( offset ) );
		}

		template< class _Ty >
		const _Ty& As( uint32 offset = 0 ) const
		{
			return *Pointer< _Ty >( offset );
		}

		template< class _Ty >
		JSpan< const _Ty > Span( uint32 offset, uint32 count ) const
		{
			return JSpan< const _Ty >( Pointer< _Ty >( offset ), count );
		}

		ResourceView Range( uint32 offset, uint32 size ) const
		{
			return ResourceView( Data( offset ), size );
		}

		ResourceView Range( uint32 offset ) const
		{
			return ResourceView( Data( offset ), DataSize - offset );
		}


	private:

		CMemPtr DataPtr;

		uint32  DataSize;

	};
}
