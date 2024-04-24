#pragma once
#include <bits/stdc++.h>

typedef unsigned long long U64;

using namespace std;

struct BoardState
{
    U64 bitboards[12];
    bool colour;
    U64 enPassantTargetSquare;
    int castlingRights;
    int halfmoveClock;
    int fullmoveCounter;
};

class Board
{
public:
    vector<BoardState> prevStates;
    BoardState currentState;

    Board();
    Board(const string &fenString);

    string getAsFenString() const;

    U64 getOccupancyBitboard() const;
    
    U64 getOccupancyBitboard(bool colour) const;

    void makeMove(pair<U64, U64>& move);
    
    void undoMove();
};