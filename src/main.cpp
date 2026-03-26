#include "moveGenerator.hpp"
#include "board.hpp"

int main(){

    std::vector<Move> moves;
    Board board;


    //board.sideToMove = black;

    
    //board.showQueenMoves = true;
    //board.showKnightMoves = true;
    //board.showPawnMoves = true;
    //board.showKingMoves = true;
    //board.showBishopMoves = true;
    //board.showRookMoves = true;
    

    /*
    board.bitboards[br] = 0x1400000000000000;
    board.bitboards[bq] = 0x20000000000000;
    board.bitboards[bb] = 0x8000000000000;
    board.bitboards[bn] = 0x200000000000;
    board.bitboards[bp] = 0x740a000000000;
    board.bitboards[bk] = 0x200000000000000;
    board.bitboards[wp] = 0x480204500;
    board.bitboards[wr] = 0x88;
    board.bitboards[wq] = 0x400000;
    board.bitboards[wn] = 0x20000;
    board.bitboards[wb] = 0x200;
    board.bitboards[wk] = 0x2;
    */
    
    board.parseFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R ");

    generateMoves(moves, board);

    std::cout << moves.size() << std::endl;

    board.printBoard();
    
    return 0;
}