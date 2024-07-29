#pragma once
#include <vector>
#include <utility>
#include "types.hpp"
#include "board.hpp"

typedef unsigned long long U64; 

U64 attacksTo(const Board& board, int square);

std::vector<Move> generateLegalMoves(const Board& board);