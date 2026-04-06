#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "types.hpp"
#include "move.hpp"


struct Board{
    bitboard bitboards[12] = {0x0ULL};
    int sideToMove = white;

    int whiteKingSquare;
    int blackKingSquare;

    bool wKingSideCastle = false;
    bool wQueenSideCastle = false;
    bool bKingSideCastle = false;
    bool bQueenSideCastle = false;
    
    bool showPawnMoves = false;
    bool showRookMoves = false;
    bool showKnightMoves = false;
    bool showBishopMoves = false;
    bool showQueenMoves = false;
    bool showKingMoves = false;

    std::vector<int> targetFromCarpture;
    std::vector<int> legalMove;

    Board(std::string fen);

    bitboard allPieces();
    bitboard allPieces(int color);

    bool hasPieceAt(int square);

    bool hasPieceAt(int square, int color);

    int getPieceAt(int square);

    int getPieceByName(char name);

    void printBoard();

    void parseFen(std::string fen);
};