#pragma once
#include <bits/stdc++.h>
#include "board.hpp"

typedef unsigned long long U64; 

using namespace std;

void initRookLookup();

void initBishopLookup();

void generateMoves(const Board& board, vector<pair<U64, U64>>& moves);

U64 getPawnMovesBitboard(const Board& board, U64 square);

U64 getKnightMovesBitboard(const Board& board, U64 square);

U64 getKingMovesBitboard(const Board& board, U64 square);

U64 getRookMovesBitboard(const Board& board, U64 square);

U64 getBishopMovesBitboard(const Board& board, U64 square);

U64 getQueenMovesBitboard(const Board& board, U64 square);