#pragma once
#include <bits/stdc++.h>

using namespace std;

class Board
{
public:
    long long bitboards[12];

    bool colour;
    int enPassantTargetSquare;
    int castlingRights;
    int halfmoveClock;
    int fullmoveCounter;

    Board();
    Board(const string &fenString);

    string getAsFenString() const;

    long long getOccupancyBitboard() const;
    
    long long getOccupancyBitboard(bool colour) const;

    void makeMove(pair<long long, long long>& move);
    
    void unmakeMove(pair<long long, long long>& move);
};