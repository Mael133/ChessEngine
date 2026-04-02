#include "board.hpp"
#include "types.hpp"
#include "move.hpp"

int getSquareCol(int square){return square % 8;}
int getSquareRow(int row){return row / 8;}

Move addMove(std::vector<Move>& moves, Board& board, int startPosition, int targetPosition){
    int oponnent = white;
    if(board.sideToMove == white) oponnent = black;
    
    Move move(startPosition, targetPosition);
    if(board.hasPieceAt(targetPosition, oponnent)) move.setCapture();
    else move.resetCapture();
    moves.push_back(move);
    return move;
}

void generatePawnMoves(std::vector<Move>& moves, Board& board){
    bitboard pawns = board.bitboards[bp];
    int direction = black;
    int oponnent = white;
    bitboard possiblePushes        = (pawns >> 8) & ~board.allPieces();             
    bitboard possibleDoublePushes  = (possiblePushes >> 8) & ~board.allPieces() & 0xff00000000ULL;
    bitboard possibleRightCaptures = ((pawns & 0x7f7f7f7f7f7f7f7fULL) >> 7) & board.allPieces(oponnent);
    bitboard possibleLeftCaptures  = ((pawns & 0xfefefefefefefefeULL) >> 9) & board.allPieces(oponnent);

    if(board.sideToMove == white){
        pawns = board.bitboards[wp];
        direction = white;
        oponnent = black;
        possiblePushes        = (pawns << 8) & ~board.allPieces();             
        possibleDoublePushes  = (possiblePushes << 8) & ~board.allPieces() & 0xff000000ULL;
        possibleRightCaptures = ((pawns & 0xfefefefefefefefeULL) << 7) & board.allPieces(oponnent);
        possibleLeftCaptures  = ((pawns & 0x7f7f7f7f7f7f7f7fULL) << 9) & board.allPieces(oponnent);
    }

    while (possiblePushes) { //push
        int targetSquare = __builtin_ctzll(possiblePushes);
        int startSquare = targetSquare - 8*direction;
        addMove(moves, board, startSquare, targetSquare);
        if(board.showPawnMoves) board.legalMove.push_back(targetSquare);
        possiblePushes &= (possiblePushes - 1);
    }

    while (possibleDoublePushes) { //double push
        int targetSquare = __builtin_ctzll(possibleDoublePushes);
        int startSquare = targetSquare - 16*direction;
        addMove(moves, board, startSquare, targetSquare);
        if(board.showPawnMoves) board.legalMove.push_back(targetSquare);
        possibleDoublePushes &= (possibleDoublePushes - 1);
    }

    while (possibleRightCaptures) { //right captures
        int targetSquare = __builtin_ctzll(possibleRightCaptures);
        int startSquare = targetSquare - 16*direction;
        addMove(moves, board, startSquare, targetSquare);
        if(board.showPawnMoves) board.targetFromCarpture.push_back(targetSquare);
        possibleRightCaptures &= (possibleRightCaptures - 1);
    }

    while (possibleLeftCaptures) { //left captures
        int targetSquare = __builtin_ctzll(possibleLeftCaptures);
        int startSquare = targetSquare - 16*direction;
        addMove(moves, board, startSquare, targetSquare);
        if(board.showPawnMoves) board.targetFromCarpture.push_back(targetSquare);
        possibleLeftCaptures &= (possibleLeftCaptures - 1);
    }
}

void generateKnightMoves(std::vector<Move>& moves, Board& board){
    bitboard knights = board.bitboards[bn];
    int oponnent = white;

    if(board.sideToMove == white){
        knights = board.bitboards[wn];
        oponnent = black;
    }

    int attacks[] = {17, 15, 10, 6, 6, 10, 15, 17};
    bitboard possibleAtacks;
    int direction;
    for (int i = 0; i < 8; i++){
        if(i>3) {
            possibleAtacks = (knights>>attacks[i]) & ~board.allPieces(board.sideToMove);
            direction = -1;
        }
        else {
            possibleAtacks = (knights<<attacks[i]) & ~board.allPieces(board.sideToMove);
            direction = 1;
        }
        while (possibleAtacks) {
            int targetSquare = __builtin_ctzll(possibleAtacks);
            int startSquare = targetSquare - attacks[i]*direction;

            if (std::abs(getSquareCol(targetSquare) - getSquareCol(startSquare)) <= 2){
                addMove(moves, board, startSquare, targetSquare);
                if(board.showKnightMoves) board.targetFromCarpture.push_back(targetSquare);
            }
            
            possibleAtacks &= (possibleAtacks - 1);
        }
    }
}

void generateKingMoves(std::vector<Move>& moves, Board& board){
    bitboard kings = board.bitboards[bk];
    if(board.sideToMove == white) kings = board.bitboards[wk];
    
    for(int i = 0; i < 64; i++){
        if(((1ULL<<i)&kings) == 0) continue;
        
        for(int direction : {1, -1, 8, -8, 7, -7, 9, -9}){
            int attackedSquare = i+direction;
            if(std::abs(getSquareCol(i)-(attackedSquare%8)) > 1) continue;
            if ((attackedSquare > 63) || (attackedSquare < 0)) continue;
            if(!board.hasPieceAt(attackedSquare, board.sideToMove)){
                Move move = addMove(moves, board, i, attackedSquare);
                if (board.showKingMoves){
                    if (move.isCapture()){
                        board.targetFromCarpture.push_back(attackedSquare);
                    }else{
                        board.legalMove.push_back(attackedSquare);
                    }
                }
                if(((attackedSquare%8)==0) || ((attackedSquare%8)==7))continue;
            }
        }
    }
}

void generateSlidingnMoves(std::vector<Move>& moves, Board& board, bitboard piece, std::vector<int>directions){
    bitboard pieces = board.bitboards[piece+6];
    if(board.sideToMove == white) pieces = board.bitboards[piece];

    for(int i = 0; i < 64; i++){
        if(((1ULL<<i)&pieces) == 0) continue;
        
        for(int direction : directions){
            int currentSquare = i;
            while(true){
                int attackedSquare = currentSquare + direction;
                if ((attackedSquare > 63) || (attackedSquare < 0)) break;
                if (std::abs(getSquareCol(currentSquare) - getSquareCol(attackedSquare)) > 2) break;
                if(board.hasPieceAt(attackedSquare, board.sideToMove)) break;

                Move move = addMove(moves, board, i, attackedSquare);
                
                if (board.showQueenMoves){
                    if (move.isCapture()){
                        board.targetFromCarpture.push_back(attackedSquare);
                    }else{
                        board.legalMove.push_back(attackedSquare);
                    }
                }

                if(move.isCapture()) break;
                currentSquare = attackedSquare; 
            }
        }
    }
}

void generateMoves(std::vector<Move>& moves, Board& board){
    moves.clear();

    generatePawnMoves    (moves, board);
    generateKnightMoves  (moves, board);
    generateKingMoves    (moves, board);
    generateSlidingnMoves(moves, board, wr, {8,1,-1,-8});
    generateSlidingnMoves(moves, board, wb, {7,9,-7,-9});
    generateSlidingnMoves(moves, board, wq, {8,1,-1,-8,7,9,-7,-9});
}