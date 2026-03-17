cmake_minimum_required(VERSION 3.10)

project(ChessEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(ChessEngine
    src/main.cpp
    src/Board.cpp
    src/MoveGenerator.cpp
)

target_include_directories(ChessEngine PRIVATE ${CMAKE_SOURCE_DIR}/include)