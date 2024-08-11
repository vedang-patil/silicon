#pragma once
#include <vector>
#include <algorithm>
#include "movegen.hpp"
#include "movegen.hpp"
#include "bits.hpp"

int negamax(Board &board, int depth);

int negamax(Board &board, int depth, int alpha, int beta);