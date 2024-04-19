#include <bits/stdc++.h>
#include "uci.hpp"

using namespace std;

int main()
{
    UCI uci(cout);
    string command = "";

    while (command != "quit")
    {
        cin >> command;
        uci.handleCommand(command);
    }
}