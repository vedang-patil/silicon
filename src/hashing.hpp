#pragma once
#include "types.hpp"
#include "board.hpp"

template <typename T>
class TranspositionTable
{
private:
    U64 _size;
    T* _table;

public:
    TranspositionTable(U64 size)
    {
        _size = size;
        _table = new T[_size];
    }

    ~TranspositionTable()
    {
        delete[] _table;
    }

    U64 getSize() const
    {
        return _size;
    }

    T& operator[](U64 zobristHash)
    {
        return _table[zobristHash % _size];
    }
};

void precomputeZobristRandoms();

U64 computeZobristHash(const BoardState &boardState);