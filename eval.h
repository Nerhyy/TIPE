#pragma once
#include "globals.h"
#include "arrFrontSpans.h"
#include "rectlookup.h"
#include "utils.h"
#include "knights.h"
#include "king.h"
#include "rooks_and_bishops.h"
#include "pawns.h"


extern const int* all_placements[2][7];

extern const int* all_placements_endgame[2][7];

extern const int piece_value[7];

int piece_eval(U64 piece[2][7]);
int piece_eval_endgame(U64 piece[2][7]);
int double_pawns(U64 piece[2][7]);
void open_files(U64 piece[2][7], bool* white_openfiles, bool* black_openfiles);
int isolated_pawns(U64 piece[2][7]);
int bishop_pair(int piece_counts[2][7]);
int rook_on_open_files(U64 piece[2][7]);
int bastion(U64 piece[2][7]);
int passed_pawns(U64 piece[2][7])  ;
int rook_on_7th(U64 piece[2][7])  ;
int doubled_rooks(U64 piece[2][7]);
int mobility (U64 piece[2][7]);
int pawn_chain(U64 piece[2][7]);
int pawn_eval(U64 piece[2][7]);
int other_eval(U64 piece[2][7], int piece_counts[2][7]);
int castling_eval(chessboard* cb);
int eval(chessboard* cb);