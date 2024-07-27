#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include "utils.hpp"
#include "movegen.hpp"
#include "board.hpp"
#include "bits.hpp"

typedef unsigned long long U64;

U64 attacks[8][256];

void precomputeAttacks()
{
    for (U64 i = 0; i < 8; i++)
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

            attacks[i][j] = result;
        }
    }
}

void generateMoves(const Board& board, std::vector<std::pair<int, int>>& moves)
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

        while (targets != 0) moves.emplace_back(lsbIdx(src), lsbIdx(popLsb(targets)));
    }
}

U64 getPawnMovesBitboard(const Board& board, U64 square)
{
    if (board.currentState.colour == 0)
    {
        U64 push = (square << 8) & (~board.getOccupancyBitboard());
        U64 doublePush = (push << 8) & (~board.getOccupancyBitboard()) & RANK_4;

        U64 enPassantSquare = (board.currentState.enPassantSquareIdx != -1 ? (1ull<<board.currentState.enPassantSquareIdx) : 0);
        U64 leftDiagCapture = ((square & (~A_FILE)) << 7) & (board.getOccupancyBitboard(!board.currentState.colour) | enPassantSquare);
        U64 rightDiagCapture = ((square & (~H_FILE)) << 9) & (board.getOccupancyBitboard(!board.currentState.colour) | enPassantSquare);

        return push | doublePush | leftDiagCapture | rightDiagCapture;
    }
    else
    {
        U64 push = (square >> 8) & (~board.getOccupancyBitboard());
        U64 doublePush = (push >> 8) & (~board.getOccupancyBitboard()) & RANK_5;

        U64 enPassantSquare = (board.currentState.enPassantSquareIdx != -1 ? (1ull<<board.currentState.enPassantSquareIdx) : 0);
        U64 leftDiagCapture = ((square & (~A_FILE)) >> 9) & (board.getOccupancyBitboard(!board.currentState.colour) | enPassantSquare);
        U64 rightDiagCapture = ((square & (~H_FILE)) >> 7) & (board.getOccupancyBitboard(!board.currentState.colour) | enPassantSquare);

        return push | doublePush | leftDiagCapture | rightDiagCapture;
    }
}

U64 getKnightMovesBitboard(const Board& board, U64 square)
{
    U64 noNoEa = ((~(RANK_7 | RANK_8 | H_FILE) & square) << 17);
    U64 noEaEa = ((~(RANK_8 | G_FILE | H_FILE) & square) << 10);
    U64 soEaEa = ((~(RANK_1 | G_FILE | H_FILE) & square) >> 6);
    U64 soSoEa = ((~(RANK_1 | RANK_2 | H_FILE) & square) >> 15);
    U64 soSoWe = ((~(RANK_1 | RANK_2 | A_FILE) & square) >> 17);
    U64 soWeWe = ((~(RANK_1 | A_FILE | B_FILE) & square) >> 10);
    U64 noWeWe = ((~(RANK_8 | A_FILE | B_FILE) & square) << 6);
    U64 noNoWe = ((~(RANK_8 | RANK_7 | A_FILE) & square) << 15);

    return (noNoEa | noEaEa | soEaEa | soSoEa | soSoWe | soWeWe | noWeWe | noNoWe) & (~board.getOccupancyBitboard(board.currentState.colour));
}

U64 getKingMovesBitboard(const Board& board, U64 square)
{
    U64 leftSquare = (square & (~A_FILE)) >> 1;
    U64 rightSquare = (square & (~H_FILE)) << 1;

    U64 up = ((leftSquare | square | rightSquare) << 8);
    U64 down = ((leftSquare | square | rightSquare) >> 8);

    U64 result = (leftSquare | rightSquare | up | down) & (~board.getOccupancyBitboard(board.currentState.colour));

    if (board.currentState.colour == 0)
    {
        if ((board.currentState.castlingRights | 1) && (board.getOccupancyBitboard() & 96) == 0) result |= 64;
        if ((board.currentState.castlingRights | 2) && (board.getOccupancyBitboard() & 14) == 0) result |= 4;
    }
    else
    {
        if ((board.currentState.castlingRights | 4) && (board.getOccupancyBitboard() & 6917529027641081856ull) == 0)
            result |= 4611686018427387904ull;
        if ((board.currentState.castlingRights | 8) && (board.getOccupancyBitboard() & 1008806316530991104ull) == 0)
            result |= 288230376151711744ull;
    }

    return result;
}

U64 getRankMovesBitboard(const Board& board, U64 square)
{
    U64 rank = eastFill(square) | westFill(square);
    U64 result = rank & attacks[lsbIdx(square) % 8][((rank & board.getOccupancyBitboard()) * A_FILE) >> 56];
    return result & ~board.getOccupancyBitboard(board.currentState.colour);
}

U64 getFileMovesBitboard(const Board& board, U64 square)
{
    U64 mainDiag = 9241421688590303745ull;
    U64 result = attacks[7 - lsbIdx(square) / 8][(((board.getOccupancyBitboard() >> (lsbIdx(square) % 8)) & A_FILE) * mainDiag) >> 56];
    result = ((((result & RANK_1) * mainDiag) >> 7) & A_FILE) << (lsbIdx(square) % 8);
    return result & ~board.getOccupancyBitboard(board.currentState.colour);
}

U64 getDiagMovesBitboard(const Board& board, U64 square)
{
    U64 diag = northEastFill(square) | southWestFill(square);
    U64 result = diag & attacks[lsbIdx(square) % 8][((diag & board.getOccupancyBitboard()) * A_FILE) >> 56];
    return result & ~board.getOccupancyBitboard(board.currentState.colour);
}

U64 getAntiDiagMovesBitboard(const Board& board, U64 square)
{
    U64 antiDiag = northWestFill(square) | southEastFill(square);
    U64 result = antiDiag & attacks[lsbIdx(square) % 8][((antiDiag & board.getOccupancyBitboard()) * A_FILE) >> 56];
    return result & ~board.getOccupancyBitboard(board.currentState.colour);
}

U64 getRookMovesBitboard(const Board& board, U64 square)
{
    return getRankMovesBitboard(board, square) | getFileMovesBitboard(board, square);
}

U64 getBishopMovesBitboard(const Board& board, U64 square)
{
    return getDiagMovesBitboard(board, square) | getAntiDiagMovesBitboard(board, square);
}

U64 getQueenMovesBitboard(const Board& board, U64 square)
{
    return getRookMovesBitboard(board, square) | getBishopMovesBitboard(board, square);
}