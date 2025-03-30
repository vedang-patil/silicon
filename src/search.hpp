#pragma once
#include <vector>
#include <algorithm>
#include <thread>
#include <atomic>
#include <functional>
#include "movegen.hpp"
#include "types.hpp"
#include "utils.hpp"

class Search
{
private:
    Board board;
    std::atomic<bool> isSearching;
    std::thread searchThread;

    void search(std::function<void(const Move&)> callback, int depth);
    int negamax(int depth, int alpha, int beta);
public:
    Search(const Board &board);

    void startSearch(std::function<void(const Move&)> callback);

    void stopSearch();
};

