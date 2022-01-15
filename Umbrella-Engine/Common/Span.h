#pragma once
#include <span>



namespace J
{
	template<class _Ty, size_t _Extent = std::dynamic_extent>
	using Span = std::span<_Ty, _Extent>;
}
