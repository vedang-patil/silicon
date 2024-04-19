#include <bits/stdc++.h>
#include "movegen.hpp"
#include "board.hpp"
#include "bitboard.hpp"

using namespace std;

void generateMoves(const Board& board, vector<pair<int, int>>& moves)
{
    long long currentColourBitboard = board.getOccupancyBitboard(board.colour);

    while (currentColourBitboard != 0)
    {
        long long x = popLsb(currentColourBitboard);
    }
}

long long getPawnMovesBitboard(const Board& board, long long square)
{
    if (board.colour == 0)
    {
        long long push = (square << 8) & (~board.getOccupancyBitboard());
        long long leftDiagCapture = ((square & (~72340172838076673)) << 7) & board.getOccupancyBitboard(!board.colour);
        long long rightDiagCapture = ((square & (~9259542123273814144)) << 9) & board.getOccupancyBitboard(!board.colour);
        return push | leftDiagCapture | rightDiagCapture;
    }
    else
    {
        long long push = (square >> 8) & (~board.getOccupancyBitboard());
        long long leftDiagCapture = ((square & (~72340172838076673)) >> 9) & board.getOccupancyBitboard(!board.colour);
        long long rightDiagCapture = ((square & (~9259542123273814144)) >> 7) & board.getOccupancyBitboard(!board.colour);
        return push | leftDiagCapture | rightDiagCapture;
    }
}

long long getKnightMovesBitboard(const Board& board, long long square)
{
    long long noNoEa = ((140185576636287 & (square)) << 17);
    long long noEaEa = ((17802464409370431 & (square)) << 10);
    long long soEaEa = ((4557430888798830336 & (square)) >> 6);
    long long soSoEa = ((9187201950435704832 & (square)) >> 15);
    long long soSoWe = ((18374403900871409664 & (square)) >> 17);
    long long soWeWe = ((18229723555195321344 & (square)) >> 10);
    long long noWeWe = ((71209857637481724 & (square)) << 6);
    long long noNoWe = ((280371153272574 & (square)) << 15);
    return (noNoEa | noEaEa | soEaEa | soSoEa | soSoWe | soWeWe | noWeWe | noNoWe) & (!board.getOccupancyBitboard(board.colour));
}

long long getKingMovesBitboard(const Board& board, long long square)
{
    long long leftSquare = (square & (~72340172838076673));
    long long rightSquare = (square & (~9259542123273814144));

    long long up = ((leftSquare | square | rightSquare) << 8);
    long long down = ((leftSquare | square | rightSquare) >> 8);

    return (leftSquare | rightSquare | up | down) & (!board.getOccupancyBitboard(board.colour));
}