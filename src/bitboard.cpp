#include <bits/stdc++.h>

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