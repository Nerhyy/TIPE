
#include "king.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#include "shift.h"
#include "utils.h"
#include "cb_operators.h"
#include "rectlookup.h"

typedef uint64_t U64; 

//---------------------------------------- King -------------------------------------------------------------

U64 kingAttacks[64];

U64 k_target(U64 king){
    U64 attacks;
    attacks  = (shift_east(king) | shift_west(king));   //on fait la droite et la gauche du roi
    king    |= attacks;                                 //on setup une ligne pour la copier
    attacks |= (shift_north(king) | shift_south(king)); //on shift en haut et bas pour faire le carre (sans le roi au centre)
    return attacks;
}

//Pleins de fonctions a coder, voir le WIKI

U64 kingsAttacks_sq(int sq){
    U64 attacks;
    U64 kings;

    kings = serialize(sq);//serialisation de square index en masque

    attacks  = (shift_east(kings) | shift_west(kings));   //on fait la droite et la gauche du roi
    kings    |= attacks;                                 //on setup une ligne pour la copier
    attacks |= (shift_north(kings) | shift_south(kings)); //on shift en haut et bas pour faire le carre (sans le roi au centre)
    return attacks;
}

void fill_king_Attacks_table(){
    for(int i = 0; i < 64 ; i++){
        kingAttacks[i] = kingsAttacks_sq(i);
        //printf("i : %d\n", i);
        //print_mask(kingAttacks[i]);

    }
}

//---------------------------------------- CASTLE ------------------------------------------------------------
//Code modifié (refactorisation)
void castle(chessboard *cb, int color, int side) { // side: 0 for left (queenside), 1 for right (kingside)
    int king_sq = bitScanForward(cb->piece[color][KING]);
    U64 rook_mask = cb->piece[color][ROOK];
    U64 left_rook = LS1B(rook_mask);
    U64 right_rook = rook_mask - left_rook;
    int rook_sq;
    U64 king_new, rook_new;
    
    if (side == 0) { // left castle
        rook_sq = bitScanForward(left_rook);
        if ((rectLookup[king_sq][rook_sq] & takenSquares(cb)) == 0) {
            king_new = left_rook << 1;
            rook_new = king_new << 1;
            cb->piece[color][KING] = king_new;
            cb->piece[color][ROOK] = (rook_mask - left_rook) + rook_new;
            printf("castle successful\n");
        } else {
            printf("Something is in the way!\n");
        }
    } else { // right castle
        rook_sq = bitScanForward(right_rook);
        if ((rectLookup[king_sq][rook_sq] & takenSquares(cb)) == 0) {
            king_new = right_rook >> 1;
            rook_new = king_new >> 1;
            cb->piece[color][KING] = king_new;
            cb->piece[color][ROOK] = left_rook + rook_new;
            printf("castle successful\n");
        } else {
            printf("Something is in the way!\n");
        }
    }
}

void castle_left_w(chessboard *cb) {
    castle(cb, WHITE, 0);
}

void castle_right_w(chessboard *cb) {
    castle(cb, WHITE, 1);
}

void castle_left_b(chessboard *cb) {
    castle(cb, BLACK, 0);
}

void castle_right_b(chessboard *cb) {
    castle(cb, BLACK, 1);
}

