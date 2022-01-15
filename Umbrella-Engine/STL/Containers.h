#pragma once
#include <vector>


namespace J
{
	// #TODO MAKE MEMORY POOLS ALLOCATOR PROPAGATION TO STL CONTAINERS

	template<class _Ty, class _Allocator = std::allocator<_Ty>>
	using JVector = std::vector<_Ty, _Allocator>;


}
