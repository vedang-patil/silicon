#include <string>
#include <iostream>
#include <thread>
#include <iostream>
#include "uci.hpp"
#include "movegen.hpp"
#include "utils.hpp"

typedef unsigned long long U64;

void UCI::loop()
{
    std::cout << "Silicon 2 by Vedang Patil" << std::endl;

    std::string command;
    while (command != "quit")
    {
        getline(std::cin, command);
        handleCommand(command);
    }
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
    else if (tokens[0] == "stop")
    {
        // Later
    }
}

void UCI::position(const std::vector<std::string>& tokens)
{
    this->board = ((tokens[1] == "startpos") ? Board() : Board(tokens[2] + ' ' + tokens[3] + ' ' + tokens[4] + ' ' + tokens[5] + ' ' + tokens[6] + ' ' + tokens[7]));
    for (size_t i = (tokens[1] == "startpos") ? 3: 8; i < tokens.size(); i++) board.makeMove(tokens[i]);
}

void UCI::go(const std::vector<std::string>& tokens)
{
    std::vector<Move> moves = generateLegalMoves(board);

    srand(time(NULL));
    Move bestMove = moves[rand() % moves.size()];
    
    std::cout << "bestMove " << (char)('a' + bestMove.from % 8) << (1 + bestMove.from / 8);
    std::cout << (char)('a' + bestMove.to % 8) << (1 + bestMove.to / 8) << std::endl;
}