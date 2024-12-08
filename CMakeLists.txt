cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

project(silicon)

add_executable(silicon src/main.cpp src/attacks.cpp src/bits.cpp src/board.cpp src/movegen.cpp src/search.cpp src/uci.cpp src/utils.cpp)
