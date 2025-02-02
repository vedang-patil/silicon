#include "utils.hpp"

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

U64 northFill(U64 square)
{
    U64 result = square;
    result |= result << 8;
    result |= result << 16;
    result |= result << 32;
    return result;
}

U64 southFill(U64 square)
{
    U64 result = square;
    result |= result >> 8;
    result |= result >> 16;
    result |= result >> 32;
    return result;
}

U64 westFill(U64 square)
{
    U64 result = square;
    result |= (result & ~A_FILE) >> 1;
    result |= (result & ~(A_FILE | B_FILE)) >> 2;
    result |= (result & ~(A_FILE | B_FILE | C_FILE | D_FILE)) >> 4;
    return result;
}

U64 eastFill(U64 square)
{
    U64 result = square;
    result |= (result & ~H_FILE) << 1;
    result |= (result & ~(H_FILE | G_FILE)) << 2;
    result |= (result & ~(H_FILE | G_FILE | F_FILE | E_FILE)) << 4;
    return result;
}

U64 northEastFill(U64 square)
{
    U64 result = square;
    result |= (result & ~H_FILE) << 9;
    result |= (result & ~(H_FILE | G_FILE)) << 18;
    result |= (result & ~(H_FILE | G_FILE | F_FILE | E_FILE)) << 36;
    return result;
}

U64 southEastFill(U64 square)
{
    U64 result = square;
    result |= (result & ~H_FILE) >> 7;
    result |= (result & ~(H_FILE | G_FILE)) >> 14;
    result |= (result & ~(H_FILE | G_FILE | F_FILE | E_FILE)) >> 28;
    return result;
}

U64 northWestFill(U64 square)
{
    U64 result = square;
    result |= (result & ~A_FILE) << 7;
    result |= (result & ~(A_FILE | B_FILE)) << 14;
    result |= (result & ~(A_FILE | B_FILE | C_FILE | D_FILE)) << 28;
    return result;
}

U64 southWestFill(U64 square)
{
    U64 result = square;
    result |= (result & ~A_FILE) >> 9;
    result |= (result & ~(A_FILE | B_FILE)) >> 18;
    result |= (result & ~(A_FILE | B_FILE | C_FILE | D_FILE)) >> 36;
    return result;
}