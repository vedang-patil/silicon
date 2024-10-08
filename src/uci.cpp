#include <string>
#include <iostream>
#include <thread>
#include <iostream>
#include "uci.hpp"
#include "movegen.hpp"
#include "utils.hpp"
#include "search.hpp"

#include "hashing.hpp"

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

        std::cout << computeZobristHash(board.currentState) << std::endl;
    }
    else if (tokens[0] == "display")
    {
        std::cout << board.getAsFenString() << std::endl;
    }
    else if (tokens[0] == "go")
    {
        std::thread t(&UCI::go, this, tokens);
        t.detach();
    }
    else if (tokens[0] == "stop")
    {
    }
    else if (tokens[0] == "depthtest")
    {
        for (int i = 0; i <= stoi(tokens[1]); i++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            int x = testMoveGeneration(board, i);
            auto stop = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration  = stop - start;
            printf("Depth: %d, Positions: %d, Time: %lf\n", i, x, duration.count());
        }
    }
    else if (tokens[0] == "perft")
    {
        std::vector<Move> moves = generateLegalMoves(board);
        long long int total = 0;

        for (Move move: moves)
        {
            board.makeMove(move);
            int x = testMoveGeneration(board, stoi(tokens[1]) - 1);
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

void UCI::position(const std::vector<std::string>& tokens)
{
    this->board = ((tokens[1] == "startpos") ? Board() : Board(tokens[2] + ' ' + tokens[3] + ' ' + tokens[4] + ' ' + tokens[5] + ' ' + tokens[6] + ' ' + tokens[7]));
    for (size_t i = (tokens[1] == "startpos") ? 3: 8; i < tokens.size(); i++) board.makeMove(tokens[i]);
}

void UCI::go(const std::vector<std::string>& tokens)
{
    int depth = 3;
    if (tokens[1] == "depth") depth = stoi(tokens[2]);

    std::vector<Move> moves = generateLegalMoves(board);

    size_t bestMoveIdx = -1;
    int bestMoveEval = -1e9;

    TranspositionTable<int> transpositionTable(0);

    for (size_t moveIdx = 0; moveIdx < moves.size(); moveIdx++)
    {
        board.makeMove(moves[moveIdx]);
        int currentMoveEval = -negamax(board, transpositionTable, depth);

        if (currentMoveEval > bestMoveEval)
        {
            bestMoveEval = currentMoveEval;
            bestMoveIdx = moveIdx;
        }

        board.undoMove();
    }

    if (bestMoveIdx == -1) bestMoveIdx = 0;
    
    std::cout << "bestMove " << (char)('a' + moves[bestMoveIdx].from % 8) << (1 + moves[bestMoveIdx].from / 8);
    std::cout << (char)('a' + moves[bestMoveIdx].to % 8) << (1 + moves[bestMoveIdx].to / 8) << std::endl;
}