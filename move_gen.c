#include "move_gen.h"

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

//////////////////////////////////////////////////// MOVE GENERATION //////////////////////////////////////////


moveList* createList(){
    moveList* l = malloc(sizeof(moveList));
    l->count = 0;
    return l;
}

void addMove(moveList* list, int from, int to, int piece, int captured, int promo, int flag){
    move m = {from, to, piece, captured, promo, flag};
    list->moves[list->count + 1] = m;
    list->count++;
}


moveList* generatePawnMoves(chessboard* cb, moveList* list){

    if(cb->turn == WHITE){

    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||WHITE||||||||||||||||||||||||||||||||||||||||||||||||||

    U64 pawns =  cb->piece[WHITE][PAWN];   // les cases occupées par les pions
    U64 empty = ~takenSquares(cb); // les cases libres
    U64 b_pieces = blackPieces(cb);

    //Left capture___________________________________________________________________

    U64 left_cap = (pawns << 7) & b_pieces & ~Hfile;
    while(left_cap){
        int to = popLSB(&left_cap);
        int from = to -7;
        int capturedPiece = what_black_p(cb, serialize(to));
        if(to >= 56){
            addMove(list, from, to, PAWN, capturedPiece, QUEEN, NORMAL);
            addMove(list, from, to, PAWN, capturedPiece, ROOK, NORMAL);
            addMove(list, from, to, PAWN, capturedPiece, KNIGHT, NORMAL);
            addMove(list, from, to, PAWN, capturedPiece, BISHOP, NORMAL);

        }
        else{
            addMove(list, from, to, PAWN, capturedPiece, EMPTY, NORMAL);
        }
    }

    //Right capture___________________________________________________________________

    U64 right_cap = (pawns << 9) & b_pieces & ~Afile;
    while(right_cap){
        int to = popLSB(&right_cap);
        int from = to -9;
        int capturedPiece = what_black_p(cb, serialize(to));
        if(to >= 56){
            addMove(list, from, to, PAWN, capturedPiece, QUEEN, NORMAL);
            addMove(list, from, to, PAWN, capturedPiece, ROOK, NORMAL);
            addMove(list, from, to, PAWN, capturedPiece, KNIGHT, NORMAL);
            addMove(list, from, to, PAWN, capturedPiece, BISHOP, NORMAL);

        }
        else{
            addMove(list, from, to, PAWN, capturedPiece, EMPTY, NORMAL);
        }
    }

    if((cb->enPassantSquare != -1) && cb->enPassantSquare > 39){
        U64 candidates = serialize(cb->enPassantSquare);
        U64 remove_junk = candidates;

        //printf("ENPASSANTSQUARE : \n");
        //print_mask(candidates);

        candidates |= ((((candidates >> 9) & ~Hfile) | ((candidates >> 7) & ~Afile)));
        candidates &= ~remove_junk;

        //printf("ENPASSANT ATTACKS : \n");
        //print_mask(candidates);
        candidates &= cb->piece[WHITE][PAWN];



        while(candidates){
            int from = popLSB(&candidates);
            addMove(list, from, cb->enPassantSquare, PAWN, PAWN, EMPTY, ENPASSANT);
        }
    }

    //Simple pawn push________________________________________________________________

    U64 singlepush = (pawns << 8) & empty;
    U64 quietmoves = singlepush & ~row8;
    U64 promotions = singlepush &  row8;

    while(quietmoves){ //on s'arrête quand quietmove est égal à 0
        int to = popLSB(&quietmoves);
        int from = to - 8;
        addMove(list, from, to, PAWN, EMPTY, EMPTY, NORMAL);
    }

    while(promotions){
        int to   = popLSB(&promotions);
        int from = to - 8;
        addMove(list, from, to, PAWN, EMPTY, QUEEN, NORMAL);
        addMove(list, from, to, PAWN, EMPTY, ROOK, NORMAL);
        addMove(list, from, to, PAWN, EMPTY, KNIGHT, NORMAL);
        addMove(list, from, to, PAWN, EMPTY, BISHOP, NORMAL);
    }

    //Double pawn push_______________________________________________________________

    U64 doublepush = ((((pawns << 8) & empty) << 8) & empty) & row4; //c'est pas beau, mais ça fait l'affaire
    while(doublepush){
        int to   = popLSB(&doublepush);
        int from = to - 16;
        addMove(list, from, to, PAWN, EMPTY, EMPTY, NORMAL);
    }



    return list;

    }


    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||BLACK||||||||||||||||||||||||||||||||||||||||||||||||||
    else if(cb->turn == BLACK){

    U64 pawns =  cb->piece[BLACK][PAWN];   // les cases occupées par les pions
    U64 empty = ~takenSquares(cb); // les cases libres
    U64 w_pieces = whitePieces(cb);

    //Left capture___________________________________________________________________

    U64 left_cap = (pawns >> 7) & w_pieces & ~Afile;
    while(left_cap){
        int to = popLSB(&left_cap);
        int from = to + 7;
        int capturedPiece = what_white_p(cb, serialize(to));
        if(to <= 7){
            addMove(list, from, to, PAWN, capturedPiece, QUEEN, NORMAL);
            addMove(list, from, to, PAWN, capturedPiece, ROOK, NORMAL);
            addMove(list, from, to, PAWN, capturedPiece, KNIGHT, NORMAL);
            addMove(list, from, to, PAWN, capturedPiece, BISHOP, NORMAL);

        }
        else{
            addMove(list, from, to, PAWN, capturedPiece, EMPTY, NORMAL);
        }
    }

    //Right capture___________________________________________________________________

    U64 right_cap = (pawns >> 9) & w_pieces & ~Hfile;
    while(right_cap){
        int to = popLSB(&right_cap);
        int from = to + 9;
        int capturedPiece = what_white_p(cb, serialize(to));
        if(to <= 7){
            addMove(list, from, to, PAWN, capturedPiece, QUEEN, NORMAL);
            addMove(list, from, to, PAWN, capturedPiece, ROOK, NORMAL);
            addMove(list, from, to, PAWN, capturedPiece, KNIGHT, NORMAL);
            addMove(list, from, to, PAWN, capturedPiece, BISHOP, NORMAL);

        }
        else{
            addMove(list, from, to, PAWN, capturedPiece, EMPTY, NORMAL);
        }
    }

    if((cb->enPassantSquare != -1) && cb->enPassantSquare < 24 && cb->enPassantSquare > 15){
        U64 candidates = serialize(cb->enPassantSquare);
        U64 remove_junk = candidates;
        candidates |= ((candidates << 9) & ~Afile) | ((candidates << 7) & ~Hfile);
        candidates &= ~remove_junk    ;
        //printf("candidates : \n");
        //print_mask(candidates);
        candidates &= cb->piece[BLACK][PAWN];

        while(candidates){
            int from = popLSB(&candidates);
            addMove(list, from, cb->enPassantSquare, PAWN, PAWN, EMPTY, ENPASSANT);
        }
    }

    //Simple pawn push________________________________________________________________

    U64 singlepush = (pawns >> 8) & empty;
    U64 quietmoves = singlepush & ~row1;
    U64 promotions = singlepush &  row1;

    while(quietmoves){ //on s'arrête quand quietmove est égal à 0
        int to = popLSB(&quietmoves);
        int from = to + 8;
        addMove(list, from, to, PAWN, EMPTY, EMPTY, NORMAL);
    }

    while(promotions){
        int to   = popLSB(&promotions);
        int from = to + 8;
        addMove(list, from, to, PAWN, EMPTY, QUEEN, NORMAL);
        addMove(list, from, to, PAWN, EMPTY, ROOK, NORMAL);
        addMove(list, from, to, PAWN, EMPTY, KNIGHT, NORMAL);
        addMove(list, from, to, PAWN, EMPTY, BISHOP, NORMAL);
    }

    //Double pawn push_______________________________________________________________

    U64 doublepush = ((((pawns >> 8) & empty) >> 8) & empty) & row5; //c'est pas beau, mais ça fait l'affaire
    while(doublepush){
        int to   = popLSB(&doublepush);
        int from = to + 16;
        addMove(list, from, to, PAWN, EMPTY, EMPTY, NORMAL);
    }

    return list;

    }

    //cb->turn non valide
    else{
        printf("erreur turn\n");
    }
}


