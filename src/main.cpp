#include <bits/stdc++.h>
#include "uci.hpp"
#include "movegen.hpp"
#include "board.hpp"

typedef unsigned long long U64;

using namespace std;

int main()
{
    initRookLookup();
    initBishopLookup();

    UCI uci(cout);
    string command = "";

    while (command != "quit")
    {
        cin >> command;
        uci.handleCommand(command);
    }
}
