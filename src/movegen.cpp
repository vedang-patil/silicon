#include <bits/stdc++.h>
#include "movegen.hpp"
#include "board.hpp"
#include "bitboard.hpp"

using namespace std;

#define A_FILE (72340172838076673ll)
#define B_FILE (144680345676153346ll)
#define C_FILE (289360691352306692ll)
#define D_FILE (578721382704613384ll)
#define E_FILE (1157442765409226768ll)
#define F_FILE (2314885530818453536ll)
#define G_FILE (4629771061636907072ll)
#define H_FILE (-9187201950435737472ll)

#define RANK_1 (255ll)
#define RANK_2 (65280ll)
#define RANK_3 (16711680ll)
#define RANK_4 (4278190080ll)
#define RANK_5 (1095216660480ll)
#define RANK_6 (280375465082880ll)
#define RANK_7 (71776119061217280ll)
#define RANK_8 (-72057594037927936ll)

map<pair<long long, long long>, long long> rookAttacks;
map<pair<long long, long long>, long long> bishopAttacks;

void initRookLookup()
{
    for (long long src = 1; src <= (1<<63); src <<= 1)
    {
        long long viewMask = 0;
        long long a = src, b = src, c = src, d = src;

        for (int i = 0; i < 7; i++)
        {
            a = a >> 8;
            b = b << 8;
            c = (c & (~H_FILE)) >> 1;
            d = (d & (~A_FILE)) << 1;
            viewMask |= (a | b | c | d);
        }

        vector<long long> possibleOccupancies;
        getSubsets(viewMask, possibleOccupancies);

        for (long long occupancy: possibleOccupancies)
        {
            long long result = 0;
            long long a = src, b = src, c = src, d = src;

            for (int i = 0; i < 7; i++)
            {
                a = a >> 8;
                b = b << 8;
                c = (c & (~H_FILE)) >> 1;
                d = (d & (~A_FILE)) << 1;
                result |= (a | b | c | d);
                long long stopNow = A_FILE | H_FILE | RANK_1 | RANK_8 | occupancy;
                a &= ~stopNow;
                b &= ~stopNow;
                c &= ~stopNow;
                d &= ~stopNow;
            }

            rookAttacks[{src, occupancy}] = result;
        }
    }
}

void initBishopLookup()
{
    for (long long src = 1; src <= (1<<63); src <<= 1)
    {
        long long viewMask = 0;
        long long a = src, b = src, c = src, d = src;

        for (int i = 0; i < 7; i++)
        {
            a = (a & ~(RANK_8 | H_FILE)) >> 9;
            b = (a & ~(H_FILE | RANK_1)) << 7;
            c = (c & ~(RANK_1 | A_FILE)) << 9;
            d = (d & ~(A_FILE | RANK_8)) >> 7;
            viewMask |= (a | b | c | d);
        }

        vector<long long> possibleOccupancies;
        getSubsets(viewMask, possibleOccupancies);

        for (long long occupancy: possibleOccupancies)
        {
            long long result = 0;
            long long a = src, b = src, c = src, d = src;

            for (int i = 0; i < 7; i++)
            {
                a = (a & ~(RANK_8 | H_FILE)) >> 9;
                b = (a & ~(H_FILE | RANK_1)) << 7;
                c = (c & ~(RANK_1 | A_FILE)) << 9;
                d = (d & ~(A_FILE | RANK_8)) >> 7;
                result |= (a | b | c | d);
                long long stopNow = A_FILE | H_FILE | RANK_1 | RANK_8 | occupancy;
                a &= ~stopNow;
                b &= ~stopNow;
                c &= ~stopNow;
                d &= ~stopNow;
            }

            bishopAttacks[{src, occupancy}] = result;
        }
    }
}

