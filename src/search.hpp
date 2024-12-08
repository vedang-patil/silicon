#pragma once
#include <vector>
#include <algorithm>
#include "movegen.hpp"
#include "movegen.hpp"
#include "bits.hpp"

class Search
{
private:
    Board board;
    bool isCancelled = false;

public:
    Search(const Board &board);

    int negamax(int depth, int alpha, int beta);

    void stopSearch();
};

