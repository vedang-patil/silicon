#include <bits/stdc++.h>
#include "uci.hpp"

typedef unsigned long long U64;

using namespace std;

UCI::UCI(ostream& outputStream)
    :outputStream(outputStream)
{
    outputStream << "Silicon 2 by Vedang Patil" << endl;
}

void UCI::handleCommand(const string& command)
{
    vector<string> tokens;
    stringstream ss(command);
    string temp;
    while (getline(ss, temp, ' ')) tokens.push_back(temp);

    if (tokens[0] == "uci")
    {
        uci();
    }
    else if (tokens[0] == "isready")
    {
        isReady();
    }
    else if (tokens[0] == "ucinewgame")
    {
        uciNewGame();
    }
    else if (tokens[0] == "position")
    {
        position(tokens);
    }
    else if (tokens[0] == "go")
    {
        go(tokens);
    }
    else if (tokens[0] == "stop")
    {
        stop();
    }
}

void UCI::uci()
{
    outputStream << "id name Silicon 2" << endl;
    outputStream << "id author Vedang Patil" << endl;
    outputStream << "uciok" << endl;
}

void UCI::isReady()
{
    outputStream << "readyok" << endl;
}

void UCI::uciNewGame()
{
}

void UCI::position(const vector<string>& tokens)
{
}

void UCI::go(const vector<string>& tokens)
{
}

void UCI::stop()
{
}