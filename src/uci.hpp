#pragma once
#include <bits/stdc++.h>

using namespace std;

class UCI
{
private:
    ostream& outputStream;
public:
    UCI(ostream& outputStream);

    void handleCommand(const string& command);
    
    void uci();

    void isReady();

    void uciNewGame();

    void position(const vector<string>& tokens);

    void go(const vector<string>& tokens);

    void stop();
};