#pragma once
#include <bits/stdc++.h>
#include "board.hpp"

using namespace std;

char indexToPiece[12] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};
unordered_map<char, int> pieceToIndex
{
    {'P', 0},
    {'N', 1},
    {'B', 2},
    {'R', 3},
    {'Q', 4},
    {'K', 5},
    {'p', 6},
    {'n', 7},
    {'b', 8},
    {'r', 9},
    {'q', 10},
    {'k', 11}
};