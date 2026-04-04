#pragma once
#include "globals.h"
#include "shift.h"
#include "utils.h"
#include "cb_operators.h"

extern U64 rectLookup[64][64];

void fill_rectLookup_w_zeros();
bool edge_compare(int i);
void fill_rectLookup();
void affichage_rect();
U64 InBetween(int from, int to);
bool mayMove(int from, int to, U64 occ);