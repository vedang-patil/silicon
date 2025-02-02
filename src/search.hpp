#pragma once
#include <vector>
#include <algorithm>
#include <thread>
#include "movegen.hpp"
#include "types.hpp"
#include "utils.hpp"

class Search
{
private:
    Board board;
    std::thread* searchThread = nullptr;
    std::atomic<bool> shouldSearchStop;
    bool isSearching = false;
    Move bestMove = Move(-1, -1);
    int bestEval;

    int negamax(int depth, int alpha, int beta);
    void search();

public:
    Search(const Board &board);

    void startSearch();

    void stopSearch();

    int getResult();
};

