#pragma once
#include <stdint.h>
#include <stdlib.h>
typedef uint64_t U64; 

/////////////////////////////////////////// STRUCTURES | ENUMERATION //////////////////////////////////////////

//Type echiquier
typedef struct echiquier{

    //Pieces blanches
    U64 whitePawns;
    U64 whiteRooks;
    U64 whiteBishops;
    U64 whiteKnights;
    U64 whiteQueens;
    U64 whiteKing;

    //Pieces noires

    U64 blackPawns;
    U64 blackRooks;
    U64 blackBishops;
    U64 blackKnights;
    U64 blackQueens;
    U64 blackKing;

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