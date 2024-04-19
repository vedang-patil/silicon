#include <bits/stdc++.h>
#include "board.hpp"
#include "constants.hpp"

using namespace std;

Board::Board()
    :Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
{}

Board::Board(const string &fenString)
{
    string piecePlacement;
    char sideToMove;
    string castlingRights;
    string enPassantTargetSquare;
    int halfmoveClock;
    int fullmoveCounter;

    stringstream ss(fenString);
    ss >> piecePlacement >> sideToMove >> castlingRights >> enPassantTargetSquare >> halfmoveClock >> fullmoveCounter;

    this->colour = (sideToMove == 'b');
    this->castlingRights |= (castlingRights.find('K') == string::npos ? 0 : 1);
    this->castlingRights |= (castlingRights.find('Q') == string::npos ? 0 : 2);
    this->castlingRights |= (castlingRights.find('k') == string::npos ? 0 : 4);
    this->castlingRights |= (castlingRights.find('q') == string::npos ? 0 : 8);
    this->enPassantTargetSquare = (enPassantTargetSquare == "-" ? -1 : (enPassantTargetSquare[1] - 1) * 8 + (enPassantTargetSquare[0] - 'a'));
    this->halfmoveClock = halfmoveClock;
    this->fullmoveCounter = fullmoveCounter;
    fill(this->bitboards, this->bitboards + 12, 0);

    int rank = 7, file = 0;
    for (const char& c: piecePlacement)
    {
        if (c == '/') continue;

        if (isdigit(c))
        {
            file += (c - '0');
        }
        else
        {
            bitboards[pieceToIndex[c]] |= (1 << (rank * 8 + file));
            file++;
        }

        rank -= file / 8;
        file %= 8;
    }
}

long long Board::getOccupancyBitboard() const
{
    return bitboards[6] | bitboards[7] | bitboards[8] | bitboards[9] | bitboards[10] | bitboards[11] | 
    bitboards[0] | bitboards[1] | bitboards[2] | bitboards[3] | bitboards[4] | bitboards[5];
}

long long Board::getOccupancyBitboard(bool colour) const
{
    if (colour) return bitboards[6] | bitboards[7] | bitboards[8] | bitboards[9] | bitboards[10] | bitboards[11];
    else return bitboards[0] | bitboards[1] | bitboards[2] | bitboards[3] | bitboards[4] | bitboards[5];
}