#include <vector>
#include <string>
#include <utility>
#include <chrono>
#include <iostream>
#include "utils.hpp"
#include "uci.hpp"
#include "movegen.hpp"
#include "board.hpp"

int testMoveGeneration(Board &board, int depth)
{
    if (!depth) return 1;

    std::vector<std::pair<int, int>> moves;
    generateMoves(board, moves);

    int numberofPositions = 0;
    for (std::pair<int, int> move: moves)
    {
        board.makeMove(move);
        numberofPositions += testMoveGeneration(board, depth - 1);
        board.undoMove();
    }

    return numberofPositions;
}

std::string numToCoord(int num)
{
    int file = num % 8;
    int rank = num / 8;

    return std::string(1, 'a' + file) + std::to_string(rank + 1);
}

int main()
{
    precomputeAttacks();

    for (int i = 1; i <= 5; i++)
    {
        Board board;
        auto start = std::chrono::high_resolution_clock::now();
        int x = testMoveGeneration(board, i);
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration  = stop - start;
        printf("Depth: %d, Positions: %d, Time: %lf\n", i, x, duration.count());
    }

    // UCI uci;
    // string command = "";

    // while (command != "quit")
    // {
    //     getline(cin, command);
    //     uci.handleCommand(command);
    // }
}
