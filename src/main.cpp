#include "moveGenerator.hpp"
#include "board.hpp"

int main(){

    std::vector<Move> moves;
    Board board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ");

    //board.showQueenMoves = true;
    board.showKnightMoves = true;
    //board.showPawnMoves = true;
    //board.showKingMoves = true;
    //board.showBishopMoves = true;
    //board.showRookMoves = true;

    generateMoves(moves, board);

    std::cout << moves.size() << std::endl;

    board.printBoard();

    std::cout << "\n" << board.whiteKingSquare << " " << board.blackKingSquare << "\n";

    std::cout << "White K: " << board.wKingSideCastle << "\n"; 
    std::cout << "White Q: " << board.wQueenSideCastle << "\n"; 
    std::cout << "Black K: " << board.bKingSideCastle << "\n"; 
    std::cout << "Black Q: " << board.bQueenSideCastle << "\n"; 
    
    return 0;
}