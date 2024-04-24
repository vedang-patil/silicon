#include <bits/stdc++.h>
#include "board.hpp"
#include "bitboard.hpp"

typedef unsigned long long U64;

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

    currentState.colour = (sideToMove == 'b');
    currentState.castlingRights = 0;
    currentState.castlingRights |= (castlingRights.find('K') == string::npos ? 0 : 1);
    currentState.castlingRights |= (castlingRights.find('Q') == string::npos ? 0 : 2);
    currentState.castlingRights |= (castlingRights.find('k') == string::npos ? 0 : 4);
    currentState.castlingRights |= (castlingRights.find('q') == string::npos ? 0 : 8);
    currentState.enPassantTargetSquare = (enPassantTargetSquare == "-" ? 0 : (enPassantTargetSquare[1] - 1) * 8 + (enPassantTargetSquare[0] - 'a'));
    currentState.halfmoveClock = halfmoveClock;
    currentState.fullmoveCounter = fullmoveCounter;
    fill(currentState.bitboards, currentState.bitboards + 12, 0);

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
            int pieceIndex;
            if (toupper(c) == 'P') pieceIndex = 0;
            else if (toupper(c) == 'N') pieceIndex = 1;
            else if (toupper(c) == 'B') pieceIndex = 2;
            else if (toupper(c) == 'R') pieceIndex = 3;
            else if (toupper(c) == 'Q') pieceIndex = 4;
            else if (toupper(c) == 'K') pieceIndex = 5;

            if (tolower(c) == c) pieceIndex += 6;

            currentState.bitboards[pieceIndex] |= (1ull << (rank * 8 + file));
            file++;
        }

        rank -= file / 8;
        file %= 8;
    }
}

string Board::getAsFenString() const
{
    stringstream result;
    int rank = 7, file = 0;
    int emptyCount = 0;
    for (int i = 0; i < 64; i++)
    {
        char piece = 0;

        if (((1ull << (rank * 8 + file)) & currentState.bitboards[0]) != 0) piece = 'P';
        else if (((1ull << (rank * 8 + file)) & currentState.bitboards[1]) != 0) piece = 'N';
        else if (((1ull << (rank * 8 + file)) & currentState.bitboards[2]) != 0) piece = 'B';
        else if (((1ull << (rank * 8 + file)) & currentState.bitboards[3]) != 0) piece = 'R';
        else if (((1ull << (rank * 8 + file)) & currentState.bitboards[4]) != 0) piece = 'Q';
        else if (((1ull << (rank * 8 + file)) & currentState.bitboards[5]) != 0) piece = 'K';
        else if (((1ull << (rank * 8 + file)) & currentState.bitboards[6]) != 0) piece = 'p';
        else if (((1ull << (rank * 8 + file)) & currentState.bitboards[7]) != 0) piece = 'n';
        else if (((1ull << (rank * 8 + file)) & currentState.bitboards[8]) != 0) piece = 'b';
        else if (((1ull << (rank * 8 + file)) & currentState.bitboards[9]) != 0) piece = 'r';
        else if (((1ull << (rank * 8 + file)) & currentState.bitboards[10]) != 0) piece = 'q';
        else if (((1ull << (rank * 8 + file)) & currentState.bitboards[11]) != 0) piece = 'k';

        if (piece == 0) emptyCount++;
        else if (emptyCount == 0) result << piece;
        else
        {
            result << emptyCount << piece;
            emptyCount = 0;
        }

        if (++file == 8)
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

    result << (currentState.colour ? " b " : " w ");

    if (1 & currentState.castlingRights) result << "K";
    if (2 & currentState.castlingRights) result << "Q";
    if (4 & currentState.castlingRights) result << "k";
    if (8 & currentState.castlingRights) result << "q";
    if ((15 & currentState.castlingRights) == 0) result << "-";

    int enPassantSquareIdx = lsbIdx(currentState.enPassantTargetSquare) - 1;
    if (enPassantSquareIdx != -1) result << " " << ('a' + enPassantSquareIdx % 8) << (1 + enPassantSquareIdx / 8);
    else result << " -";

    result << ' ' << currentState.halfmoveClock << ' ' << currentState.fullmoveCounter;
    return result.str();
}

U64 Board::getOccupancyBitboard() const
{
    U64 result = 0;
    for (int i = 0; i < 12; i++) result |= currentState.bitboards[i];
    return result;
}

U64 Board::getOccupancyBitboard(bool colour) const
{
    U64 result = 0;
    if (!colour) for (int i = 0; i < 6; i++) result |= currentState.bitboards[i];
    else for (int i = 6; i < 12; i++) result |= currentState.bitboards[i];
    return result;
}

void Board::makeMove(const string& move)
{
    int fromIndex = (move[1] - '1') * 8 + (move[0] - 'a');
    int toIndex = (move[3] - '1') * 8 + (move[2] - 'a');
    this->makeMove(make_pair(1ull<<fromIndex, 1ull<<toIndex));
}

void Board::makeMove(const pair<U64, U64>& move)
{
    prevStates.push_back(currentState);

    for (int i = 0; i < 12; i++)
    {
        if ((currentState.bitboards[i] & move.first) != 0)
        {
            currentState.bitboards[i] = currentState.bitboards[i] & (~move.first);
            currentState.bitboards[i] = currentState.bitboards[i] | move.second;
            break;
        }
    }

    currentState.fullmoveCounter += currentState.colour;
    currentState.colour = !(currentState.colour);
    currentState.halfmoveClock++;
}

void Board::undoMove()
{
    currentState = prevStates[prevStates.size() - 1];
    prevStates.pop_back();
}