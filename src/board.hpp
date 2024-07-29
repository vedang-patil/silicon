#pragma once
#include <vector>
#include <string>
#include <utility>
#include "types.hpp"

struct BoardState
{
    U64 bitboards[12] = {};
    bool colour = false;
    int enPassantSquare = -1;
    int castlingRights = 0;
    int halfmoveClock = 0;
    int fullmoveCounter = 0;
};

class Board
{
public:
    std::vector<BoardState> prevStates;
    BoardState currentState;

    Board();
    Board(const std::string &fenString);

    std::string getAsFenString() const;

    U64 getOccupancyBitboard() const;
    
    U64 getOccupancyBitboard(bool colour) const;

    void makeMove(const std::string& move);

    void makeMove(const Move& move);
    
    void undoMove();
};