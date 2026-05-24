#pragma once

#include <iostream>

#ifdef DEBUG
#define MLIB_COLOR_RED   "\033[31m"
#define MLIB_COLOR_RESET "\033[0m"
#define DEBUG_LOG(obj_ptr, x) std::clog << MLIB_COLOR_RED << "[DEBUG]obj: " << (obj_ptr) << " | " <<  x << MLIB_COLOR_RESET << std::endl
#else
#define DEBUG_LOG(obj_ptr, x) void(0)
#endif