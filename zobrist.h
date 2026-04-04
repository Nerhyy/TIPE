#pragma once

#include "globals.h"
#include "cb_operators.h"



extern U64 zobrist_pieces[2][7][64];
extern U64 zobrist_castling[16];
extern U64 zobrist_en_passant[8];
extern U64 zobrist_black_to_move;

void init_zobrist();
U64 generate_hash(chessboard* cb);
