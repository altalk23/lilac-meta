#include <function.hpp>
#include <x86/optcall.hpp>
#include <x86/membercall.hpp>
#include <iostream>

void test() {
    printf("Test");
}

int main() {
    using namespace lilac;
    using namespace meta::x86;

    meta::Function<void(int, float, float, float), Optcall> { &test }(0, 1.0f, 2.0f, 3.0f);
    meta::Function<void(int, int, float, float), Optcall> { &test }(0, 1, 2.0f, 3.0f);
    meta::Function<void(int, int, int, float), Optcall> { &test }(0, 1, 2, 3.0f);
    meta::Function<void(int, int, int, int), Optcall> { &test }(0, 1, 2, 3);
    
    // std::string h = "hi";
    struct Test {
        int one;
        char two;
    };

    meta::Function<void(float, std::string), Optcall> { &test }(0.0f, "hi");

    Test t = { 1, '0' };

    meta::Function<void(Test*, float, int), Membercall> { &test }(&t, 1.0f, 0x70);
}