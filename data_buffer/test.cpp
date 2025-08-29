#include "data_buffer.hpp"

int main(void)
{
    DataBuffer d;

    int a = 10;
    d << a;

    int b;
    d >> b;
    std::cout << b;
}