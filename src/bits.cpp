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
    return std::popcount(x);
}

int lsbIdx(U64 x)
{
   return std::countr_zero(x);
}