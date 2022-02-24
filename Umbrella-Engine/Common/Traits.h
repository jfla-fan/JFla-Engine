#pragma once

#include "Common.h"
#include "BaseTypes.h"
#include <type_traits>
#include <concepts>



namespace UE
{

	template< typename _Range, typename _Ty >
	concept FwdIterableSizedRange = requires ()
	{
		std::ranges::forward_range<_Range> &&
		std::is_same_v< typename std::iterator_traits< std::ranges::iterator_t< _Range > >::value_type, _Ty > &&
		std::ranges::sized_range< _Range >;
	};

	/*template< typename _Range >
	concept FwdIterableSizedRange = requires()
	{
		std::ranges::forward_range< _Range > &&
		std::ranges::sized_range< _Range >;
	};*/

}