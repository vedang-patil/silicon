#pragma once
#include <vector>
#include <utility>
#include "board.hpp"

typedef unsigned long long U64; 

void precomputeAttacks();

void generateMoves(const Board& board, std::vector<std::pair<int, int>>& moves);

U64 getPawnMovesBitboard(const Board& board, U64 square);

U64 getKnightMovesBitboard(const Board& board, U64 square);

U64 getKingMovesBitboard(const Board& board, U64 square);

U64 getRookMovesBitboard(const Board& board, U64 square);

U64 getBishopMovesBitboard(const Board& board, U64 square);

U64 getQueenMovesBitboard(const Board& board, U64 square);