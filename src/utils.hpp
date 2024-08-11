#pragma once
#include <vector>
#include <string>
#include "board.hpp"
#include "types.hpp"
#include "bits.hpp"

typedef unsigned long long U64;

std::vector<std::string> split_str(const std::string &s, const char delimiter);

U64 northFill(U64 square);

U64 southFill(U64 square);

U64 westFill(U64 square);

U64 eastFill(U64 square);

U64 northEastFill(U64 square);

U64 southEastFill(U64 square);

U64 northWestFill(U64 square);

U64 southWestFill(U64 square);