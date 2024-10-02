#include "types.hpp"
#include "board.hpp"

void precomputeZobristRandoms();

U64 computeZobristHash(const BoardState &boardState);