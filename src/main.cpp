#include <bits/stdc++.h>
#include "uci.hpp"
#include "movegen.hpp"
#include "board.hpp"

using namespace std;

int main()
{
    initRookLookup();
    initBishopLookup();

    UCI uci;
    string command = "";

    while (command != "quit")
    {
        getline(cin, command);
        uci.handleCommand(command);
    }
}
