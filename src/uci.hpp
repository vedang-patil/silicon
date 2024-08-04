#pragma once
#include <vector>
#include <string>
#include "types.hpp"
#include "board.hpp"

typedef unsigned long long U64;

class UCI
{
private:
    Board board;

public:
    void loop();
    
    void handleCommand(const std::string& command);
    
    void position(const std::vector<std::string>& tokens);

    void go(const std::vector<std::string>& tokens);
};