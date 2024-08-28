#include "mylib/mylib.h"
#include <algorithm>

namespace mylib {

std::string Utility::toUpperCase(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

int Utility::add(int a, int b) {
    return a + b;
}

} // namespace mylib