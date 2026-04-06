#pragma once
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "eval.h"
#include "pawns.h"
#include "knights.h"
#include "rooks_and_bishops.h"
#include "globals.h"
#include "shift.h"
#include "utils.h"
#include "cb_operators.h"
#include "king.h"
#include "rectlookup.h"


void castling_rights(chessboard* cb, move m, U64 from);
void promotion(chessboard* cb, move m, U64 to);
void makeMove(chessboard* cb, move m);
ld* create_lostdata();
ld create_lostdata2();
void reset_lostdata(ld* lostdata);
void makeMove_ld(chessboard* cb, move m, ld* lostdata);
void unmakeMove(chessboard* cb, move m, ld* lostdata);
bool legalmove_check(chessboard* cb, move m);
void free_moveList(moveList* l);
moveList* legalMoveList (chessboard* cb);
void affiche_legalmoveList(moveList* l);
moveList* legalCaptureMoveList (chessboard* cb);