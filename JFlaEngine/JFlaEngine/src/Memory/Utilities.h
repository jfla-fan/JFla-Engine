#pragma once
#include "../Core.h"
#include <memory>



namespace Engine::Memory
{

	inline MemPtr Memcpy(CMemPtr Source, MemPtr Dest, SIZE_T Count) { return std::memcpy(Dest, Source, Count); }
	
	inline MemPtr Memmove(CMemPtr Source, MemPtr Dest, SIZE_T Count) { return std::memmove(Dest, Source, Count); }

	inline MemPtr Alloc(SIZE_T BytesCount) { return malloc(BytesCount); }

	template<class _Ty> inline _Ty* Alloc() { return new _Ty(); }

	template<class _Ty> inline _Ty* Alloc(SIZE_T Count) { return new _Ty[Count]; }

}