#pragma once

#include <vector>
#include <list>
#include <map>
#include <array>
#include <span>
#include <utility>



namespace UE
{
	// #TODO MAKE MEMORY POOLS ALLOCATOR PROPAGATION TO STL CONTAINERS

	template< class _Ty, class _Allocator = std::allocator< _Ty > >
	using JVector = std::vector< _Ty, _Allocator >;

	template< class _Ty, class _Allocator = std::allocator< _Ty > >
	using List = std::list< _Ty, _Allocator >;

	template< class _KeyTy, class _ValueTy, class _Predicate = std::less< _KeyTy >, class _Allocator = std::allocator< std::pair< const _KeyTy, _ValueTy> > >
	using HashMap = std::map< _KeyTy, _ValueTy, _Predicate, _Allocator >;

	template< class _Ty, SIZE_T _Count >
	using Array = std::array< _Ty, _Count >;

	template< class _Ty, size_t _Extent = std::dynamic_extent >
	using JSpan = std::span< _Ty, _Extent >;

	template< class _Ty1, class _Ty2 >
	using JPair = std::pair< _Ty1, _Ty2 >;

	template< class _Ty1, class _Ty2 >
	inline auto MakePair( const _Ty1& v1, const _Ty2& v2 ) { return std::make_pair( v1, v2 );}
}
