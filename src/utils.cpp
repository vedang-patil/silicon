#include "utils.hpp"

std::vector<std::string> split_str(const std::string &s, const char delimiter)
{
    size_t start = 0;
    size_t end = s.find_first_of(delimiter);

    std::vector<std::string> result;

    while (end <= std::string::npos)
    {
        result.emplace_back(s.substr(start, end - start));
        if (end == std::string::npos) break;

        start = end + 1;
        end = s.find_first_of(delimiter, start);
    }

    return result;
}

U64 northFill(U64 square)
{
    U64 result = square;
    result |= result << 8;
    result |= result << 16;
    result |= result << 32;
    return result;
}

U64 southFill(U64 square)
{
    U64 result = square;
    result |= result >> 8;
    result |= result >> 16;
    result |= result >> 32;
    return result;
}

U64 westFill(U64 square)
{
    U64 result = square;
    result |= (result & ~A_FILE) >> 1;
    result |= (result & ~(A_FILE | B_FILE)) >> 2;
    result |= (result & ~(A_FILE | B_FILE | C_FILE | D_FILE)) >> 4;
    return result;
}

U64 eastFill(U64 square)
{
    U64 result = square;
    result |= (result & ~H_FILE) << 1;
    result |= (result & ~(H_FILE | G_FILE)) << 2;
    result |= (result & ~(H_FILE | G_FILE | F_FILE | E_FILE)) << 4;
    return result;
}

U64 northEastFill(U64 square)
{
    U64 result = square;
    result |= (result & ~H_FILE) << 9;
    result |= (result & ~(H_FILE | G_FILE)) << 18;
    result |= (result & ~(H_FILE | G_FILE | F_FILE | E_FILE)) << 36;
    return result;
}

U64 southEastFill(U64 square)
{
    U64 result = square;
    result |= (result & ~H_FILE) >> 7;
    result |= (result & ~(H_FILE | G_FILE)) >> 14;
    result |= (result & ~(H_FILE | G_FILE | F_FILE | E_FILE)) >> 28;
    return result;
}

U64 northWestFill(U64 square)
{
    U64 result = square;
    result |= (result & ~A_FILE) << 7;
    result |= (result & ~(A_FILE | B_FILE)) << 14;
    result |= (result & ~(A_FILE | B_FILE | C_FILE | D_FILE)) << 28;
    return result;
}

U64 southWestFill(U64 square)
{
    U64 result = square;
    result |= (result & ~A_FILE) >> 9;
    result |= (result & ~(A_FILE | B_FILE)) >> 18;
    result |= (result & ~(A_FILE | B_FILE | C_FILE | D_FILE)) >> 36;
    return result;
}