#include "board.hpp"
#include "move.hpp"
#include <string.h>

Board::Board(std::string fen){
    parseFen(fen);
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

int Board::getPieceAt(int square){
    for(int i = 0; i < 12; i++){
        if((bitboards[i] & (1ULL<<square)) != 0) return i;
    }
}

int Board::getPieceByName(char name){
    std::string names = "PRNBQKprnbqk";
    return static_cast<int>(names.find(name, 0));
}

void Board::printBoard(){
    //♙♟
    std::string images[12] = {"♟", "♜", "♞", "♝", "♛", "♚", "♟", "♜", "♞", "♝", "♛", "♚"};

    std::string background;
    
    //---lichess colors
    std::string whitebg   = "\033[48;2;200;177;141m";
    std::string darkbg    = "\033[48;2;141;96;59m";

    //---green
    //std::string green     = "\033[48;2;9;140;63m";
    //std::string darkGreen = "\033[48;2;9;100;63m";

    //std::string whitebg   = "\033[48;2;118;124;202m";
    //std::string darkbg    = "\033[48;2;53;58;128m";
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
        if((j+(j/8))%2==0) background = whitebg;
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

void Board::parseFen(std::string fen){
    memset(bitboards, 0, sizeof(bitboards));
    int row = 7;
    int col = 7;
    int index = 0;
    for(char i : fen){
        index++;
        if(i == ' ')break;
        if(i == '/'){
            row--;
            col = 7;
            continue;
        }
        if(std::isdigit(i)){
            col -= (int)(i - '0');
            continue;
        }
        
        int square = col + (row*8);
        int piece = getPieceByName(i);
        bitboards[piece] |= (1ULL<<square);
        if(piece == wk) whiteKingSquare = col + (row*8);
        if(piece == bk) blackKingSquare = col + (row*8);

        col--;
        

    }
    if(fen[index] == 'w') sideToMove = white;
    else sideToMove = black;
    index += 2;
    if (fen[index] == '-'){
        index += 2;
    }else{
        for(int i = 0; i < 4; i ++){
            if(i!=0)index++;
            if(fen[index] == 'K'){
                wKingSideCastle = true;
            }else if(fen[index] == 'Q'){
                wQueenSideCastle = true;
            }else if(fen[index] == 'k'){
                bKingSideCastle = true;
            }else if(fen[index] == 'q'){
                bQueenSideCastle = true;
            }else{
                break;   
            }
        }
        index++;
    }
}
