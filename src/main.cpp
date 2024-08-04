#include <vector>
#include <string>
#include <utility>
#include <chrono>
#include <iostream>
#include "types.hpp"
#include "attacks.hpp"
#include "utils.hpp"
#include "uci.hpp"
#include "movegen.hpp"
#include "board.hpp"

int testMoveGeneration(Board &board, int depth)
{
    if (!depth) return 1;

    std::vector<Move> moves = generateLegalMoves(board);
    if (moves.size() == 0) return 0;

    int numberofPositions = 0;
    for (Move move: moves)
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

std::string numToPromoPiece(int num)
{
    if (num == 1) return "n";
    else if (num == 2) return "b";
    else if (num == 3) return "r";
    else if (num == 4) return "q";

    return "";
}

int main()
{
    precomputeAttacks();
    // UCI().loop();

    for (int i = 1; i <= 6; i++)
    {
        Board board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
        auto start = std::chrono::high_resolution_clock::now();
        int x = testMoveGeneration(board, i);
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration  = stop - start;
        printf("Depth: %d, Positions: %d, Time: %lf\n", i, x, duration.count());
    }

    // Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0");
    // std::vector<Move> moves = generateLegalMoves(board);
    // int total = 0;

    // for (Move move: moves)
    // {
    //     board.makeMove(move);
    //     int x = testMoveGeneration(board, 1);
    //     total += x;
    //     std::cout << numToCoord(move.from) << numToCoord(move.to) << numToPromoPiece(move.promotion) << " " << x << std::endl;
    //     board.undoMove();
    // }

    // std::cout << "Total: " << total << std::endl;
}
