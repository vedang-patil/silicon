#include <vector>
#include <string>
#include <sstream>
#include <utility>
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
    currentState.enPassantSquareIdx = (pieces[3] == "-" ? -1 : (pieces[3][1] - '1') * 8 + (pieces[3][0] - 'a'));
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

    if (currentState.enPassantSquareIdx != -1) result << " " << (char)('a' + currentState.enPassantSquareIdx % 8) << (1 + currentState.enPassantSquareIdx / 8);
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

void Board::makeMove(const std::string& move)
{
    this->makeMove(std::make_pair((move[1] - '1') * 8 + (move[0] - 'a'), (move[3] - '1') * 8 + (move[2] - 'a')));
}

void Board::makeMove(const std::pair<int, int>& move)
{
    prevStates.push_back(currentState);

    int fromPiece = -1, toPiece = -1;

    for (int i = 0; i < 12; i++)
    {
        if (currentState.bitboards[i] & (1ull<<move.first)) fromPiece = i;
        else if (currentState.bitboards[i] & (1ull<<move.second)) toPiece = i;
    }
    
    if (fromPiece == 0 && move.second == currentState.enPassantSquareIdx)
    {
        currentState.bitboards[0] &= ~(1ull<<move.first);
        currentState.bitboards[0] |= (1ull<<move.second);
        currentState.bitboards[6] &= ~(1ull<<(move.second - 8));
    }
    else if (fromPiece == 6 && move.second == currentState.enPassantSquareIdx)
    {
        currentState.bitboards[6] &= ~(1ull<<move.first);
        currentState.bitboards[6] |= (1ull<<move.second);
        currentState.bitboards[0] &= ~(1ull<<(move.second + 8));
    }
    else if (fromPiece == 5 && move.first == 4 && (move.second == 2 || move.second == 6))
    {
        currentState.bitboards[5] &= ~(1ull<<move.first);
        currentState.bitboards[5] |= (1ull<<move.second);
        
        if (move.second == 2)
        {
            currentState.bitboards[3] &= ~(1);
            currentState.bitboards[3] |= (8);
        }
        else if (move.second == 6)
        {
            currentState.bitboards[9] &= ~(1ull<<7);
            currentState.bitboards[9] |= (1ull<<5);
        }
    }
    else if (fromPiece == 11 && move.first == 60 && (move.second == 58 || move.second == 62))
    {
        currentState.bitboards[11] &= ~(1ull<<move.first);
        currentState.bitboards[11] |= (1ull<<move.second);

        if (move.second == 58)
        {
            currentState.bitboards[56] &= ~(1);
            currentState.bitboards[59] |= (8);
        }
        else if (move.second == 62)
        {
            currentState.bitboards[63] &= ~(1ull<<7);
            currentState.bitboards[61] |= (1ull<<5);
        }
    }
    else if (fromPiece == 0 && move.second / 8 == 7)
    {
        currentState.bitboards[fromPiece] &= ~(1ull<<move.first);
        if (toPiece != -1) currentState.bitboards[toPiece] &= ~(1ull<<move.second);
        currentState.bitboards[4] |= 1ull<<move.second;
    }
    else if (fromPiece == 6 && move.second / 8 == 0)
    {
        currentState.bitboards[fromPiece] &= ~(1ull<<move.first);
        if (toPiece != -1) currentState.bitboards[toPiece] &= ~(1ull<<move.second);
        currentState.bitboards[10] |= 1ull<<move.second;
    }
    else
    {
        currentState.bitboards[fromPiece] &= ~(1ull<<move.first);
        if (toPiece != -1) currentState.bitboards[toPiece] &= ~(1ull<<move.second);
        currentState.bitboards[fromPiece] |= 1ull<<move.second;
    }

    if (fromPiece == 0 && move.first + 16 == move.second) currentState.enPassantSquareIdx = move.first + 8;
    else if (fromPiece == 6 && move.first - 16 == move.second) currentState.enPassantSquareIdx = move.first - 8;
    else currentState.enPassantSquareIdx = -1;

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