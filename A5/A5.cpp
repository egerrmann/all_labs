#include <iostream>

extern "C" {
    int __cdecl Arcsin(float x);
    float __cdecl CalculateSh(float x, float eps);
}

int main()
{
    float x, x1, eps;
    std::cout << "Insert x of arcsin(x): " << std::endl;
    std::cin >> x;
    std::cout << Arcsin(x) << std::endl;
    std::cout << "Insert x and epsilon:" << std::endl;
    std::cin >> x1 >> eps;
    std::cout << CalculateSh(x1, eps);
    return 0;
}
