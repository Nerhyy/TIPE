#pragma once
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

moveList* createList();
void addMove(moveList* list, int from, int to, int piece, int captured, int promo, int flag);
moveList* generatePawnMoves(chessboard* cb, moveList* list);
void addMove_bitboard(chessboard* cb, moveList* list, int sq, U64 moves, int piece);
moveList* generateSlidingMoves(chessboard* cb, moveList* l);
moveList* generateMoves(chessboard* cb, moveList* l);
void affiche_moveList(moveList* l);

