#pragma once
#include <stdint.h>
#include <stdlib.h>
#include<stdio.h>
#include <string.h>
#include <time.h>
#include<stdbool.h>
typedef uint64_t U64; 

/////////////////////////////////////////// STRUCTURES | ENUMERATION //////////////////////////////////////////

//Type echiquier
//On peut réduire la taille de ces entiers à 1 octet ?
typedef struct echiquier{

    U64 piece[2][7]; //piece[COLOR][TYPE] et la case 0 sert à rien

    int turn; //A qui de jouer

    int castle; // 0000 les deux premiers bits sont les blancs pour castle left et right, les deux autres pour les noirs

    int enPassantSquare;
    int halfmoveclock;
    int fullmove;


}chessboard;

enum enumSquare { //serialized index for squares
    a1 = 0, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};

enum enumSquareS {
    A1 = 0x1,
    B1 = 0x2,
    C1 = 0x4,
    D1 = 0x8,
    E1 = 0x10,
    F1 = 0x20,
    G1 = 0x40,
    H1 = 0x80,
    A2 = 0x100,
    B2 = 0x200,
    C2 = 0x400,
    D2 = 0x800,
    E2 = 0x1000,
    F2 = 0x2000,
    G2 = 0x4000,
    H2 = 0x8000,
    A3 = 0x10000,
    B3 = 0x20000,
    C3 = 0x40000,
    D3 = 0x80000,
    E3 = 0x100000,
    F3 = 0x200000,
    G3 = 0x400000,
    H3 = 0x800000,
    A4 = 0x1000000,
    B4 = 0x2000000,
    C4 = 0x4000000,
    D4 = 0x8000000,
    E4 = 0x10000000,
    F4 = 0x20000000,
    G4 = 0x40000000,
    H4 = 0x80000000,
    A5 = 0x100000000,
    B5 = 0x200000000,
    C5 = 0x400000000,
    D5 = 0x800000000,
    E5 = 0x1000000000,
    F5 = 0x2000000000,
    G5 = 0x4000000000,
    H5 = 0x8000000000,
    A6 = 0x10000000000,
    B6 = 0x20000000000,
    C6 = 0x40000000000,
    D6 = 0x80000000000,
    E6 = 0x100000000000,
    F6 = 0x200000000000,
    G6 = 0x400000000000,
    H6 = 0x800000000000,
    A7 = 0x1000000000000,
    B7 = 0x2000000000000,
    C7 = 0x4000000000000,
    D7 = 0x8000000000000,
    E7 = 0x10000000000000,
    F7 = 0x20000000000000,
    G7 = 0x40000000000000,
    H7 = 0x80000000000000,
    A8 = 0x100000000000000,
    B8 = 0x200000000000000,
    C8 = 0x400000000000000,
    D8 = 0x800000000000000,
    E8 = 0x1000000000000000,
    F8 = 0x2000000000000000,
    G8 = 0x4000000000000000,
    H8 = 0x8000000000000000,
    
};

enum color{
    BLACK = 0,
    WHITE = 1
};


//-------------- MOVE LIST -----------------------------------------------

enum Piece{
    EMPTY  = 0,
    PAWN   = 1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK   = 4,
    QUEEN  = 5,
    KING   = 6
};

enum flag{
    NORMAL = 0,
    ENPASSANT = 1,
    CASTLE = 2
};

//On peut réduire la taille de ces entiers à 1 octet ?
typedef struct{
    int from; 
    int to;
    int piece;
    int captured;
    int promo;
    int flag;
}move;

/*
typedef struct{
    int peak; //de taille 256
    int lst[256];
}list; */

typedef struct{
    move moves[256]; //Le nombre de coups légaux maximaux est environ 218
    int count;
}moveList;

//On peut réduire la taille de ces entiers à 1 octet ?
typedef struct{
    int castle;
    int enPassantSquare;
    int halfmoveclock;
    int fullmove;
}ld; //Pour le unmake move

/////////////////////////////////////////// VARIABLES GLOBALES //////////////////////////////////////////

//On cree des masques des pieces sur 64bits pour faire des bitwise operations
//qui sont bien plus optimisees pour le calcul

//Masques utiles
extern U64 voidmask;  //ok
extern U64 univers; //ok

extern U64 Afile ; //ok
extern U64 Bfile ;//ok
extern U64 Cfile ;//ok
extern U64 Dfile ;//ok
extern U64 Efile ;//ok
extern U64 Ffile ;//ok
extern U64 Gfile ;//ok
extern U64 Hfile ; //ok

extern U64 row1;//ok
extern U64 row2;//ok
extern U64 row3; //ok
extern U64 row4; //ok
extern U64 row5; //ok
extern U64 row6; //ok
extern U64 row7; //ok
extern U64 row8; //ok

extern U64 lightSquares; //ok
extern U64 blackSquares; //ok

extern chessboard ogChessBoard;
extern chessboard emptyC;

extern move emptymove;

extern const U64 files[8];