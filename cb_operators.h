#pragma once
#include "globals.h"
#include <stdlib.h>
#include <stdio.h>
#include<stdbool.h>

//Operation sur chessboard
U64 takenSquares(chessboard* echiquier);
U64 whitePieces(chessboard* echiquier);
U64 blackPieces(chessboard* echiquier);
chessboard* makeChessboard();
U64 LS1B(U64 mask);
int popCount (U64 x);
int bitScanForward(U64 mask);
U64 serialize(int sq);
chessboard* makeChessboard_og();
int popLSB(U64* mask);

U64 all_attacks(chessboard *cb ,int sq);
U64 any_attack_by_side(chessboard *cb, int sq , int color);
U64 all_color_piece(chessboard* cb, int color);
bool is_square_attacked(chessboard *cb, int sq, int color);

int what_black_p(chessboard* cb, U64 bit);
int what_white_p(chessboard* cb, U64 bit);
int deserialize(U64 sq);

U64 takenSquares_p(U64 piece[2][7]);

U64 whitePieces_p(U64 piece[2][7]);
U64 blackPieces_p(U64 piece[2][7]);

void init_piece_counts(chessboard* cb);