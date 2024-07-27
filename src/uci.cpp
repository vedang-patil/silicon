#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include "uci.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include "utils.hpp"

typedef unsigned long long U64;

UCI::UCI()
{
    std::cout << "Silicon 2 by Vedang Patil" << std::endl;
}

void UCI::handleCommand(const std::string& command)
{
    std::vector<std::string> tokens = split_str(command, ' ');

    if (tokens[0] == "uci")
    {
        std::cout << "id name Silicon 2" << std::endl;
        std::cout << "id author Vedang Patil" << std::endl;
        std::cout << "uciok" << std::endl;
    }
    else if (tokens[0] == "isready")
    {
        std::cout << "readyok" << std::endl;
    }
    else if (tokens[0] == "position")
    {
        position(tokens);
    }
    else if (tokens[0] == "display")
    {
        std::cout << board.getAsFenString() << std::endl;
    }
    else if (tokens[0] == "go")
    {
        go(tokens);
    }
}

void UCI::position(const std::vector<std::string>& tokens)
{
    this->board = ((tokens[1] == "startpos") ? Board() : Board(tokens[2] + ' ' + tokens[3] + ' ' + tokens[4] + ' ' + tokens[5] + ' ' + tokens[6] + ' ' + tokens[7]));
    for (size_t i = (tokens[1] == "startpos") ? 3: 8; i < tokens.size(); i++) board.makeMove(tokens[i]);
}

void UCI::go(const std::vector<std::string>& tokens)
{
    std::vector<std::pair<int, int>> moves;
    generateMoves(board, moves);

    srand(time(NULL));
    std::pair<int, int> bestMove = moves[rand() % moves.size()];
    
    std::cout << "bestMove " << (char)('a' + bestMove.first % 8) << (1 + bestMove.first / 8);
    std::cout << (char)('a' + bestMove.second % 8) << (1 + bestMove.second / 8) << std::endl;
}