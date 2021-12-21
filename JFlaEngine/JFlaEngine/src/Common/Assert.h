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
