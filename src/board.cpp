#include "board.hpp"
#include "position.hpp"
#include "move.hpp"

Board::Board(){
    sideToMove = white;

    //startig position
    bitboards[bp] = 0xff000000000000;
    bitboards[br] = 0x8100000000000000;
    bitboards[bn] = 0x4200000000000000;
    bitboards[bb] = 0x2400000000000000;
    bitboards[bk] = 0x800000000000000;
    bitboards[bq] = 0x1000000000000000;

    bitboards[wp] = 0xff00;
    bitboards[wr] = 0x81;
    bitboards[wn] = 0x42;
    bitboards[wb] = 0x24;
    bitboards[wk] = 0x8;
    bitboards[wq] = 0x10;
};

bitboard Board::allPieces(){
    return bitboards[wp] | bitboards[bp]|
            bitboards[wr] | bitboards[br]|
            bitboards[wn] | bitboards[bn]|
            bitboards[wb] | bitboards[bb]|
            bitboards[wq] | bitboards[bq]|
            bitboards[wk] | bitboards[bk];
}
bitboard Board::allPieces(int color){
    bool offset = (color == white);
    return bitboards[bp-(offset*6)] | bitboards[br-(offset*6)]|
            bitboards[bn-(offset*6)] | bitboards[bb-(offset*6)]|
            bitboards[bq-(offset*6)] | bitboards[bk-(offset*6)];
}

bool Board::hasPieceAt(int square){
    return ((1ULL << square)&allPieces()) != 0;
}

bool Board::hasPieceAt(int square, int color){
    return ((1ULL << square)&allPieces(color)) != 0;
}

void Board::printBoard(){
    //♙♟
    std::string images[12] = {"♟", "♜", "♞", "♝", "♛", "♚", "♟", "♜", "♞", "♝", "♛", "♚"};

    std::string background;
    
    //---lichess colors
    //std::string whitebg   = "\033[48;2;200;177;141m";
    //std::string darkbg    = "\033[48;2;141;96;59m";

    //---green
    //std::string green     = "\033[48;2;9;140;63m";
    //std::string darkGreen = "\033[48;2;9;100;63m";

    std::string lightbg   = "\033[48;2;118;124;202m";
    std::string darkbg    = "\033[48;2;53;58;128m";
    std::string whitefg   = "\033[38;2;240;240;240m";
    std::string blackfg   = "\033[38;2;0;0;0m";   

    std::string red       = "\033[48;2;160;9;63m";
    std::string darkRed   = "\033[48;2;120;9;43m";
    
    std::string bege      = "\033[48;2;173;159;156m";
    std::string darkBege  = "\033[48;2;132;119;114m";


    //std::cout << "\033[H\033[2J\033[3J"; //clears the screen
    bool empty = false;
    std::cout << "\n";
    for(int j = 63; j > -1; j--){
        empty = true;
        if((j+(j/8))%2==0) background = lightbg;
        else background = darkbg;
        
        for(int square : legalMove){
            if(square == j){
                if((j+(j/8))%2==0) background = bege;
                else background = darkBege;
            }
        }
        for(int square : targetFromCarpture){
            if(square == j){
                if((j+(j/8))%2==0) background = red;
                else background = darkRed;
            } 
        }
        
        for(int i = 0; i < 12; i++){
            if((bitboards[i]>>j)%2 == 1){
                empty = false;

                std::string pieceColor = (i < 6) ? whitefg : blackfg;

                std::cout << background << pieceColor << images[i] << " ";
            }
        }
        if (empty) std::cout << background << "  ";
        if(j%8==0) std::cout<<"\033[0m\n";
    }
    std::cout << "\033[0m"; 
}

piece Board::getPieceAt(int square){
    for(int i = 0; i < 12; i++){
        if((bitboards[i] & (1ULL<<square)) != 0) return piece(i);
    }
}
