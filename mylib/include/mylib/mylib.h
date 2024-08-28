#pragma once

#ifdef MYLIB_EXPORTS
    #define MYLIB_API __declspec(dllexport)
#else
    #define MYLIB_API __declspec(dllimport)
#endif

#include <string>

namespace mylib {

class MYLIB_API Utility {
public:
    static std::string toUpperCase(const std::string& input);
    static int add(int a, int b);
};

} // namespace mylib