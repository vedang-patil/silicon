#include <bits/stdc++.h>
#include "bitboard.hpp"

using namespace std;

long long lsb(long long x)
{
    return x & (-x);
}

long long popLsb(long long& x)
{
    long long y = lsb(x);
    x &= (~y);
    return y;
}

int popCount(long long x)
{
    int result = 0;

    while (x != 0)
    {
        popLsb(x);
        result++;
    }

    return result;
}

void getSubsets(long long x, vector<long long>& result)
{
    for (long long i = 0; i < (1<<popCount(x)); i++)
    {
        long long tempX = x;
        long long current = 0;
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