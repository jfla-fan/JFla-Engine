#pragma once
#include <iostream>


#define FATAL_ASSERT(cond, message)					\
			do {									\
				if (!(cond))						\
				{									\
					std::cout << message << '\n';	\
					std::exit(EXIT_FAILURE);					\
				}									\
			} while(0)

