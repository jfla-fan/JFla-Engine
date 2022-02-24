#pragma once
#include <memory>
#include <ranges>


namespace UE
{

	template< class _Ty > using Ref = std::shared_ptr< _Ty >;

	template< class _Ty, class... Args > 
	constexpr Ref< _Ty > MakeRef( Args&&... args )
	{
		return std::make_shared< _Ty >( std::forward< Args >( args )... );
	}

	template< class _Ty, typename _Deleter = std::default_delete< _Ty > > using Scope = std::unique_ptr< _Ty, _Deleter >;

	template< class _Ty, class... Args >
	constexpr Scope< _Ty > MakeScoped( Args&&... args )
	{
		return std::make_unique< _Ty >( std::forward< Args >( args )... );
	}

	template< class _Ty, typename _Deleter, class... Args >
	constexpr Scope< _Ty, _Deleter > MakeScoped( Args&&... args )
	{
		return Scope< _Ty, _Deleter >( new _Ty( std::forward< Args >( args )... ) );
	}

	template< class _Ty > 
	using WeakRef = std::weak_ptr< _Ty >;

	template< class _Enum >
	constexpr auto to_underlying( _Enum const& value ) { return static_cast< std::underlying_type_t< _Enum > >( value ); }
}