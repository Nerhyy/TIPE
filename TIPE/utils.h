#pragma once
#include "globals.h"


void print_mask(U64 mask); //ok
void print_piece_positions(chessboard* echiquier); //ok
void print_chessboard_white(chessboard* echiquier); //ok
chessboard* convert_FEN_to_cb(char FEN[92]); //fonction intankable
