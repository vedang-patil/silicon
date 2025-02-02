#pragma once
#include <vector>
#include <string>
#include "types.hpp"
#include "board.hpp"
#include "search.hpp"

typedef unsigned long long U64;

class UCI
{
private:
    Search* search = nullptr;
    Board board;

    void position(std::stringstream& tokens);
public:
    void handleCommand(const std::string& command);
};