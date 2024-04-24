#include <bits/stdc++.h>
#include "movegen.hpp"
#include "board.hpp"
#include "bitboard.hpp"

typedef unsigned long long U64;

using namespace std;

map<pair<U64, U64>, U64> rookAttacks;
map<pair<U64, U64>, U64> bishopAttacks;

void initRookLookup()
{
    for (U64 src = 1; src != 0; src <<= 1)
    {
        U64 unblockedOccupancy = 0;
        U64 north = src, south = src, east = src, west = src;

        for (int i = 0; i < 7; i++)
        {
            north = (north & ~RANK_7) << 8;
            south = (south & ~RANK_2) >> 8;
            east = (east & ~H_FILE & ~G_FILE) << 1;
            west = (west & ~A_FILE & ~B_FILE) >> 1;
            unblockedOccupancy |= (north | south | east | west);
        }

        vector<U64> possibleOccupancies;
        getSubsets(unblockedOccupancy, possibleOccupancies);

        for (U64 occupancy: possibleOccupancies)
        {
            U64 a = src, b = src, c = src, d = src;
            U64 result = 0;

            for (int i = 0; i < 7; i++)
            {
                a = (a & (~occupancy)) >> 8;
                b = (b & (~occupancy)) << 8;
                c = (c & (~H_FILE) & (~occupancy)) << 1;
                d = (d & (~A_FILE) & (~occupancy)) >> 1;

                result |= (a | b | c | d);
            }

            rookAttacks[{src, occupancy}] = result;
        }
    }
}

void initBishopLookup()
{
    for (U64 src = 1; src != 0; src <<= 1)
    {
        U64 unblockedOccupancy = 0;
        U64 northEast = src, southEast = src, northWest = src, southWest = src;
        for (int i = 0; i < 7; i++)
        {
            northWest = (northWest & (~(A_FILE | B_FILE | RANK_8 | RANK_7))) << 7;
            northEast = (northEast & (~(RANK_8 | RANK_7 | H_FILE | G_FILE))) << 9;
            southEast = (southEast & (~(H_FILE | G_FILE | RANK_1 | RANK_2))) >> 7;
            southWest = (southWest & (~(RANK_1 | RANK_2 | A_FILE | B_FILE))) >> 9;
            unblockedOccupancy |= (northWest | northEast | southEast | southWest);
        }

        vector<U64> possibleOccupancies;
        getSubsets(unblockedOccupancy, possibleOccupancies);

        for (U64 occupancy: possibleOccupancies)
        {
            U64 a = src, b = src, c = src, d = src;
            U64 result = 0;

            for (int i = 0; i < 7; i++)
            {
                a = (a & (~(A_FILE | RANK_8)) & (~occupancy)) << 7;
                b = (b & (~(RANK_8 | H_FILE)) & (~occupancy)) << 9;
                c = (c & (~(H_FILE | RANK_1)) & (~occupancy)) >> 7;
                d = (d & (~(RANK_1 | A_FILE)) & (~occupancy)) >> 9;

                result |= (a | b | c | d);
            }

            bishopAttacks[{src, occupancy}] = result;
        }
    }
}

void generateMoves(const Board& board, vector<pair<U64, U64>>& moves)
{
    U64 currentColourBitboard = board.getOccupancyBitboard(board.currentState.colour);

    while (currentColourBitboard != 0)
    {
        U64 src = popLsb(currentColourBitboard);
        U64 targets = 0;

        if ((src & board.currentState.bitboards[0 + board.currentState.colour * 6]) != 0) targets = getPawnMovesBitboard(board, src);
        else if ((src & board.currentState.bitboards[1 + board.currentState.colour * 6]) != 0) targets = getKnightMovesBitboard(board, src);
        else if ((src & board.currentState.bitboards[2 + board.currentState.colour * 6]) != 0) targets = getBishopMovesBitboard(board, src);
        else if ((src & board.currentState.bitboards[3 + board.currentState.colour * 6]) != 0) targets = getRookMovesBitboard(board, src);
        else if ((src & board.currentState.bitboards[4 + board.currentState.colour * 6]) != 0) targets = getQueenMovesBitboard(board, src);
        else if ((src & board.currentState.bitboards[5 + board.currentState.colour * 6]) != 0) targets = getKingMovesBitboard(board, src);

        while (targets != 0) moves.emplace_back(src, popLsb(targets));
    }
}

