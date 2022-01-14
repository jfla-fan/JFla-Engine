#pragma once
#include "MemoryUtils.h"
#include <memory>



namespace J::Memory
{



	inline MemPtr Memcpy(CMemPtr Source, MemPtr Dest, SIZE_T Count) { return std::memcpy(Dest, Source, Count); }
	
	inline MemPtr Memmove(CMemPtr Source, MemPtr Dest, SIZE_T Count) { return std::memmove(Dest, Source, Count); }

	inline MemPtr Alloc(SIZE_T BytesCount) { return malloc(BytesCount); }



	template<class _Ty> inline _Ty* Alloc() { return new _Ty(); }

	template<class _Ty, class _Sz = SIZE_T> inline _Ty* Alloc(_Sz Count) { return new _Ty[Count]; }

	template<class _Ty> inline void Free(_Ty* Element) { delete Element; }

	//template<class _Ty> inline void Free(_Ty arr[]) { delete[] arr; }

}