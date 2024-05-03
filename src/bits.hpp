#pragma once
#include <bits/stdc++.h>

#define A_FILE (72340172838076673ull)
#define B_FILE (A_FILE << 1)
#define C_FILE (B_FILE << 1)
#define D_FILE (C_FILE << 1)
#define E_FILE (D_FILE << 1)
#define F_FILE (E_FILE << 1)
#define G_FILE (F_FILE << 1)
#define H_FILE (G_FILE << 1)

#define RANK_1 (255ull)
#define RANK_2 (RANK_1 << 8)
#define RANK_3 (RANK_2 << 8)
#define RANK_4 (RANK_3 << 8)
#define RANK_5 (RANK_4 << 8)
#define RANK_6 (RANK_5 << 8)
#define RANK_7 (RANK_6 << 8)
#define RANK_8 (RANK_7 << 8)

typedef unsigned long long U64;

using namespace std;

U64 lsb(U64 x);

U64 popLsb(U64& x);

int popCount(U64 x);

int lsbIdx(U64 x);

void getSubsets(U64 x, vector<U64>& result);