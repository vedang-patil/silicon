#include "search.hpp"

int staticAnalysis(const Board &board)
{
    int score = 0;

    score += popCount(board.currentState.bitboards[0]);
    score += popCount(board.currentState.bitboards[1]) * 3;
    score += popCount(board.currentState.bitboards[2]) * 3;
    score += popCount(board.currentState.bitboards[3]) * 5;
    score += popCount(board.currentState.bitboards[4]) * 9;

    score -= popCount(board.currentState.bitboards[6]);
    score -= popCount(board.currentState.bitboards[7]) * 3;
    score -= popCount(board.currentState.bitboards[8]) * 3;
    score -= popCount(board.currentState.bitboards[9]) * 5;
    score -= popCount(board.currentState.bitboards[10]) * 9;

    return (board.currentState.colour ? -score : score);
}

Search::Search(const Board &board)
{
    this->board = board;
}

int Search::negamax(int depth, int alpha, int beta)
{
    if (depth == 0) return staticAnalysis(board);

    std::vector<Move> moves = generateLegalMoves(board);

    if (moves.size() == 0)
    {
        if ((board.getOccupancyBitboard(~board.currentState.colour) & attacksTo(board, board.currentState.bitboards[board.currentState.colour * 6 + 5])) != 0)
            return -1e8;
        return 0;
    }

    int best = -1e9;

    for (Move &move: moves)
    {
        if (isCancelled) break;
        board.makeMove(move);
        int current = -negamax(depth - 1, -beta, -alpha);
        best = std::max(best, current);
        alpha = std::max(alpha, best);
        board.undoMove();
        if (beta <= alpha) break;
    }

    return best;
}

void Search::stopSearch()
{
    isCancelled = true;
}