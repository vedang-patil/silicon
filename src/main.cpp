#include <bits/stdc++.h>
#include "uci.hpp"
#include "bitboard.hpp"
#include "board.hpp"
#include "movegen.hpp"

using namespace std;

int main()
{
    initRookLookup();
    initBishopLookup();

    Board board("4k3/8/8/3p4/4P3/8/8/4K3 w - d6 0 2");
    vector<pair<long long, long long>> moves;
    generateMoves(board, moves);

    for (pair<long long, long long> move: moves)
    {
        board.makeMove(move);
        cout << board.getAsFenString() << "\n";
        board.unmakeMove(move);
    }

    // UCI uci(cout);
    // string command = "";

    // while (command != "quit")
    // {
    //     cin >> command;
    //     uci.handleCommand(command);
    // }
}