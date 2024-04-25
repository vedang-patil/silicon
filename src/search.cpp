#include <bits/stdc++.h>
#include "board.hpp"
#include "movegen.hpp"
#include "bitboard.hpp"

typedef unsigned long long U64; 

using namespace std;

int staticEval(Board &board)
{
    int result = 0;
    result += popCount(board.currentState.bitboards[0]);
    result += popCount(board.currentState.bitboards[1] * 3);
    result += popCount(board.currentState.bitboards[2] * 3);
    result += popCount(board.currentState.bitboards[3] * 5);
    result += popCount(board.currentState.bitboards[4] * 9);
    result -= popCount(board.currentState.bitboards[6]);
    result -= popCount(board.currentState.bitboards[7] * 3);
    result -= popCount(board.currentState.bitboards[8] * 3);
    result -= popCount(board.currentState.bitboards[9] * 5);
    result -= popCount(board.currentState.bitboards[10] * 9);
    return board.currentState.colour ? -result : result;
}

int negaMax(Board &board, int depth)
{
    if (depth == 0) return staticEval(board);

    vector<pair<U64, U64>> moves;
    generateMoves(board, moves);
    int best = -1e9;

    for (pair<U64, U64>& move: moves)
    {
        board.makeMove(move);
        int score = -negaMax(board, depth - 1);
        board.undoMove();
        if (score > best) best = score;
    }

    return best;
}