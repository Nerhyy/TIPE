//PIECE ADAPTED
#include <stdlib.h>
#include <stdio.h>
#include "cb_operators.h"
#include "globals.h"
#include "pawns.h"
#include "knights.h"
#include "king.h"
#include "rooks_and_bishops.h"




chessboard* makeChessboard(){
    chessboard* cb = malloc(sizeof(chessboard));

    cb->piece[WHITE][PAWN]    = 0;
    cb->piece[WHITE][ROOK]    = 0;
    cb->piece[WHITE][BISHOP]  = 0;
    cb->piece[WHITE][KNIGHT]  = 0;
    cb->piece[WHITE][QUEEN]   = 0;
    cb->piece[WHITE][KING]    = 0;

    cb->piece[BLACK][PAWN]    = 0;
    cb->piece[BLACK][ROOK]    = 0;
    cb->piece[BLACK][BISHOP]  = 0;
    cb->piece[BLACK][KNIGHT]  = 0;
    cb->piece[BLACK][QUEEN]   = 0;
    cb->piece[BLACK][KING]    = 0;

    cb->turn = 0;
    cb->castle = 0b0000;
    cb->enPassantSquare = -1;
    cb->halfmoveclock = 0;
    cb->fullmove = 0;

    return cb;
}

chessboard* makeChessboard_og(){
    chessboard* cb = malloc(sizeof(chessboard));
    cb->piece[WHITE][PAWN]    = 0b0000000000000000000000000000000000000000000000001111111100000000;
    cb->piece[WHITE][ROOK]    = 0b0000000000000000000000000000000000000000000000000000000010000001;
    cb->piece[WHITE][BISHOP]  = 0b0000000000000000000000000000000000000000000000000000000000100100;
    cb->piece[WHITE][KNIGHT]  = 0b0000000000000000000000000000000000000000000000000000000001000010;
    cb->piece[WHITE][QUEEN]   = 0b0000000000000000000000000000000000000000000000000000000000001000;
    cb->piece[WHITE][KING]    = 0b0000000000000000000000000000000000000000000000000000000000010000;

    cb->piece[BLACK][PAWN]    = 0b0000000011111111000000000000000000000000000000000000000000000000;
    cb->piece[BLACK][ROOK]    = 0b1000000100000000000000000000000000000000000000000000000000000000;
    cb->piece[BLACK][BISHOP]  = 0b0010010000000000000000000000000000000000000000000000000000000000;
    cb->piece[BLACK][KNIGHT]  = 0b0100001000000000000000000000000000000000000000000000000000000000;
    cb->piece[BLACK][QUEEN]   = 0b0000100000000000000000000000000000000000000000000000000000000000;
    cb->piece[BLACK][KING]    = 0b0001000000000000000000000000000000000000000000000000000000000000;

    cb->turn = 0;
    cb->castle = 0b1111;
    cb->enPassantSquare = -1;
    cb->halfmoveclock = 0;
    cb->fullmove = 0;

    return cb;
}
//modifié
U64 piecesOfColor(U64 piece[2][7], int color) {
    return piece[color][PAWN] | piece[color][ROOK] | piece[color][BISHOP] | piece[color][KNIGHT] | piece[color][QUEEN] | piece[color][KING];
}

//Renvoie un masque des cases occupees
U64 takenSquares(chessboard* echiquier){
    return piecesOfColor(echiquier->piece, WHITE) | piecesOfColor(echiquier->piece, BLACK);
}

U64 takenSquares_p(U64 piece[2][7]){
    return piecesOfColor(piece, WHITE) | piecesOfColor(piece, BLACK);
}

U64 whitePieces(chessboard* echiquier){
    return piecesOfColor(echiquier->piece, WHITE);
}

U64 blackPieces(chessboard* echiquier){
    return piecesOfColor(echiquier->piece, BLACK);
}

U64 whitePieces_p(U64 piece[2][7]){
    return piecesOfColor(piece, WHITE);
}

U64 blackPieces_p(U64 piece[2][7]){
    return piecesOfColor(piece, BLACK);
}

U64 LS1B(U64 mask){
    return ~(mask-1) & mask;
}


//Constantes pour le popCount
U64 k1 = 0x5555555555555555; /*  -1/3   */
U64 k2 = 0x3333333333333333; /*  -1/5   */
U64 k4 = 0x0f0f0f0f0f0f0f0f; /*  -1/17  */
U64 kf = 0x0101010101010101; /*  -1/255 */

//Compte la population d'un masque (COMPLEXITE PAS TOP)
int popCount (U64 x) {
    return __builtin_popcountll(x);
}

