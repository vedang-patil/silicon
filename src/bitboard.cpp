#include <bits/stdc++.h>
#include "bitboard.hpp"

typedef unsigned long long U64;

using namespace std;

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
    return popCount(lsb(x));
}

void getSubsets(U64 x, vector<U64>& result)
{
    for (U64 i = 0; i < (1ull<<popCount(x)); i++)
    {
        U64 tempX = x;
        U64 current = 0;
        int j = 0;

        while (tempX != 0)
        {
            if (i & (1 << j)) current |= popLsb(tempX);
            else popLsb(tempX);
            j++;
        }

        result.push_back(current);
    }
}