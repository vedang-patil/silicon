#pragma once
#include <vector>
#include <string>
#include "board.hpp"

typedef unsigned long long U64;

class UCI
{
private:
    Board board;
public:
    UCI();

    void handleCommand(const std::string& command);
    
    void position(const std::vector<std::string>& tokens);

    void go(const std::vector<std::string>& tokens);
};