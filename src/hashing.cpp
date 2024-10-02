#include "hashing.hpp"
#include "board.hpp"
#include "bits.hpp"
#include <random>
#include <limits>
#include <iostream>

U64 zobristPieceTable[12][64];
U64 zobristColour;
U64 zobristEpTable[64];
U64 zobristCastlingTable[16];

void precomputeZobristRandoms()
{
    std::mt19937_64 generator;
    std::uniform_int_distribution<U64> dist(0, std::numeric_limits<U64>::max());

    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            zobristPieceTable[i][j] = dist(generator);
        }
    }


    zobristColour = dist(generator);
    for (int i = 0; i < 64; i++) zobristEpTable[i] = dist(generator);
    for (int i = 0; i < 16; i++) zobristCastlingTable[i] = dist(generator);

}

U64 computeZobristHash(const BoardState &boardState)
{
    U64 result = 0;

    for (int i = 0; i < 12; i++)
    {
        U64 pieceOccupancy = boardState.bitboards[i];

        while (pieceOccupancy != 0)
        {
            result ^= zobristPieceTable[i][lsbIdx(pieceOccupancy)];
            popLsb(pieceOccupancy);
        }
    }

    if (boardState.colour) result ^= zobristColour;

    if (boardState.enPassantSquare != -1) result ^= zobristEpTable[boardState.enPassantSquare];
    
    result ^= zobristCastlingTable[boardState.castlingRights];

    return result;
}