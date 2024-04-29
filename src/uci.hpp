#pragma once
#include <bits/stdc++.h>
#include "board.hpp"

using namespace std;

class UCI
{
private:
    Board board;
public:
    UCI();

    void handleCommand(const string& command);
    
    void uci();

    void isReady();

    void uciNewGame();

    void position(const vector<string>& tokens);

    void go(const vector<string>& tokens);

    void stop();

    void display();
};