int bitScanForward(U64 mask) { //Renvoie l'indice du bit de point faible en LERFM
    U64 lsb;
    if(mask == 0){
        return -100;
    }
    mask &= -mask; // isolation du LS1B
    lsb = mask
        | mask>>32;
    return (((((((((((mask>>32) !=0)  * 2)
                  + ((lsb & 0xffff0000) !=0)) * 2)
                  + ((lsb & 0xff00ff00) !=0)) * 2)
                  + ((lsb & 0xf0f0f0f0) !=0)) * 2)
                  + ((lsb & 0xcccccccc) !=0)) * 2)
                  + ((lsb & 0xaaaaaaaa) !=0);
}

U64 serialize(int sq){ //OK
    U64 square = 1;
    for(int i = 0 ; i < sq ; i++){
        square <<=1;
    }
    return square;
}

int popLSB(U64* mask){

    int index = __builtin_ctzll(*mask);
    *mask &= (*mask - 1);

    return index;
}

U64 all_attacks(chessboard *cb ,int sq){ //renvoie les attaques d'une super piece en partant d'une case
    U64 knights;
    U64 kings;
    U64 rookQueens;
    U64 bishopQueens;

    knights       = cb->piece[WHITE][KNIGHT]  | cb->piece[BLACK][KNIGHT];
    kings         = cb->piece[WHITE][KING]    | cb->piece[BLACK][KING];
    rookQueens    = cb->piece[WHITE][QUEEN]   | cb->piece[BLACK][QUEEN];
    bishopQueens  = rookQueens;
    rookQueens   |= cb->piece[WHITE][ROOK]    | cb->piece[BLACK][ROOK];
    bishopQueens |= cb->piece[WHITE][BISHOP]  | cb->piece[BLACK][BISHOP];

    return    (rookAttacks(takenSquares(cb) , sq)  & rookQueens     )
            | (bishopAttacks(takenSquares(cb), sq) & bishopQueens   )
            | ((kingsAttacks_sq(sq))               & kings          )
            | (nAttacks(serialize(sq))             & knights        )
            | (wp_attacks_all(serialize(sq))       & cb->piece[WHITE][PAWN] )
            | (bp_attacks_all(serialize(sq))       & cb->piece[BLACK][PAWN] )
            ;
}
//modifié
U64 any_attack_by_side(chessboard *cb, int sq , int color){ 
    return all_attacks(cb,sq) & (cb->piece[color][BISHOP] | cb->piece[color][KING] | cb->piece[color][KNIGHT] | cb->piece[color][PAWN] | cb->piece[color][QUEEN] | cb->piece[color][ROOK]);
}


U64 all_color_piece(chessboard* cb, int color){
    return (cb->piece[color][BISHOP] | cb->piece[color][KING] | cb->piece[color][KNIGHT] | cb->piece[color][PAWN] | cb->piece[color][QUEEN] | cb->piece[color][ROOK]);
}
//modifié
bool is_square_attacked(chessboard *cb, int sq, int color) {

    U64 occ = takenSquares(cb);
    int other_color = 1 - color;
    if ((pawnAttacks[color][sq] & cb->piece[other_color][PAWN]) != 0)
    {
        return true;
    }
    if (((knightAttacks[sq]) & cb->piece[other_color][KNIGHT]) != 0)
    {
        return true;
    }
    if ((kingAttacks[sq] & cb->piece[other_color][KING]) != 0)
    {
        return true;
    }
    if ((bishopAttacks(occ, sq) & (cb->piece[other_color][QUEEN] | cb->piece[other_color][BISHOP])) != 0)
    {
        return true;
    }
    if ((rookAttacks(occ, sq) & (cb->piece[other_color][QUEEN] | cb->piece[other_color][ROOK])) != 0)
    {
        return true;
    }
    return false;
}
//modifié
int what_piece(chessboard* cb, U64 bit, int color) {
    if ((cb->piece[color][PAWN] & bit) != 0) {
        return PAWN;
    } else if ((cb->piece[color][KNIGHT] & bit) != 0) {
        return KNIGHT;
    } else if ((cb->piece[color][ROOK] & bit) != 0) {
        return ROOK;
    } else if ((cb->piece[color][BISHOP] & bit) != 0) {
        return BISHOP;
    } else if ((cb->piece[color][QUEEN] & bit) != 0) {
        return QUEEN;
    } else if ((cb->piece[color][KING] & bit) != 0) {
        return KING;
    } else {
        return EMPTY;
    }
}
//modifié
int what_black_p(chessboard* cb, U64 bit) {
    return what_piece(cb, bit, BLACK);
}
//modifié
int what_white_p(chessboard* cb, U64 bit) {
    return what_piece(cb, bit, WHITE);
}

int deserialize(U64 sq){ //Deserialize un mask unitaire
    if(sq == 0){
        printf("MASQUE VIDE --------------------------------------------------------------------------------------\n");
        return -1; //Que je trouve le probleme
    }
    return __builtin_ctzll(sq);
}
