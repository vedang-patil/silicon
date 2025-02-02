#pragma once
#include "types.hpp"

typedef unsigned long long U64;

U64 lsb(U64 x);

U64 popLsb(U64& x);

int popCount(U64 x);

int lsbIdx(U64 x);

U64 northFill(U64 square);

U64 southFill(U64 square);

U64 westFill(U64 square);

U64 eastFill(U64 square);

U64 northEastFill(U64 square);

U64 southEastFill(U64 square);

U64 northWestFill(U64 square);

U64 southWestFill(U64 square);