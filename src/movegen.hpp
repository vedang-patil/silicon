#pragma once
#include <bits/stdc++.h>
#include "board.hpp"

using namespace std;

void initRookLookup();

void initBishopLookup();

void generateMoves(const Board& board, vector<pair<long long, long long>>& moves);

long long getPawnMovesBitboard(const Board& board, long long square);

long long getKnightMovesBitboard(const Board& board, long long square);

long long getKingMovesBitboard(const Board& board, long long square);

long long getRookMovesBitboard(const Board& board, long long square);

long long getBishopMovesBitboard(const Board& board, long long square);

long long getQueenMovesBitboard(const Board& board, long long square);