void addMove_bitboard(chessboard* cb, moveList* list, int sq, U64 moves, int piece){ //Pas pour les pions
    while(moves){
        int to = popLSB(&moves);

        if(cb->turn == WHITE){
            int captured = what_black_p(cb, serialize(to));
            addMove(list, sq, to, piece, captured, EMPTY, NORMAL);
        }
        else if(cb->turn == BLACK){
            int captured = what_white_p(cb, serialize(to));
            addMove(list, sq, to, piece, captured, EMPTY, NORMAL);
        }
    }
}

moveList* generateSlidingMoves(chessboard* cb, moveList* l){

    U64 occ = takenSquares(cb);

    U64 bishops = (cb->turn == WHITE) ? cb->piece[WHITE][BISHOP] : cb->piece[BLACK][BISHOP] ;
    U64 rooks   = (cb->turn == WHITE) ? cb->piece[WHITE][ROOK]   : cb->piece[BLACK][ROOK]   ;
    U64 queens  = (cb->turn == WHITE) ? cb->piece[WHITE][QUEEN]  : cb->piece[BLACK][QUEEN]  ;

    U64 ownpieces = 0;
    if(cb->turn == WHITE){
        ownpieces = whitePieces(cb);
    }
    else{
        ownpieces = blackPieces(cb);
    }


    while(bishops){
        int sq      = popLSB(&bishops);
        U64 attacks = bishopAttacks(occ, sq) & ~ownpieces;
        addMove_bitboard(cb, l, sq, attacks, BISHOP);
    }
    while(rooks){
        int sq      = popLSB(&rooks);
        U64 attacks = rookAttacks(occ, sq) & ~ownpieces;
        addMove_bitboard(cb, l, sq, attacks, ROOK);
    }
    while(queens){
        int sq      = popLSB(&queens);
        U64 attacks = (bishopAttacks(occ, sq) | rookAttacks(occ, sq)) & ~ownpieces;
        addMove_bitboard(cb, l, sq, attacks, QUEEN);
    }

    return l;
}
//modifié
moveList* generateKnightMoves(chessboard* cb, moveList*l)
{
    U64 w_p = whitePieces(cb);
    U64 b_p = blackPieces(cb);
    U64 knights = (cb->turn == WHITE) ? cb->piece[WHITE][KNIGHT] : cb->piece[BLACK][KNIGHT];
    U64 ownpieces = (cb->turn == WHITE) ? w_p : b_p;

    while(knights){
        int sq = popLSB(&knights);
        U64 moves = knightAttacks[sq] & ~ownpieces;
        addMove_bitboard(cb, l, sq, moves, KNIGHT);
    }
}
//modifié
moveList* generateKingMoves(chessboard* cb, moveList*l)
{
    U64 w_p = whitePieces(cb);
    U64 b_p = blackPieces(cb);
    if(cb->turn == WHITE){
        int sqk = bitScanForward(cb->piece[WHITE][KING]);
        addMove_bitboard(cb, l , sqk, (kingAttacks[sqk] & ~w_p), KING);
        if(((cb->castle & 0b1000) == 8) && !(rectLookup[e1][h1] & (w_p | b_p))){
            addMove(l, e1, g1, KING, EMPTY, EMPTY, CASTLE);
        }
        if(((cb->castle & 0b0100) == 4) && !(rectLookup[a1][e1] & (w_p | b_p))){
            addMove(l, e1, c1, KING, EMPTY, EMPTY, CASTLE);
        }
        
    }
    else if(cb->turn == BLACK){
        int sqk = bitScanForward(cb->piece[BLACK][KING]);
        addMove_bitboard(cb, l , sqk, (kingAttacks[sqk] & ~b_p), KING);
        if(((cb->castle & 0b0010) == 2) && !(rectLookup[e8][h8] & (w_p | b_p))){
            addMove(l, e8, g8, KING, EMPTY, EMPTY, CASTLE);
        }
        if(((cb->castle & 0b0001) == 1) && !(rectLookup[a8][e8] & (w_p | b_p))){
            addMove(l, e8, c8, KING, EMPTY, EMPTY, CASTLE);
        }
    }
}


//modifié
moveList* generateMoves(chessboard* cb, moveList* l){


    //Cavaliers_________________________________________________________________________________________________

    generateKnightMoves(cb, l);

    //Sliding Pieces_____________________________________________________________________________________________

    generateSlidingMoves(cb, l);

    //King________________________________________________________________________________________________________

    generateKingMoves(cb, l);

    //Pions_____________________________________________________________________________________________________

    generatePawnMoves(cb, l);
    

    return l;

}

void affiche_moveList(moveList* l){
    move current;
    int i = 0;
    printf("l->count  : %d \n", l->count);
    if(l->count == 0){
        printf("CHECKMATE CLAUSE \n");
    }
    while(i < l->count + 1){
        current = l->moves[i];
        if((l->moves[i].from == 0) && (l->moves[i].to == 0)){
            i++;
        }
        else{
            printf("[%d][%d][%d][%d][%d][%d]\n", current.from, current.to, current.piece, current.captured, current.promo, current.flag);
            i++;
        }
    }
}