void generateMoves(const Board& board, vector<pair<long long, long long>>& moves)
{
    long long currentColourBitboard = board.getOccupancyBitboard(board.colour);

    while (currentColourBitboard != 0)
    {
        long long src = popLsb(currentColourBitboard);
        long long targets = 0;

        int offset = board.colour ? 6 : 0;
        if ((src & board.bitboards[0 + offset]) != 0) targets |= getPawnMovesBitboard(board, src);
        else if ((src & board.bitboards[1 + offset]) != 0) targets |= getKnightMovesBitboard(board, src);
        else if ((src & board.bitboards[2 + offset]) != 0) targets |= getBishopMovesBitboard(board, src);
        else if ((src & board.bitboards[3 + offset]) != 0) targets |= getRookMovesBitboard(board, src);
        else if ((src & board.bitboards[4 + offset]) != 0) targets |= getQueenMovesBitboard(board, src);
        else if ((src & board.bitboards[5 + offset]) != 0) targets |= getKingMovesBitboard(board, src);

        while (src != 0) moves.emplace_back(src, popLsb(targets));
    }
}

long long getPawnMovesBitboard(const Board& board, long long square)
{
    if (board.colour == 0)
    {
        long long push = (square << 8) & (~board.getOccupancyBitboard());
        long long leftDiagCapture = ((square & (~A_FILE)) << 7) & board.getOccupancyBitboard(!board.colour);
        long long rightDiagCapture = ((square & (~H_FILE)) << 9) & board.getOccupancyBitboard(!board.colour);
        return push | leftDiagCapture | rightDiagCapture;
    }
    else
    {
        long long push = (square >> 8) & (~board.getOccupancyBitboard());
        long long leftDiagCapture = ((square & (~A_FILE)) >> 9) & board.getOccupancyBitboard(!board.colour);
        long long rightDiagCapture = ((square & (~H_FILE)) >> 7) & board.getOccupancyBitboard(!board.colour);
        return push | leftDiagCapture | rightDiagCapture;
    }
}

long long getKnightMovesBitboard(const Board& board, long long square)
{
    long long noNoEa = ((~(RANK_7 & RANK_8 & H_FILE) & (square)) << 17);
    long long noEaEa = ((~(RANK_8 & G_FILE & H_FILE) & (square)) << 10);
    long long soEaEa = ((~(RANK_1 & G_FILE & H_FILE) & (square)) >> 6);
    long long soSoEa = ((~(RANK_1 & RANK_2 & H_FILE) & (square)) >> 15);
    long long soSoWe = ((~(RANK_1 & RANK_2 & A_FILE) & (square)) >> 17);
    long long soWeWe = ((~(RANK_1 & A_FILE & B_FILE) & (square)) >> 10);
    long long noWeWe = ((~(RANK_8 & A_FILE & B_FILE) & (square)) << 6);
    long long noNoWe = ((~(RANK_8 & RANK_7 & A_FILE) & (square)) << 15);
    return (noNoEa | noEaEa | soEaEa | soSoEa | soSoWe | soWeWe | noWeWe | noNoWe) & (!board.getOccupancyBitboard(board.colour));
}

long long getKingMovesBitboard(const Board& board, long long square)
{
    long long leftSquare = (square & (~A_FILE));
    long long rightSquare = (square & (~H_FILE));

    long long up = ((leftSquare | square | rightSquare) << 8);
    long long down = ((leftSquare | square | rightSquare) >> 8);

    return (leftSquare | rightSquare | up | down) & (!board.getOccupancyBitboard(board.colour));
}

long long getRookMovesBitboard(const Board& board, long long square)
{
    long long relevantOccupancyBitboard = rookAttacks[{square, 0}] & board.getOccupancyBitboard();
    return rookAttacks[{square, relevantOccupancyBitboard}] & (~board.getOccupancyBitboard(board.colour));
}

long long getBishopMovesBitboard(const Board& board, long long square)
{
    long long relevantOccupancyBitboard = bishopAttacks[{square, 0}] & board.getOccupancyBitboard();
    return bishopAttacks[{square, relevantOccupancyBitboard}] & (~board.getOccupancyBitboard(board.colour));
}

long long getQueenMovesBitboard(const Board& board, long long square)
{
    return getRookMovesBitboard(board, square) | getBishopMovesBitboard(board, square);
}