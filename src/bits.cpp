#include <bit>
#include "bits.hpp"

U64 lsb(U64 x)
{
    return x & (0-x);
}

U64 popLsb(U64& x)
{
    U64 y = lsb(x);
    x &= (~y);
    return y;
}

int popCount(U64 x)
{
    int result = 0;

    while (x != 0)
    {
        popLsb(x);
        result++;
    }

    return result;
}

int lsbIdx(U64 x)
{
   return std::countr_zero(x);
}