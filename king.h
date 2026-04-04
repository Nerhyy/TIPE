#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#include "shift.h"
#include "utils.h"
#include "cb_operators.h"

U64 k_target(U64 king);
U64 kingsAttacks_sq(int sq);
void fill_king_Attacks_table();
void castle_left_w(chessboard *cb);
void castle_right_w(chessboard *cb);
void castle_left_b(chessboard *cb);
void castle_right_b(chessboard *cb);

extern U64 kingAttacks[64];
