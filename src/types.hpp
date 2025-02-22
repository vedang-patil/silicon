#pragma once
#include <string>

typedef unsigned long long U64;

const U64 A_FILE = 72340172838076673ull;
const U64 B_FILE = (A_FILE << 1);
const U64 C_FILE = (B_FILE << 1);
const U64 D_FILE = (C_FILE << 1);
const U64 E_FILE = (D_FILE << 1);
const U64 F_FILE = (E_FILE << 1);
const U64 G_FILE = (F_FILE << 1);
const U64 H_FILE = (G_FILE << 1);

const U64 RANK_1 = 255ull;
const U64 RANK_2 = (RANK_1 << 8);
const U64 RANK_3 = (RANK_2 << 8);
const U64 RANK_4 = (RANK_3 << 8);
const U64 RANK_5 = (RANK_4 << 8);
const U64 RANK_6 = (RANK_5 << 8);
const U64 RANK_7 = (RANK_6 << 8);
const U64 RANK_8 = (RANK_7 << 8);

struct Move
{
    int from, to, promotion;

    Move(int from, int to)
    {
        this->from = from;
        this->to = to;
        this->promotion = 0;
    }

    Move(int from, int to, int promotion)
    {
        this->from = from;
        this->to = to;
        this->promotion = promotion;
    }

    Move(const std::string& move)
    {
        this->from = (move[1] - '1') * 8 + (move[0] - 'a');
        this->to = (move[3] - '1') * 8 + (move[2] - 'a');

        if (move.size() == 5)
        {
            if (move[4] == 'n') this->promotion = 1;
            else if (move[4] == 'b') this->promotion = 2;
            else if (move[4] == 'r') this->promotion = 3;
            else if (move[4] == 'q') this->promotion = 4;
        }
    }

    std::string toString() const
    {
        std::string result = "";
        result += ('a' + (this->from % 8));
        result += std::to_string(this->from / 8 + 1);
        result += ('a' + (this->to % 8));
        result += std::to_string(this->to / 8 + 1);

        if (this->promotion == 1) result = result + "n";
        else if (this->promotion == 2) result = result + "b";
        else if (this->promotion == 3) result = result + "r";
        else if (this->promotion == 4) result = result + "q";
        return result;
    }
};