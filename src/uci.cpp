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
    else if (tokens[0] == "display")
    {
        display();
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
    Board board;
}

void UCI::position(const vector<string>& tokens)
{
    this->board = ((tokens[1] == "startpos") ? Board() : Board(tokens[3] + tokens[4] + tokens[5] + tokens[6] + tokens[6] + tokens[8]));
    for (size_t i = (tokens[1] == "startpos") ? 3: 9; i < tokens.size(); i++) board.makeMove(tokens[i]);
}

void UCI::go(const vector<string>& tokens)
{
}

void UCI::stop()
{
}

void UCI::display()
{
    outputStream << board.getAsFenString() << endl;
}