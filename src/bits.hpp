#pragma once
#include <vector>

typedef unsigned long long U64;

U64 lsb(U64 x);

U64 popLsb(U64& x);

int popCount(U64 x);

int lsbIdx(U64 x);

void getSubsets(U64 x, std::vector<U64>& result);