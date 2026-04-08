#pragma once

#include "tt.h"
#include "globals.h"

#define MAX_PLY 64 

extern move killer_moves[MAX_PLY][2];

void attribute_quiescence_order_score(int move_scores[], moveList l) ;
void attribute_order_score(int move_scores[], moveList l, move tt_best_move, int ply);
void clear_killer_moves();