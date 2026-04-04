#pragma once

#include <string.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "pawns.h"
#include "knights.h"
#include "rooks_and_bishops.h"
#include "globals.h"
#include "shift.h"
#include "utils.h"
#include "cb_operators.h"
#include "king.h"
#include "rectlookup.h"
#include "move_gen.h"
#include "move_making.h"

typedef uint64_t U64; 

void counting_stats(chessboard* cb, move m, int depth);
U64 perft(chessboard *cb, int depth);
void test_perft(chessboard* cb, int n);

extern int cpt_capt;
extern int cpt_check;
extern int cpt_ep;
extern int cpt_promo;
extern int cpt_castle;
extern int cpt_checkmate;

extern char FENempty[92];
extern char fenpft1[92];
extern char fenpft2[92];
extern char fenpft3[92];
extern char fenpft4[92];
extern char fenpft5[92];