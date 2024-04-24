#pragma once
#include <bits/stdc++.h>
#include "board.hpp"

typedef unsigned long long U64;

using namespace std;

class UCI
{
private:
    ostream& outputStream;
    Board board;
public:
    UCI(ostream& outputStream);

    void handleCommand(const string& command);
    
    void uci();

    void isReady();

    void uciNewGame();

    void position(const vector<string>& tokens);

    void go(const vector<string>& tokens);

    void stop();

    void display();
};