U64 getPawnMovesBitboard(const Board& board, U64 square)
{
    if (board.currentState.colour == 0)
    {
        U64 push = (square << 8) & (~board.getOccupancyBitboard());
        U64 doublePush = (push << 8) & (~board.getOccupancyBitboard()) & RANK_4;
        U64 leftDiagCapture = ((square & (~A_FILE)) << 7) & board.getOccupancyBitboard(!board.currentState.colour);
        U64 rightDiagCapture = ((square & (~H_FILE)) << 9) & board.getOccupancyBitboard(!board.currentState.colour);

        bool canEnPassantCapture = ((board.currentState.enPassantTargetSquare & (~A_FILE)) << 1) == square;
        canEnPassantCapture |= ((board.currentState.enPassantTargetSquare & (~H_FILE)) >> 1) == square;
        U64 enPassantCapture = (canEnPassantCapture ? (board.currentState.enPassantTargetSquare << 8) : 0);

        return push | doublePush | leftDiagCapture | rightDiagCapture | enPassantCapture;
    }
    else
    {
        U64 push = (square >> 8) & (~board.getOccupancyBitboard());
        U64 doublePush = (push >> 8) & (~board.getOccupancyBitboard()) & RANK_5;
        U64 leftDiagCapture = ((square & (~A_FILE)) >> 9) & board.getOccupancyBitboard(!board.currentState.colour);
        U64 rightDiagCapture = ((square & (~H_FILE)) >> 7) & board.getOccupancyBitboard(!board.currentState.colour);

        bool canEnPassantCapture = ((board.currentState.enPassantTargetSquare & (~A_FILE)) << 1) == square;
        canEnPassantCapture |= ((board.currentState.enPassantTargetSquare & (~H_FILE)) >> 1) == square;
        U64 enPassantCapture = (canEnPassantCapture ? (board.currentState.enPassantTargetSquare >> 8) : 0);

        return push | doublePush | leftDiagCapture | rightDiagCapture | enPassantCapture;
    }
}

U64 getKnightMovesBitboard(const Board& board, U64 square)
{
    U64 noNoEa = ((~(RANK_7 & RANK_8 & H_FILE) & (square)) << 17);
    U64 noEaEa = ((~(RANK_8 & G_FILE & H_FILE) & (square)) << 10);
    U64 soEaEa = ((~(RANK_1 & G_FILE & H_FILE) & (square)) >> 6);
    U64 soSoEa = ((~(RANK_1 & RANK_2 & H_FILE) & (square)) >> 15);
    U64 soSoWe = ((~(RANK_1 & RANK_2 & A_FILE) & (square)) >> 17);
    U64 soWeWe = ((~(RANK_1 & A_FILE & B_FILE) & (square)) >> 10);
    U64 noWeWe = ((~(RANK_8 & A_FILE & B_FILE) & (square)) << 6);
    U64 noNoWe = ((~(RANK_8 & RANK_7 & A_FILE) & (square)) << 15);
    return (noNoEa | noEaEa | soEaEa | soSoEa | soSoWe | soWeWe | noWeWe | noNoWe) & (~board.getOccupancyBitboard(board.currentState.colour));
}

U64 getKingMovesBitboard(const Board& board, U64 square)
{
    U64 leftSquare = (square & (~A_FILE)) >> 1;
    U64 rightSquare = (square & (~H_FILE)) << 1;

    U64 up = ((leftSquare | square | rightSquare) << 8);
    U64 down = ((leftSquare | square | rightSquare) >> 8);

    return (leftSquare | rightSquare | up | down) & (~board.getOccupancyBitboard(board.currentState.colour));
}

U64 getRookMovesBitboard(const Board& board, U64 square)
{
    return rookAttacks[{square, rookAttacks[{square, 0}] & board.getOccupancyBitboard()}] & (~board.getOccupancyBitboard(board.currentState.colour));
}

U64 getBishopMovesBitboard(const Board& board, U64 square)
{
    return bishopAttacks[{square, bishopAttacks[{square, 0}] & board.getOccupancyBitboard()}] & (~board.getOccupancyBitboard(board.currentState.colour));
}

U64 getQueenMovesBitboard(const Board& board, U64 square)
{
    return getRookMovesBitboard(board, square) | getBishopMovesBitboard(board, square);
}