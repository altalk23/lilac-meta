#include <function.hpp>
#include <windows_x86/optcall.hpp>
#include <iostream>

int main() {
    using namespace lilac;
    using namespace meta::WindowsX86;

    meta::Function<void(int, float), Optcall> a{ 2 };
    a(2, 3.0f);
}