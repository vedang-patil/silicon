#include <sstream>
#include <string>
#include <iostream>
#include <thread>
#include <iostream>
#include "uci.hpp"
#include "movegen.hpp"
#include "utils.hpp"
#include "search.hpp"

typedef unsigned long long U64;

void UCI::handleCommand(const std::string& command) {
    std::string commandType = command.substr(0, command.find(' '));

    if (commandType == "uci") {
        std::cout << "id name Silicon 2\nid author Vedang Patil\nuciok" << std::endl;
    }
    else if (commandType == "isready") {
        std::cout << "readyok" << std::endl;
    }
    else if (commandType == "position") {
        std::stringstream ss(command);
        position(ss);
    }
    else if (commandType == "go") {
        this->search->startSearch();
    }
    else if (commandType == "stop") {
        this->search->stopSearch();
    }
    else if (commandType == "display") {
        std::cout << board.getAsFenString() << std::endl;
    }
}

void UCI::position(std::stringstream& ss) {
    std::string commandType, posType;
    ss >> commandType >> posType;
    
    if (posType == "startpos") {
        this->board = Board();
    }
    else {
        std::string fen, fenPiece;

        for (int i = 0; i < 6; i++) {
            ss >> fenPiece;
            fen += fenPiece;
        }

        this->board = Board(fen);
    }

    if (ss.peek() != EOF) {
        std::string word;
        ss >> word;
        while (ss >> word) board.makeMove(word);
    }
}
