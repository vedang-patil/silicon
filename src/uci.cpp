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

void UCI::loop()
{
    std::cout << "Silicon 2 by Vedang Patil" << std::endl;

    std::string command;
    while (command != "quit")
    {
        getline(std::cin, command);
        std::stringstream ss(command);

        handleCommand(ss);
    }
}

int testMoveGeneration(Board &board, int depth)
{
    if (!depth) return 1;

    std::vector<Move> moves = generateLegalMoves(board);
    if (moves.size() == 0) return 0;

    int numberofPositions = 0;
    for (Move move: moves)
    {
        board.makeMove(move);
        numberofPositions += testMoveGeneration(board, depth - 1);
        board.undoMove();
    }

    return numberofPositions;
}

void UCI::handleCommand(std::stringstream& ss)
{
    std::string command;
    ss >> command;

    if (command == "uci")
    {
        std::cout << "id name Silicon 2" << std::endl;
        std::cout << "id author Vedang Patil" << std::endl;
        std::cout << "uciok" << std::endl;
    }
    else if (command == "isready")
    {
        std::cout << "readyok" << std::endl;
    }
    else if (command == "position")
    {
        position(ss);
    }
    else if (command == "display")
    {
        std::cout << board.getAsFenString() << std::endl;
    }
    else if (command == "go")
    {
        go(ss);
    }
    else if (command == "stop")
    {
    }
    else if (command == "depthtest")
    {
        std::string depth;
        ss >> depth;
        for (int i = 0; i <= stoi(depth); i++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            int x = testMoveGeneration(board, i);
            auto stop = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration  = stop - start;
            printf("Depth: %d, Positions: %d, Time: %lf\n", i, x, duration.count());
        }
    }
    else if (command == "perft")
    {
        std::string depth;
        ss >> depth;
        std::vector<Move> moves = generateLegalMoves(board);
        long long int total = 0;

        for (Move move: moves)
        {
            board.makeMove(move);
            int x = testMoveGeneration(board, stoi(depth) - 1);
            total += x;
            std::cout << std::string(1, 'a' + (move.from % 8)) + std::to_string((move.from / 8) + 1);
            std::cout << std::string(1, 'a' + (move.to % 8)) + std::to_string((move.to / 8) + 1);
            std::cout << (move.promotion == 1 ? "n" : move.promotion == 2 ? "b" : move.promotion == 3 ? "r" : move.promotion == 4 ? "q" : "");
            std::cout << " " << x << std::endl;
            board.undoMove();
        }

        std::cout << "Total: " << total << std::endl;
    }
}

void UCI::position(std::stringstream& ss)
{
    std::string posType;
    ss >> posType;
    
    if (posType == "startpos")
    {
        this->board = Board();
    }
    else
    {
        std::string fen, fenPiece;

        for (int i = 0; i < 6; i++)
        {
            ss >> fenPiece;
            fen += fenPiece;
        }

        this->board = Board(fen);
    }

    if (ss.peek() != EOF)
    {
        std::string word;
        ss >> word;
        while (ss >> word) board.makeMove(word);
    }
}

void UCI::go(std::stringstream& ss)
{
    std::vector<Move> moves = generateLegalMoves(board);

    size_t bestMoveIdx = 0;
    
    std::cout << "bestMove " << (char)('a' + moves[bestMoveIdx].from % 8) << (1 + moves[bestMoveIdx].from / 8);
    std::cout << (char)('a' + moves[bestMoveIdx].to % 8) << (1 + moves[bestMoveIdx].to / 8) << std::endl;

    std::cout << "info depth 1 seldepth 1 multipv 1 score cp 10 nodes 10 nps 5 time 5 pv ";
    std::cout << (char)('a' + moves[bestMoveIdx].from % 8) << (1 + moves[bestMoveIdx].from / 8);
    std::cout << (char)('a' + moves[bestMoveIdx].to % 8) << (1 + moves[bestMoveIdx].to / 8) << std::endl;
}