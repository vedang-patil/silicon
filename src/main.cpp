#include <string>
#include <iostream>
#include "attacks.hpp"
#include "uci.hpp"

int main()
{
    precomputeAttacks();
    UCI uci([](const std::string& output) { std::cout << output << std::endl; });

    std::cout << "Silicon 2 by Vedang Patil" << std::endl;
    std::string line;
    
    do {
        getline(std::cin, line);
        uci.handleCommand(line);
    } while (line != "quit");
}
