#pragma once

#include <algorithm>
#include <numeric>


namespace UE::Algo
{

	// #TODO THIS IS NOT PARALLEL. MAKE HANDY PARALLEL IMPLEMENTATION OF THIS WHEN THREAD POOLS WILL BE READY

	template< class _It, class _Func >
	constexpr void SequentialFor( _It first, _It last, _Func func )
	{
		for ( ; first != last; ++first )
		{
			func( *first );
		}
	}

	template< class _It, class _Diff, class _Func >
	constexpr void SequentialFor( _It first, const _Diff count, _Func func )
	{
		if ( 0 < count )
		{
			do 
			{
				func( *first );
				--count;
				++first;
			} while ( 0 < count );
		}
	}

	template< class _It, class _Func >
	constexpr void ParallelFor( _It first, _It last, _Func func )
	{
		std::for_each( first, last, func );
	}

	template< class _It, class _Diff, class _Func >
	constexpr void ParallelFor( _It start, const _Diff count, _Func func )
	{
		std::for_each_n( start, count, func );
	}


	template< class _It, class _Ty, class _Func >
	constexpr _Ty Accumulate( _It first, _It last, _Ty accumulator, _Func func )
	{
		return std::accumulate( first, last, accumulator, func );
	}
}
