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