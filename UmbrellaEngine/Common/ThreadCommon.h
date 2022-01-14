#pragma once
#include <mutex>
#include "Macro.h"

#define JF_SCOPED_LOCK(lock) TSpinLock< decltype(lock) > JF_CONCATENATE2( var, __LINE__ ) ( lock )



namespace J
{
	using TMutex = std::mutex;

	template<class... _Mutexes>
	using TSpinLock = std::scoped_lock<_Mutexes...>;

	template<class _Mutex>
	using TUniqueLock = std::unique_lock<_Mutex>;

}


