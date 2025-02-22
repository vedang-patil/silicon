#pragma once
#include <vector>
#include <string>
#include <functional>
#include "types.hpp"
#include "board.hpp"
#include "search.hpp"

typedef unsigned long long U64;

class UCI
{
private:
    std::function<void(const std::string&)> outputCallback;
    Search* search = nullptr;
    Board board;

    void position(std::stringstream& tokens);
public:
    UCI(std::function <void(const std::string&)> outputCallback);

    void handleCommand(const std::string& command);
};