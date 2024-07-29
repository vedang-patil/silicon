#pragma once
#include "utils.hpp"

typedef unsigned long long U64;

void precomputeAttacks();

U64 getPawnAttacks(int square, bool colour);

U64 getKnightAttacks(int square);

U64 getKingAttacks(int square);

U64 getBishopAttacks(int square, U64 occupancy);

U64 getRookAttacks(int square, U64 occupancy);

U64 getQueenAttacks(int square, U64 occupancy);