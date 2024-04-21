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
            bitboards[pieceToIndex[c]] |= (1ll << (rank * 8 + file));
            file++;
        }

        rank -= file / 8;
        file %= 8;
    }
}

string Board::getAsFenString() const
{
    char pieces[64] = {};

    for (int i = 0; i < 64; i++)
    {
        if (((1ll << i) & this->bitboards[0]) != 0) pieces[i] = 'P';
        else if (((1ll << i) & this->bitboards[1]) != 0) pieces[i] = 'N';
        else if (((1ll << i) & this->bitboards[2]) != 0) pieces[i] = 'B';
        else if (((1ll << i) & this->bitboards[3]) != 0) pieces[i] = 'R';
        else if (((1ll << i) & this->bitboards[4]) != 0) pieces[i] = 'Q';
        else if (((1ll << i) & this->bitboards[5]) != 0) pieces[i] = 'K';
        else if (((1ll << i) & this->bitboards[6]) != 0) pieces[i] = 'p';
        else if (((1ll << i) & this->bitboards[7]) != 0) pieces[i] = 'n';
        else if (((1ll << i) & this->bitboards[8]) != 0) pieces[i] = 'b';
        else if (((1ll << i) & this->bitboards[9]) != 0) pieces[i] = 'r';
        else if (((1ll << i) & this->bitboards[10]) != 0) pieces[i] = 'q';
        else if (((1ll << i) & this->bitboards[11]) != 0) pieces[i] = 'k';
    }

    stringstream result;
    int rank = 7, file = 0;
    int emptyCount = 0;
    for (int i = 0; i < 64; i++)
    {
        if (pieces[rank * 8 + file] == 0) emptyCount++;
        else
        {
            if (emptyCount > 0)
            {
                result << emptyCount;
                emptyCount = 0;
            }

            result << pieces[rank * 8 + file];
        }

        file++;
        if (file == 8)
        {
            if (emptyCount > 0)
            {
                result << emptyCount;
                emptyCount = 0;
            }

            if (i != 63) result << '/';
        }

        rank -= file / 8;
        file %= 8;
    }

    result << ' ';

    if (this->colour) result << 'b';
    else result << 'w';

    result << ' ';

    if (1 & this->castlingRights) result << 'K';
    if (2 & this->castlingRights) result << 'Q';
    if (4 & this->castlingRights) result << 'k';
    if (8 & this->castlingRights) result << 'q';

    result << ' ';

    if (this->enPassantTargetSquare == -1) result << '-';
    else result << "-";

    result << ' ' << halfmoveClock << ' ' << fullmoveCounter;
    return result.str();
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

void Board::makeMove(pair<long long, long long>& move)
{
    for (int i = 0; i < 12; i++)
    {
        if ((bitboards[i] & move.first) != 0)
        {
            this->bitboards[i] = this->bitboards[i] & (~move.first);
            this->bitboards[i] = this->bitboards[i] | move.second;
            break;
        }
    }

    fullmoveCounter += this->colour;
    this->colour = !(this->colour);
    this->halfmoveClock++;
}

void Board::unmakeMove(pair<long long, long long>& move)
{
    for (int i = 0; i < 12; i++)
    {
        if ((bitboards[i] & move.second) != 0)
        {
            this->bitboards[i] = this->bitboards[i] & (~move.second);
            this->bitboards[i] = this->bitboards[i] | move.first;
            break;
        }
    }

    fullmoveCounter += this->colour;
    this->colour = !(this->colour);
    this->halfmoveClock++;
}