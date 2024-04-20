#include <bits/stdc++.h>
#include "uci.hpp"
#include "movegen.hpp"

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