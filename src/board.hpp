#pragma once
#include <bits/stdc++.h>

using namespace std;

struct BoardState
{
    long long bitboards[12];
    bool colour;
    long long enPassantTargetSquare;
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

    long long getOccupancyBitboard() const;
    
    long long getOccupancyBitboard(bool colour) const;

    void makeMove(pair<long long, long long>& move);
    
    void undoMove();
};