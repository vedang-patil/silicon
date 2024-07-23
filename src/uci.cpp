#include <bits/stdc++.h>
#include "uci.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include "utils.hpp"

typedef unsigned long long U64;

using namespace std;

UCI::UCI()
{
    cout << "Silicon 2 by Vedang Patil" << endl;
}

void UCI::handleCommand(const string& command)
{
    vector<string> tokens = split_str(command, ' ');

    if (tokens[0] == "uci") uci();
    else if (tokens[0] == "isready") isReady();
    else if (tokens[0] == "ucinewgame") uciNewGame();
    else if (tokens[0] == "position") position(tokens);
    else if (tokens[0] == "stop") stop();
    else if (tokens[0] == "display") display();
    else if (tokens[0] == "go")
    {
        thread t(&UCI::go, this, tokens);
        t.detach();
    }
}

void UCI::uci()
{
    cout << "id name Silicon 2" << endl;
    cout << "id author Vedang Patil" << endl;
    cout << "uciok" << endl;
}

void UCI::isReady()
{
    cout << "readyok" << endl;
}

void UCI::uciNewGame()
{
    Board board;
}

void UCI::position(const vector<string>& tokens)
{
    this->board = ((tokens[1] == "startpos") ? Board() : Board(tokens[2] + ' ' + tokens[3] + ' ' + tokens[4] + ' ' + tokens[5] + ' ' + tokens[6] + ' ' + tokens[7]));
    for (size_t i = (tokens[1] == "startpos") ? 3: 8; i < tokens.size(); i++) board.makeMove(tokens[i]);
}

void UCI::go(const vector<string>& tokens)
{
    vector<pair<int, int>> moves;
    generateMoves(board, moves);

    srand(time(NULL));
    pair<int, int> bestMove = moves[rand() % moves.size()];
    
    cout << "bestMove " << (char)('a' + bestMove.first % 8) << (1 + bestMove.first / 8);
    cout << (char)('a' + bestMove.second % 8) << (1 + bestMove.second / 8) << endl;
}

void UCI::stop()
{
}

void UCI::display()
{
    cout << board.getAsFenString() << endl;
}