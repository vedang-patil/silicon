#include <sstream>
#include <iostream>
#include <bitset>
#include "types.hpp"
#include "board.hpp"
#include "bits.hpp"
#include "utils.hpp"

typedef unsigned long long U64;

Board::Board()
    :Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
{}

Board::Board(const std::string &fenString)
{
    std::vector<std::string> pieces = split_str(fenString, ' ');

    currentState.colour = (pieces[1] == "b");
    currentState.castlingRights = 0;
    currentState.castlingRights |= (pieces[2].find('K') == std::string::npos ? 0 : 1);
    currentState.castlingRights |= (pieces[2].find('Q') == std::string::npos ? 0 : 2);
    currentState.castlingRights |= (pieces[2].find('k') == std::string::npos ? 0 : 4);
    currentState.castlingRights |= (pieces[2].find('q') == std::string::npos ? 0 : 8);
    currentState.enPassantSquare = (pieces[3] == "-" ? -1 : (pieces[3][1] - '1') * 8 + (pieces[3][0] - 'a'));
    currentState.halfmoveClock = stoi(pieces[4]);
    currentState.fullmoveCounter = stoi(pieces[5]);

    int rank = 7, file = 0;
    for (const char& c: pieces[0])
    {
        if (c == '/') continue;

        if (isdigit(c))
        {
            file += (c - '0');
        }
        else
        {
            int pieceIndex = 0;
            if (toupper(c) == 'N') pieceIndex = 1;
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

std::string Board::getAsFenString() const
{
    std::stringstream result;
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

    if (currentState.enPassantSquare != -1) result << " " << (char)('a' + currentState.enPassantSquare % 8) << (1 + currentState.enPassantSquare / 8);
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

void Board::makeMove(const Move& move)
{
    prevStates.push_back(currentState);

    int fromPiece = -1, toPiece = -1;

    for (int i = 0; i < 12; i++)
    {
        if (currentState.bitboards[i] & (1ull<<move.from)) fromPiece = i;
        else if (currentState.bitboards[i] & (1ull<<move.to)) toPiece = i;
    }
    
    if (fromPiece == 0 && move.to == currentState.enPassantSquare)
    {
        currentState.bitboards[0] &= ~(1ull<<move.from);
        currentState.bitboards[0] |= (1ull<<move.to);
        currentState.bitboards[6] &= ~(1ull<<(move.to - 8));
    }
    else if (fromPiece == 6 && move.to == currentState.enPassantSquare)
    {
        currentState.bitboards[6] &= ~(1ull<<move.from);
        currentState.bitboards[6] |= (1ull<<move.to);
        currentState.bitboards[0] &= ~(1ull<<(move.to + 8));
    }
    else if (fromPiece == 5 && move.from == 4 && (move.to == 2 || move.to == 6))
    {
        currentState.bitboards[5] &= ~(1ull<<move.from);
        currentState.bitboards[5] |= (1ull<<move.to);
        
        if (move.to == 2)
        {
            currentState.bitboards[3] &= ~(1);
            currentState.bitboards[3] |= (8);
        }
        else if (move.to == 6)
        {
            currentState.bitboards[3] &= ~(1ull<<7);
            currentState.bitboards[3] |= (1ull<<5);
        }
    }
    else if (fromPiece == 11 && move.from == 60 && (move.to == 58 || move.to == 62))
    {
        currentState.bitboards[11] &= ~(1ull<<move.from);
        currentState.bitboards[11] |= (1ull<<move.to);

        if (move.to == 58)
        {
            currentState.bitboards[9] &= ~(1ull<<56);
            currentState.bitboards[9] |= (1ull<<59);
        }
        else if (move.to == 62)
        {
            currentState.bitboards[9] &= ~(1ull<<63);
            currentState.bitboards[9] |= (1ull<<61);
        }
    }
    else if (fromPiece == 0 && move.to / 8 == 7)
    {
        currentState.bitboards[fromPiece] &= ~(1ull<<move.from);
        if (toPiece != -1) currentState.bitboards[toPiece] &= ~(1ull<<move.to);
        if (move.promotion == 1) currentState.bitboards[1] |= 1ull<<move.to;
        else if (move.promotion == 2) currentState.bitboards[2] |= 1ull<<move.to;
        else if (move.promotion == 3) currentState.bitboards[3] |= 1ull<<move.to;
        else if (move.promotion == 4) currentState.bitboards[4] |= 1ull<<move.to;
    }
    else if (fromPiece == 6 && move.to / 8 == 0)
    {
        currentState.bitboards[fromPiece] &= ~(1ull<<move.from);
        if (toPiece != -1) currentState.bitboards[toPiece] &= ~(1ull<<move.to);
        if (move.promotion == 1) currentState.bitboards[7] |= 1ull<<move.to;
        else if (move.promotion == 2) currentState.bitboards[8] |= 1ull<<move.to;
        else if (move.promotion == 3) currentState.bitboards[9] |= 1ull<<move.to;
        else if (move.promotion == 4) currentState.bitboards[10] |= 1ull<<move.to;
    }
    else
    {
        currentState.bitboards[fromPiece] &= ~(1ull<<move.from);
        if (toPiece != -1) currentState.bitboards[toPiece] &= ~(1ull<<move.to);
        currentState.bitboards[fromPiece] |= 1ull<<move.to;
    }

    if (fromPiece == 0 && move.from + 16 == move.to) currentState.enPassantSquare = move.from + 8;
    else if (fromPiece == 6 && move.from - 16 == move.to) currentState.enPassantSquare = move.from - 8;
    else currentState.enPassantSquare = -1;

    if (fromPiece == 5) currentState.castlingRights &= ~3;
    else if (fromPiece == 3 && move.from == 0) currentState.castlingRights &= ~2;
    else if (fromPiece == 3 && move.from == 7) currentState.castlingRights &= ~1;
    else if (fromPiece == 11) currentState.castlingRights &= ~12;
    else if (fromPiece == 9 && move.from == 56) currentState.castlingRights &= ~8;
    else if (fromPiece == 9 && move.from == 63) currentState.castlingRights &= ~4;
    else if (move.to == 0) currentState.castlingRights &= ~2;
    else if (move.to == 7) currentState.castlingRights &= ~1;
    else if (move.to == 56) currentState.castlingRights &= ~8;
    else if (move.to == 63) currentState.castlingRights &= ~4;

    currentState.fullmoveCounter += currentState.colour;
    currentState.colour = !(currentState.colour);
    currentState.halfmoveClock++;

    if (fromPiece == 0 || fromPiece == 6 || toPiece != -1) currentState.halfmoveClock = 0;
}

void Board::undoMove()
{
    currentState = prevStates[prevStates.size() - 1];
    prevStates.pop_back();
}