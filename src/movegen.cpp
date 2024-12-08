#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include "utils.hpp"
#include "movegen.hpp"
#include "board.hpp"
#include "bits.hpp"
#include "attacks.hpp"

U64 attacksTo(const Board& board, int square)
{
    U64 pawns = (getPawnAttacks(square, 0) & board.currentState.bitboards[6]) | (getPawnAttacks(square, 1) & board.currentState.bitboards[0]);
    U64 knights = getKnightAttacks(square) & (board.currentState.bitboards[1] | board.currentState.bitboards[7]);
    U64 bishops = getBishopAttacks(square, board.getOccupancyBitboard()) & (board.currentState.bitboards[2] | board.currentState.bitboards[8]);
    U64 rooks = getRookAttacks(square, board.getOccupancyBitboard()) & (board.currentState.bitboards[3] | board.currentState.bitboards[9]);
    U64 queens = getQueenAttacks(square, board.getOccupancyBitboard()) & (board.currentState.bitboards[4] | board.currentState.bitboards[10]);
    U64 kings = getKingAttacks(square) & (board.currentState.bitboards[5] | board.currentState.bitboards[11]);
    return pawns | knights | bishops | rooks | queens | kings;
}

std::vector<Move> generatePseudoLegalMoves(const Board& board)
{
    std::vector<Move> moves;

    U64 currentColourBitboard = board.getOccupancyBitboard(board.currentState.colour);

    while (currentColourBitboard != 0)
    {
        U64 src = popLsb(currentColourBitboard);
        int srcIdx = lsbIdx(src);
        U64 targets = 0;

        if ((src & board.currentState.bitboards[0 + board.currentState.colour * 6]) != 0)
        {
            targets = getPawnAttacks(srcIdx, board.currentState.colour)
                    & (board.getOccupancyBitboard(!board.currentState.colour)
                    | (board.currentState.enPassantSquare != -1 ? (1ull<<board.currentState.enPassantSquare) : 0));

            if (board.currentState.colour == 0)
            {
                U64 push = (src << 8) & (~board.getOccupancyBitboard());
                U64 doublePush = (push << 8) & (~board.getOccupancyBitboard()) & RANK_4;
                targets |= push | doublePush;
            }
            else
            {
                U64 push = (src >> 8) & (~board.getOccupancyBitboard());
                U64 doublePush = (push >> 8) & (~board.getOccupancyBitboard()) & RANK_5;
                targets |= push | doublePush;
            }


            U64 promotionTargets = (RANK_1 | RANK_8) & targets;
            targets &= ~(RANK_1 | RANK_8);
            while (promotionTargets != 0)
            {
                int t = lsbIdx(popLsb(promotionTargets));
                moves.emplace_back(srcIdx, t, 1);
                moves.emplace_back(srcIdx, t, 2);
                moves.emplace_back(srcIdx, t, 3);
                moves.emplace_back(srcIdx, t, 4);
            }
        }
        else if ((src & board.currentState.bitboards[1 + board.currentState.colour * 6]) != 0)
            targets = getKnightAttacks(srcIdx) & ~board.getOccupancyBitboard(board.currentState.colour);
        else if ((src & board.currentState.bitboards[2 + board.currentState.colour * 6]) != 0)
            targets = getBishopAttacks(srcIdx, board.getOccupancyBitboard()) & ~board.getOccupancyBitboard(board.currentState.colour);
        else if ((src & board.currentState.bitboards[3 + board.currentState.colour * 6]) != 0)
            targets = getRookAttacks(srcIdx, board.getOccupancyBitboard()) & ~board.getOccupancyBitboard(board.currentState.colour);
        else if ((src & board.currentState.bitboards[4 + board.currentState.colour * 6]) != 0)
            targets = getQueenAttacks(srcIdx, board.getOccupancyBitboard()) & ~board.getOccupancyBitboard(board.currentState.colour);
        else if ((src & board.currentState.bitboards[5 + board.currentState.colour * 6]) != 0)
            targets = getKingAttacks(srcIdx) & ~board.getOccupancyBitboard(board.currentState.colour);

        while (targets != 0)
            moves.emplace_back(srcIdx, lsbIdx(popLsb(targets)));
    }

    if (board.currentState.colour == 0)
    {
        bool canCastleKingside = ((board.currentState.castlingRights & 1) && (board.getOccupancyBitboard() & 96) == 0);
        bool canCastleQueenside = ((board.currentState.castlingRights & 2) && (board.getOccupancyBitboard() & 14) == 0);

        canCastleKingside = canCastleKingside && (((attacksTo(board, 4) | attacksTo(board, 5)) & board.getOccupancyBitboard(!board.currentState.colour)) == 0);
        canCastleQueenside = canCastleQueenside && (((attacksTo(board, 4) | attacksTo(board, 3)) & board.getOccupancyBitboard(!board.currentState.colour)) == 0);

        if (canCastleKingside)
            moves.emplace_back(4, 6);
        if (canCastleQueenside)
            moves.emplace_back(4, 2);
    }
    else
    {
        bool canCastleKingside = ((board.currentState.castlingRights & 4) && (board.getOccupancyBitboard() & 6917529027641081856ull) == 0);
        bool canCastleQueenside = ((board.currentState.castlingRights & 8) && (board.getOccupancyBitboard() & 1008806316530991104ull) == 0);

        canCastleKingside = canCastleKingside && (((attacksTo(board, 60) | attacksTo(board, 61) | attacksTo(board, 62)) & board.getOccupancyBitboard(!board.currentState.colour)) == 0);
        canCastleQueenside = canCastleQueenside && (((attacksTo(board, 60) | attacksTo(board, 59) | attacksTo(board, 58)) & board.getOccupancyBitboard(!board.currentState.colour)) == 0);

        if (canCastleKingside)
            moves.emplace_back(60, 62);
        if (canCastleQueenside)
            moves.emplace_back(60, 58);
    }

    return moves;
}


std::vector<Move> generateLegalMoves(const Board& board)
{
    std::vector<Move> moves;
    Board b = board;

    for (Move& move: generatePseudoLegalMoves(board))
    {
        b.makeMove(move);
        if ((attacksTo(b, lsbIdx(b.currentState.bitboards[(!b.currentState.colour) * 6 + 5])) & b.getOccupancyBitboard(b.currentState.colour)) == 0)
            moves.push_back(move);
        b.undoMove();
    }

    return moves;
}