#pragma once

#ifdef DEBUG
    #define MLIB_COLOR_RED   "\033[31m"
    #define MLIB_COLOR_RESET "\033[0m"
    #if defined(_WIN32)
        #define LOG_STREAM std::clog
    #else
        #define LOG_STREAM std::cout
    #endif
    #define DEBUG_LOG(obj_ptr, x) LOG_STREAM << MLIB_COLOR_RED << "[DEBUG]obj: " << (obj_ptr) << " | " <<  x << MLIB_COLOR_RESET << std::endl
#else
    #define DEBUG_LOG(obj_ptr, x) void(0)
#endif