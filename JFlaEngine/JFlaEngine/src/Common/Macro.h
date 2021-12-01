#pragma once
#include <type_traits>
#include "Compiler.h"

#define BIT(n) (1 << (n))

#define UNDERLYING_TYPE(type) std::underlying_type_t<type>

#define DECLARE_ENUM_FLAG_OPERATIONS(enum_name)\
		FORCEINLINE UNDERLYING_TYPE(enum_name) operator & (enum_name lhs, enum_name rhs)\
		{ return static_cast<UNDERLYING_TYPE(enum_name)>(lhs) & static_cast<UNDERLYING_TYPE(enum_name)>(rhs); }\
		FORCEINLINE UNDERLYING_TYPE(enum_name) operator | (enum_name lhs, enum_name rhs)\
		{ return static_cast<UNDERLYING_TYPE(enum_name)>(lhs) | static_cast<UNDERLYING_TYPE(enum_name)>(rhs); }\
		FORCEINLINE UNDERLYING_TYPE(enum_name) operator & (enum_name lhs, UNDERLYING_TYPE(enum_name) rhs)\
		{ return static_cast<UNDERLYING_TYPE(enum_name)>(lhs) | rhs; }\
		FORCEINLINE UNDERLYING_TYPE(enum_name) operator & (UNDERLYING_TYPE(enum_name) lhs, enum_name rhs)\
		{ return lhs | static_cast<UNDERLYING_TYPE(enum_name)>(rhs); }\
		FORCEINLINE UNDERLYING_TYPE(enum_name) operator | (enum_name lhs, UNDERLYING_TYPE(enum_name) rhs)\
		{ return static_cast<UNDERLYING_TYPE(enum_name)>(lhs) | rhs; }\
		FORCEINLINE UNDERLYING_TYPE(enum_name) operator | (UNDERLYING_TYPE(enum_name) lhs, enum_name rhs)\
		{ return lhs | static_cast<UNDERLYING_TYPE(enum_name)>(rhs); }


#define JF_ARRAY_COUNT(arr) sizeof((arr)) / sizeof((arr)[0])

#define JF_ARRAY_COUNT_TYPE(arr, type) sizeof((arr)) / sizeof(type)


#if ENGINE_MSVC_COMPILER
#define FORCEINLINE __forceinline
#elif ENGINE_GCC_COMPILER
#define FORCEINLINE
#else
#define FORCEINLINE
#endif

#define NOEXCEPT noexcept

#define NODISCARD [[nodiscard]]