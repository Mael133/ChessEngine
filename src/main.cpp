#include "moveGenerator.hpp"
#include "board.hpp"

int main(){

    std::vector<Move> moves;
    Board board;


    //board.sideToMove = black;

    //board.showQueenMoves = true;
    board.showKnightMoves = true;
    //board.showPawnMoves = true;
    //board.showKingMoves = true;
    //board.showBishopMoves = true;
    //board.showRookMoves = true;
    
    //board.parseFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R ");
    board.parseFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");

    generateMoves(moves, board);

    std::cout << moves.size() << std::endl;

    board.printBoard();
    
    return 0;
}