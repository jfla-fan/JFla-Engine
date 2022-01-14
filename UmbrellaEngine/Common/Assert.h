#pragma once
#include <iostream>


#define FATAL_ASSERT(condition, message)			\
			do {									\
				if (!(condition))					\
				{									\
					std::cout << message << '\n';	\
					std::exit(EXIT_FAILURE);		\
				}									\
			} while(0)

#define check(condition) do { if (!(condition)) { throw "check failure"; } } while(0)



#if JF_DEBUG
#define JF_ASSERT(condition, message) FATAL_ASSERT(condition, message)
#else
#define JF_ASSERT(condition, message) do { (void)condition; (void)message; } while ()
#endif

#define JF_ALWAYSENABLED_ASSERT(condition, message) FATAL_ASSERT(condition, message)