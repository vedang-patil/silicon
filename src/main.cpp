#include <bits/stdc++.h>
#include "uci.hpp"
#include "movegen.hpp"
#include "board.hpp"

using namespace std;

int main()
{
    initRookLookup();
    initBishopLookup();

    Board board;
    vector<pair<long long, long long>> moves;
    generateMoves(board, moves);

    for (pair<long long, long long>& move: moves)
    {
        board.makeMove(move);
        cout << board.getAsFenString() << endl;
        board.undoMove();
    }

    // UCI uci(cout);
    // string command = "";

    // while (command != "quit")
    // {
    //     cin >> command;
    //     uci.handleCommand(command);
    // }
}