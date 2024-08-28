#include <iostream>
#include "mylib/mylib.h"

int main() {
    std::string text = "hello, world!";
    std::cout << "Original: " << text << std::endl;
    std::cout << "Uppercase: " << mylib::Utility::toUpperCase(text) << std::endl;

    int a = 5, b = 3;
    std::cout << a << " + " << b << " = " << mylib::Utility::add(a, b) << std::endl;

    return 0;
}