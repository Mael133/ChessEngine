#pragma once
#include "types.hpp"

struct Position{
    public:
    int row;
    int col;
    
    bitboard bitbrd;

    Position(int square){
        this->row = square/8;
        this->col = square%8;
        this->bitbrd = (1ULL << square);
    }
    Position(int row, int col){
        this->row = row;
        this->col = col;
        this->bitbrd = (1ULL << ((row*8)+col));
    }

    int getSquare(){
        return (row*8)+col;
    }
};