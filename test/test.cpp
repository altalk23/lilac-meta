#include <function.hpp>
#include <iostream>

int main() {
    using namespace lilac;

    meta::Function<void(int, float)> a{ 2 };
    a(2, 3.0f);
}