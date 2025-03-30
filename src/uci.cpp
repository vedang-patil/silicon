#include <sstream>
#include <string>
#include "uci.hpp"
#include "movegen.hpp"
#include "utils.hpp"
#include "search.hpp"

typedef unsigned long long U64;

UCI::UCI(std::function <void(const std::string&)> outputCallback) {
    this->outputCallback = outputCallback;
    this->search = new Search(this->board);
}

void UCI::handleCommand(const std::string& command) {
    std::string commandType = command.substr(0, command.find(' '));
    std::stringstream ss(command);

    if (commandType == "uci") {
        outputCallback("id name Silicon");
        outputCallback("id author Vedang Patil");
        outputCallback("uciok");
    }
    else if (commandType == "isready") {
        outputCallback("readyok");
    }
    else if (commandType == "position") {
        position(ss);
    }
    else if (commandType == "go") {
        this->search->startSearch(5, [this](const Move& bestMove) {
            outputCallback("bestMove " + bestMove.toString());
        });
    }
    else if (commandType == "stop") {
        this->search->stopSearch();
    }
    else if (commandType == "display") {
        outputCallback(board.getAsFenString());
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

    this->search = new Search(this->board);
}
