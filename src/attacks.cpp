#include "attacks.hpp"

U64 slidingAttacks[8][256];

U64 pawnAttacks[2][64];
U64 knightAttacks[64];
U64 kingAttacks[64];

void precomputeAttacks()
{
    // Sliding pieces: Kindergarten Bitboards
    for (int i = 0; i < 8; i++)
    {
        for (U64 j = 0; j < 256; j++)
        {
            U64 result = 0;

            U64 left = ((1<<i) & ~A_FILE) >> 1;
            U64 right = ((1<<i) & ~H_FILE) << 1;
            for (int k = 0; k < 7; k++)
            {
                result |= (left | right);
                left = (left & ~j & ~A_FILE) >> 1;
                right = (right & ~j & ~H_FILE) << 1;
            }

            result |= (result << 8);
            result |= (result << 16);
            result |= (result << 32);

            slidingAttacks[i][j] = result;
        }
    }

    // Knights
    for (int i = 0; i < 64; i++)
    {
        U64 square = (1ull<<i);

        U64 noNoEa = ((~(RANK_7 | RANK_8 | H_FILE) & square) << 17);
        U64 noEaEa = ((~(RANK_8 | G_FILE | H_FILE) & square) << 10);
        U64 soEaEa = ((~(RANK_1 | G_FILE | H_FILE) & square) >> 6);
        U64 soSoEa = ((~(RANK_1 | RANK_2 | H_FILE) & square) >> 15);
        U64 soSoWe = ((~(RANK_1 | RANK_2 | A_FILE) & square) >> 17);
        U64 soWeWe = ((~(RANK_1 | A_FILE | B_FILE) & square) >> 10);
        U64 noWeWe = ((~(RANK_8 | A_FILE | B_FILE) & square) << 6);
        U64 noNoWe = ((~(RANK_7 | RANK_8 | A_FILE) & square) << 15);

        knightAttacks[i] = (noNoEa | noEaEa | soEaEa | soSoEa | soSoWe | soWeWe | noWeWe | noNoWe);
    }

    // Kings
    for (int i = 0; i < 64; i++)
    {
        U64 square = (1ull<<i);

        U64 leftSquare = (square & (~A_FILE)) >> 1;
        U64 rightSquare = (square & (~H_FILE)) << 1;
        U64 up = ((leftSquare | square | rightSquare) << 8);
        U64 down = ((leftSquare | square | rightSquare) >> 8);

        kingAttacks[i] = (leftSquare | rightSquare | up | down);
    }

    // Pawns
    for (int i = 0; i < 64; i++)
    {
        U64 square = (1ull<<i);

        pawnAttacks[0][i] = ((square & (~A_FILE)) << 7) | ((square & (~H_FILE)) << 9);
        pawnAttacks[1][i] = ((square & (~A_FILE)) >> 9) | ((square & (~H_FILE)) >> 7);
    }
}

U64 getPawnAttacks(int square, bool colour)
{
    return pawnAttacks[colour][square];
}

U64 getKnightAttacks(int square)
{
    return knightAttacks[square];
}

U64 getKingAttacks(int square)
{
    return kingAttacks[square];
}

U64 getRankAttacks(int square, U64 occupancy)
{
    U64 rank = eastFill(1ull<<square) | westFill(1ull<<square);
    U64 result = rank & slidingAttacks[square % 8][((rank & occupancy) * A_FILE) >> 56];
    return result;
}

U64 getFileAttacks(int square, U64 occupancy)
{
    U64 mainDiag = 9241421688590303745ull;
    U64 result = slidingAttacks[7 - square / 8][(((occupancy >> (square % 8)) & A_FILE) * mainDiag) >> 56];
    result = ((((result & RANK_1) * mainDiag) >> 7) & A_FILE) << (square % 8);
    return result;
}

U64 getDiagAttacks(int square, U64 occupancy)
{
    U64 diag = northEastFill(1ull<<square) | southWestFill(1ull<<square);
    U64 result = diag & slidingAttacks[square % 8][((diag & occupancy) * A_FILE) >> 56];
    return result;
}

U64 getAntiDiagAttacks(int square, U64 occupancy)
{
    U64 antiDiag = northWestFill(1ull<<square) | southEastFill(1ull<<square);
    U64 result = antiDiag & slidingAttacks[square % 8][((antiDiag & occupancy) * A_FILE) >> 56];
    return result;
}

U64 getBishopAttacks(int square, U64 occupancy)
{
    return getDiagAttacks(square, occupancy) | getAntiDiagAttacks(square, occupancy);
}

U64 getRookAttacks(int square, U64 occupancy)
{
    return getRankAttacks(square, occupancy) | getFileAttacks(square, occupancy);
}

U64 getQueenAttacks(int square, U64 occupancy)
{
    return getRookAttacks(square, occupancy) | getBishopAttacks(square, occupancy);
}