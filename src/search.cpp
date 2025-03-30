#include "search.hpp"
#include "iostream"

int staticAnalysis(const Board &board)
{
    int score = 0;

    score += popCount(board.currentState.bitboards[0]) * 100;
    score += popCount(board.currentState.bitboards[1]) * 300;
    score += popCount(board.currentState.bitboards[2]) * 300;
    score += popCount(board.currentState.bitboards[3]) * 500;
    score += popCount(board.currentState.bitboards[4]) * 900;

    score -= popCount(board.currentState.bitboards[6]) * 100;
    score -= popCount(board.currentState.bitboards[7]) * 300;
    score -= popCount(board.currentState.bitboards[8]) * 300;
    score -= popCount(board.currentState.bitboards[9]) * 500;
    score -= popCount(board.currentState.bitboards[10]) * 900;

    return (board.currentState.colour ? -score : score);
}

Search::Search(const Board &board)
{
    this->board = board;
    this->isSearching.store(false);
}

void Search::startSearch(std::function<void(const Move&)> callback)
{
    isSearching.store(true);
    this->searchThread = std::thread(&Search::search, this, callback);
    this->searchThread.detach();;
}

void Search::stopSearch()
{
    isSearching.store(false);
}

void Search::search(std::function<void(const Move&)> callback, int depth)
{
    std::vector<Move> moves = generateLegalMoves(board);
    if (moves.size() == 0)
    {
        callback(Move(-1, -1, -1));
        return;
    }

    int bestEval = -1e9;
    Move bestMove(-1, -1, -1);

    for (Move &move: moves)
    {
        if (!isSearching.load()) break;
        int current = negamax(depth - 1, -1e9, 1e9);
        if (current > bestEval)
        {
            bestEval = current;
            bestMove = move;
        }
    }

    callback(bestMove);
}

int Search::negamax(int depth, int alpha, int beta)
{
    if (depth == 0) return staticAnalysis(board);

    std::vector<Move> moves = generateLegalMoves(board);

    if (moves.size() == 0)
    {
        if ((board.getOccupancyBitboard(~board.currentState.colour) & attacksTo(board, board.currentState.bitboards[board.currentState.colour * 6 + 5])) != 0)
            return -1e9;
        return 0;
    }

    int best = -1e9;

    for (Move &move: moves)
    {
        if (!isSearching.load()) break;

        board.makeMove(move);
        int current = -negamax(depth - 1, -beta, -alpha);
        board.undoMove();

        best = std::max(best, current);
        alpha = std::max(alpha, best);
        if (beta <= alpha) break;
    }

    return best;
}