#include "search.hpp"

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
        if (shouldSearchStop.load()) break;
        board.makeMove(move);
        int current = -negamax(depth - 1, -beta, -alpha);
        best = std::max(best, current);
        alpha = std::max(alpha, best);
        board.undoMove();
        if (beta <= alpha) break;
    }

    return best;
}

void Search::search()
{
    bestEval = negamax(5, -1e9, 1e9);
}

void Search::startSearch()
{
    shouldSearchStop.store(false);
    isSearching = true;
    this->searchThread = new std::thread(search);
}

void Search::stopSearch()
{
    shouldSearchStop.store(true);
    isSearching = false;
    this->searchThread->join();
    delete this->searchThread;
}

int Search::getResult()
{
    return this->bestEval;
}