#include <bits/stdc++.h>
#include "uci.hpp"
#include "movegen.hpp"
#include "board.hpp"

using namespace std;

int testMoveGeneration(Board &board, int depth)
{
    if (!depth) return 1;

    vector<pair<int, int>> moves;
    generateMoves(board, moves);

    int numberofPositions = 0;
    for (pair<int, int> move: moves)
    {
        board.makeMove(move);
        numberofPositions += testMoveGeneration(board, depth - 1);
        board.undoMove();
    }

    return numberofPositions;
}

int main()
{
    initRookLookup();
    initBishopLookup();

    // for (int i = 1; i <= 10; i++)
    // {
    //     Board board;
    //     auto start = chrono::high_resolution_clock::now();
    //     int x = testMoveGeneration(board, i);
    //     auto stop = chrono::high_resolution_clock::now();
    //     chrono::duration<double> duration  = stop - start;
    //     printf("Depth: %d, Positions: %d, Time: %lf\n", i, x, duration.count());
    // }

    UCI uci;
    string command = "";

    while (command != "quit")
    {
        getline(cin, command);
        uci.handleCommand(command);
    }
}
