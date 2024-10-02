#include <vector>
#include <string>
#include <utility>
#include <chrono>
#include <iostream>
#include "attacks.hpp"
#include "uci.hpp"
#include "hashing.hpp"

int main()
{
    precomputeAttacks();
    precomputeZobristRandoms();
    UCI().loop();
}
