#pragma once
#include <vector>
#include <algorithm>
#include "movegen.hpp"
#include "movegen.hpp"
#include "bits.hpp"
#include "hashing.hpp"

int negamax(Board &board, TranspositionTable<int> &transpositionTable, int depth);

int negamax(Board &board, TranspositionTable<int> &transpositionTable, int depth, int alpha, int beta);