#include "debug_log.hpp"

#include <iostream>

void debugLogging(const char* msg) {
#ifdef _DEBUG
    std::cout << msg << std::endl;
#endif
}