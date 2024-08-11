#include <vector>
#include <string>
#include <utility>
#include <chrono>
#include <iostream>
#include "attacks.hpp"
#include "uci.hpp"

int main()
{
    precomputeAttacks();
    UCI().loop();
}
