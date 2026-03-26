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

    if(board.sideToMove == white){
        pawns = board.bitboards[wp];
        direction = white;
        oponnent = black;
    }

    for(int i = 8; i < 56; i++){
        if(((1ULL<<i)&pawns) == 0) continue;

        if(!board.hasPieceAt(i+(8*direction))){//push
            Move move = addMove(moves, board, i, i+(8*direction));
            if (board.showPawnMoves) board.legalMove.push_back(move.to());
        }//push

        if(((i>7)&&(i<16)&&(direction==1)) || ((i>47)&&(i<56)&&(direction==-1))){//double push
            if(!board.hasPieceAt(i+(16*direction)) && !board.hasPieceAt(i+(8*direction))){
                Move move = addMove(moves, board, i, i+(16*direction));
                if (board.showPawnMoves) board.legalMove.push_back(move.to());
            } 
        }//double push

        if (((board.sideToMove == white) && (getSquareCol(i) != 0)) ||
            ((board.sideToMove == black) && (getSquareCol(i) != 7))){//right capture
            if(board.hasPieceAt(i+(7*direction), oponnent)){
                Move move = addMove(moves, board, i, i+(7*direction));
                if (board.showPawnMoves){
                    board.legalMove.push_back(move.to());
                    board.targetFromCarpture.push_back(move.to());
                }
            }
        }//right capture

        if (((board.sideToMove == white) && (getSquareCol(i) != 7)) ||
            ((board.sideToMove == black) && (getSquareCol(i) != 0))){//left capture
            if(board.hasPieceAt(i+(9*direction), oponnent)){
                Move move = addMove(moves, board, i, i+(9*direction));
                if (board.showPawnMoves){
                    board.legalMove.push_back(move.to());
                    board.targetFromCarpture.push_back(move.to());
                }
            }
        }//left capture
    }
}


void generateKnightMoves(std::vector<Move>& moves, Board& board){
    bitboard knights = board.bitboards[bn];
    int oponnent = white;

    if(board.sideToMove == white){
        knights = board.bitboards[wn];
        oponnent = black;
    }

    int attacks[][2] = {{2, 1}, {2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1}};
    int row = 0; int col = 1;

    for(int i = 0; i < 64; i ++){
        if(((1ULL<<i)&knights) == 0) continue;

        for(int * attack : attacks){
            if(((attack[col] + getSquareCol(i)) > 7) || ((attack[col] + getSquareCol(i)) < 0)) continue;

            int attackedSquare = i+(attack[col]+(attack[row]*8));
            if ((attackedSquare > 63) || (attackedSquare < 0)) continue;
            if (board.hasPieceAt(attackedSquare, board.sideToMove)) continue;

            Move move = addMove(moves, board, i, attackedSquare);

            if (!board.showKnightMoves) continue;
            if (move.isCapture()) board.targetFromCarpture.push_back(attackedSquare);
            board.legalMove.push_back(attackedSquare);
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