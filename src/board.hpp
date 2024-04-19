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

    long long getOccupancyBitboard() const;
    
    long long getOccupancyBitboard(bool colour) const;
};