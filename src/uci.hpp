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
    
    void handleCommand(std::stringstream& ss);
    
    void position(std::stringstream& tokens);

    void go(std::stringstream& tokens);
};