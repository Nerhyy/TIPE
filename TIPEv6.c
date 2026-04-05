//PIECE ADAPTED

// Auteur : Neji Nguyen
// Version : 11
// Date : 8 Janvier 2026
// Objectif : Faire une implementation du jeu d'echec en C


// TO DO LIST //////////////////////////////////////////////////////////////////////////////////////////////

/*

    - Factoriser les fonctions colonnes par colonnes des heuristiques
    - debugger la quiescence
    - valeur du mate


*/

///////////////////////////////////////////////// MODULES //////////////////////////////////////////////////

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
#include "perft.h"
#include "tt.h"
#include "zobrist.h"


int piece_eval(U64 piece[2][7]);
int mobility(U64 piece[2][7]);
int pawn_eval(U64 piece[2][7]);
int other_eval(U64 piece[2][7]);
int castling_eval(chessboard* cb);


typedef uint64_t U64;

U64 arrFrontSpans[2][64];

void fill_arrFrontSpans(){




    U64 maskIter = 1;
 
    for(int i = 0; i < 64 ; i++){ //

        if(i < 56){ //Pour les blancs
            //printf("maskUp au départ %d : \n", i);
            U64 maskUp = maskIter << 8;
            //print_mask(maskUp);

            //printf("maskUp apres %d : \n", i);
            maskUp = maskUp | ((maskUp<<1) & ~Afile);
            maskUp = maskUp | ((maskUp>>1) & ~Hfile);
            //print_mask(maskUp);

            arrFrontSpans[WHITE][i] = maskUp;

            for(int j = 0 ; j < ((8 - (i/8)) -2) ; j++){
                arrFrontSpans[WHITE][i] |= arrFrontSpans[WHITE][i]<<8;
            }
        }
        else{
           arrFrontSpans[WHITE][i] = 0; 
        }
        if(i > 7){ //Pour les noirs
            U64 maskDown = maskIter >> 8;
            maskDown = maskDown | ((maskDown<<1) & ~Afile);
            maskDown = maskDown | ((maskDown>>1) & ~Hfile);

            arrFrontSpans[BLACK][i] = maskDown;

            for(int j = 0 ; j < i/8 -1 ; j++){
                arrFrontSpans[BLACK][i] |= arrFrontSpans[BLACK][i]>>8;
            }
        }
        else{
           arrFrontSpans[BLACK][i] = 0; 
        }

        maskIter <<=1;

        
    }
}


int eval(chessboard* cb){

    int score = 0;
   
    score += piece_eval(cb->piece);
    score += mobility(cb->piece);
    score += pawn_eval(cb->piece);
    score += other_eval(cb->piece);

    //score += castling_eval(cb); //A AJOUTER

    return (cb->turn == WHITE) ? score : -score ;

}

int castling_eval(chessboard* cb){

    int score =0;
    if(((cb->castle & 0x0001) == 0) && ((rectLookup[d8][g8] && cb->piece[BLACK][KING]) != 0)){
        score += 20;
    }
    if(((cb->castle & 0x0010) == 0) && ((rectLookup[f8][b8] && cb->piece[BLACK][KING]) != 0)){
        score += 20;
    }
    if(((cb->castle & 0x0100) == 0) && ((rectLookup[d1][g1] && cb->piece[WHITE][KING]) != 0) ){
        score -= 20;
    }
    if(((cb->castle & 0x1000) == 0) && ((rectLookup[f1][b1] && cb->piece[WHITE][KING]) != 0) ){
        score -= 20;
    }

    return score;
}



//CONSTANTE DE PLACEMENT POUR LES BLANCS___________________________________________________


const int w_pawn_place[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50, 
    10, 10, 20, 30, 30, 20, 10, 10, 
     5, 10, 10, 25, 25, 10, 10,  5, 
     0,  0,  0, 25, 25,  0,  0,  0, 
     5, -5, 10,  0,  0, 10, -5,  5, 
     5, 10, 10,-20,-20, 10, 10,  5, 
     0,  0,  0,  0,  0,  0,  0,  0
};

const int w_knight_place[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -40,  5, 10, 15, 15, 10,  5,-40,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};


const int w_bishop_place[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

const int w_rook_place[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
     5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -10,  0,  0,  0,  0,  0,  0, -10,
     0,  0,  0,  5,  5,  0,  0,  0
};

const int w_queen_place[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -5,  0,  5,  5,  5,  5,  0, -5,
     0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20

};

const int w_king_place[64] = {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    20, 20,  0,  0,  0,  0, 20, 20,
    20, 40, 10,  0,  0, 10, 40, 20
};

const int w_king_place_endgame[64] = {
    -50,-40,-30,-20,-20,-30,-40,-50,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -50,-30,-30,-30,-30,-30,-30,-50
};

//CONSTANTE DE PLACEMENT POUR LES NOIRS___________________________________________________

const int b_pawn_place[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
     5, 10, 10,-20,-20, 10, 10,  5, 
     5, -5, 10,  0,  0, 10, -5,  5, 
     0,  0,  0, 25, 25,  0,  0,  0, 
     5,  5, 10, 25, 25, 10,  5,  5,
    10, 10, 20, 30, 30, 20, 10, 10,
    50, 50, 50, 50, 50, 50, 50, 50,
     0,  0,  0,  0,  0,  0,  0,  0
};

const int b_knight_place[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -40,  5, 10, 15, 15, 10,  5,-40,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

const int b_bishop_place[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};


const int b_rook_place[64] = {
     0,  0,  0,  5,  5,  0,  0,  0,
    -10,  0,  0,  0,  0,  0,  0, -10,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
     5, 10, 10, 10, 10, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0,
};

const int b_queen_place[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -10,  5,  5,  5,  5,  5,  0,-10,
     0,  0,  5,  5,  5,  5,  0, -5,
    -5,  0,  5,  5,  5,  5,  0, -5,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20,
    
};

const int b_king_place[64] = {
    20, 40, 10,  0,  0, 10, 40, 20,
    20, 20,  0,  0,  0,  0, 20, 20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30
};

const int b_king_place_endgame[64] = {
    -50,-30,-30,-30,-30,-30,-30,-50,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -50,-40,-30,-20,-20,-30,-40,-50,
};

const int mvv_lva_table[7][7] = {
    {0, 0, 0, 0, 0, 0, 0}, // 0: EMPTY (Pas de capture)
    {0, 105, 104, 103, 102, 101, 100}, // 1: Pion capturé
    {0, 205, 204, 203, 202, 201, 200}, // 2: Cavalier capturé
    {0, 305, 304, 303, 302, 301, 300}, // 3: Fou capturé
    {0, 405, 404, 403, 402, 401, 400}, // 4: Tour capturée
    {0, 505, 504, 503, 502, 501, 500}, // 5: Reine capturée
    {0, 0, 0, 0, 0, 0, 0}              // 6: Roi (n'est techniquement jamais capturé)
};

const int* all_placements[2][8] = {
    // ================= BLANCS =================
    [WHITE] = {
        [0] = 0,
        [PAWN]         = w_pawn_place,
        [KNIGHT]       = w_knight_place,
        [BISHOP]       = w_bishop_place,
        [ROOK]         = w_rook_place,
        [QUEEN]        = w_queen_place,
        [KING]         = w_king_place,
        [7] = w_king_place_endgame, //KING ENDGAME
    },

    // ================= NOIRS ==================
    [BLACK] = {
        [0] = 0,
        [PAWN]         = b_pawn_place,
        [KNIGHT]       = b_knight_place,
        [BISHOP]       = b_bishop_place,
        [ROOK]         = b_rook_place,
        [QUEEN]        = b_queen_place,
        [KING]         = b_king_place,
        [7] = b_king_place_endgame, //KING ENDGAME
    }
};

const int piece_value[7] = {0,100,320,350,520,1000,200000}; //[EMPTY][PAWN][KNIGHT][BISHOP][ROOK][QUEEN][KING]

//score pour un camp sur la pos de départ : 204 085

int piece_eval(U64 piece[2][7]){

    int score = 0; //Score calculé

        //Heuristique matérielle pour les blancs
        for(int i = 1; i < 7 ; i++){   //On itère sur les pièces

            U64 tempbitboard = piece[WHITE][i];

            if(piece[WHITE][i] != 0){        //On check si le masque est vide (OPTIMISATION)

                while(tempbitboard){       //Tant que le mask iter est dans la range de l'echiquier
                    int sq = __builtin_ctzll(tempbitboard);
                    score += piece_value[i] + all_placements[WHITE][i][sq];
                    //printf("score [0][%d][%d] : %f", i, cur_sq ,score);
                    tempbitboard &= (tempbitboard - 1);
                }
                 
            }
    
        }

        

        //Heuristique matérielle pour les noirs
        for(int i = 1; i < 7 ; i++){   //On itère sur les pièces

            U64 tempbitboard = piece[BLACK][i];

            if(piece[BLACK][i] != 0){        //On check si le masque est vide (OPTIMISATION)

                while(tempbitboard){       //Tant que le mask iter est dans la range de l'echiquier

                    int sq = __builtin_ctzll(tempbitboard);
                    score -= piece_value[i] + all_placements[BLACK][i][sq];
                    //printf("score [1][%d][%d] : %f", i, cur_sq ,score);
                    tempbitboard &= (tempbitboard - 1);

                }

            }
    
        }

    return score;

}

int double_pawns(U64 piece[2][7]){

    int score = 0;

    for(int i = 0; i < 8; i++){
        int popcw = popCount(piece[WHITE][PAWN] & files[i]);
        int popcb = popCount(piece[BLACK][PAWN] & files[i]);

        if(popcw >= 2){

            if(popcw >= 3){ //Pions triplés ou plus
                score -= 60;
            }
            else{ //Pions doublés
                score -= 30;
            }

        }

        if(popcb >= 2){

            if(popcb >= 3){ //Pions triplés ou plus
                score += 60;
            }
            else{ //Pions doublés
                score += 30;
            }

        }
    }

    return score;

}

void open_files(U64 piece[2][7], bool* white_openfiles, bool* black_openfiles){ //Remplit les tableaux

    for(int i = 0; i < 8; i++){ //Il y a t il des pions sur les colonnes ?
        white_openfiles[i] = (piece[WHITE][PAWN] & files[i]) != 0;
        black_openfiles[i] = (piece[BLACK][PAWN] & files[i]) != 0;
    }

}

int isolated_pawns(U64 piece[2][7]){ //A FACTORISER AVEC LES ROOKS ON OPEN FILES

    int score = 0;

    bool istherepawnfileiw[8];
    bool istherepawnfileib[8];

    open_files(piece, istherepawnfileiw, istherepawnfileib);

    for(int i = 1 ; i < 7; i++){

        bool possede_voisin_w = false;
        bool possede_voisin_b = false;
        possede_voisin_w = istherepawnfileiw[i - 1] || istherepawnfileiw[i + 1];
        possede_voisin_b = istherepawnfileib[i - 1] || istherepawnfileib[i + 1];

        if(istherepawnfileiw[i] && !possede_voisin_w){
            score -= 15;
        }
        if(istherepawnfileib[i] && !possede_voisin_b){
            score += 15;
        }
    }
    if(istherepawnfileiw[0] && !istherepawnfileiw[1]){
        score -= 15;
    }
    if(istherepawnfileib[0] && !istherepawnfileiw[1]){
        score += 15;
    }
    if(istherepawnfileiw[7] && !istherepawnfileiw[6]){
        score -= 15;
    }
    if(istherepawnfileib[7] && !istherepawnfileiw[6]){
        score += 15;
    }
    return score;

}

int bishop_pair(U64 piece[2][7]){

    int score = 0;

    if (popCount(piece[WHITE][BISHOP]) == 2){
        score += 50;
    }
    if(popCount(piece[BLACK][BISHOP] == 2)){
        score -= 50;
    }

    return score;

}

int rook_on_open_files(U64 piece[2][7]){

    int score = 0;

    bool istherepawnfileiw[8];
    bool istherepawnfileib[8];

    open_files(piece, istherepawnfileiw, istherepawnfileib); //Je pense on peut s'en passer

    for(int i = 0; i < 8; i++){
        if(((files[i] & piece[WHITE][ROOK]) != 0) && (!(istherepawnfileiw[i]))){ //Il y a une tour sur la colonne, et il y n'a pas de pions sur la colonnes (du camps)
            if(!istherepawnfileib[i]){ //y'a aucun pion sur la colonne
                score += 20;
            } 
            else{
                score += 10;
            }
        }
        if(((files[i] & piece[BLACK][ROOK]) != 0) && (!(istherepawnfileiw[i] & istherepawnfileib[i]))){ //Il y a une tour sur la colonne, et il y n'a pas de pions sur la colonnes (du camps)
            if(!istherepawnfileiw[i]){ //y'a aucun pion sur la colonne
                score -= 20;
            }
            else{
                score -= 10;
            }
        }
    }

    return score;

}

int bastion(U64 piece[2][7]){ //Si un cavalier ou bishop est protégé par un pion loin dans le camps adverse

    int score = 0;

    U64 wknights = piece[WHITE][KNIGHT] & (row5 | row6 | row7); //On prend les knights sur la 5eme et 6 rangée
    U64 bknights = piece[BLACK][KNIGHT] & (row4 | row3 | row2); //On prend les knights sur la 5eme et 6 rangée

    while(wknights){
        int sq = popLSB(&wknights);

        if((pawnAttacks[BLACK][sq] & piece[WHITE][PAWN]) != 0) { //On regarde si des pions protège le cavalier (bastion)
            if((arrFrontSpans[WHITE][sq] & piece[BLACK][PAWN]) == 0){ //Il y a t il un pion en face du bastion ?
                score += 40; //A REDEFINIR
            }
            else{
                score+= 15;
            }
        }
    }

    while(bknights){
        int sq = popLSB(&bknights);


        if((pawnAttacks[WHITE][sq] & piece[BLACK][PAWN]) != 0) { //On regarde si des pions protège le cavalier (bastion)
            if((arrFrontSpans[BLACK][sq] & piece[WHITE][PAWN]) == 0){ //Il y a t il un pion en face du bastion ?
                score -= 40; //A REDEFINIR
            }
            else{
                score -= 15;
            }
        }
    }

    return score;
}


int passed_pawns(U64 piece[2][7]){ // A FACTORISER

    int score = 0;

    U64 wpawns = piece[WHITE][PAWN];
    U64 bpawns = piece[BLACK][PAWN];

    while(wpawns){
        int sq = __builtin_ctzll(wpawns);
        if((arrFrontSpans[WHITE][sq] & piece[BLACK][PAWN]) == 0){
            score += 15; //A MODIFIER
        }
        wpawns &= (wpawns -1);
    }

    while(bpawns){
        int sq = __builtin_ctzll(bpawns);
        if((arrFrontSpans[BLACK][sq] & piece[WHITE][PAWN]) == 0){
            score -= 15; //A MODIFIER
        }
        bpawns &= (bpawns -1);
    }


    return score;
}




int rook_on_7th(U64 piece[2][7]){

    int score = 0;

    if(popCount(piece[WHITE][ROOK] & row7) == 2){ //Si deux tours sont sur la 7eme rangée
        score +=10;
        if((piece[BLACK][KING] & row8) != 0){ //Si le roi est sur la 8eme rangée
            score += 15;
        }
    }

    if(popCount(piece[BLACK][ROOK] & row2) == 2){ //Si deux tours sont sur la 2eme rangée
        score -=10;
        if((piece[WHITE][KING] & row1) != 0){ //Si le roi est sur la 8eme rangée
            score -= 15;
        }
    }

    return score;

}

int doubled_rooks(U64 piece[2][7]){
    
    int score= 0;
    
    for(int i = 0; i < 8; i++){
        if(popCount(piece[WHITE][ROOK] & files[i]) >= 2){
            score+= 15;
        }
        if(popCount(piece[WHITE][ROOK] & files[i]) >= 2){
            score-= 15;
        }
    }

    return score;

}

int mobility (U64 piece[2][7]){

    int score = 0;
    U64 w_p_attacks = wp_attacks_all(piece[WHITE][PAWN]);
    U64 b_p_attacks = bp_attacks_all(piece[BLACK][PAWN]);

    U64 all_w_pieces = whitePieces_p(piece);
    U64 all_b_pieces = whitePieces_p(piece);

    U64 occ = all_w_pieces | all_b_pieces;

    U64 wknights = piece[WHITE][KNIGHT] ;
    U64 wrooks   = piece[WHITE][ROOK]   ;
    U64 wbishops = piece[WHITE][BISHOP] ;

    U64 bknights = piece[BLACK][KNIGHT] ;
    U64 brooks   = piece[BLACK][ROOK]   ;
    U64 bbishops = piece[BLACK][BISHOP] ;

    while(wknights){

        int sq = popLSB(&wknights);

        U64 moves = knightAttacks[sq];

        score += 4*popCount((moves & (~b_p_attacks)) & (~all_w_pieces));
        
    }
    while(wrooks){

        int sq = popLSB(&wrooks);

        U64 moves = rookAttacks(occ, sq);

        score += 2*popCount((moves & (~b_p_attacks)) & (~all_w_pieces));

    }
    while(wbishops){

        int sq = popLSB(&wbishops);

        U64 moves = bishopAttacks(occ, sq);

        score += 3*popCount((moves & (~b_p_attacks)) & (~all_w_pieces));

    }
    while(bknights){

        int sq = popLSB(&bknights);

        U64 moves = knightAttacks[sq];

        score -= 4*popCount((moves & (~w_p_attacks)) & (~all_b_pieces));

    }
    while(brooks){

        int sq = popLSB(&brooks);

        U64 moves = rookAttacks(occ, sq);

        score -= 2*popCount((moves & (~w_p_attacks)) & (~all_b_pieces));

    }
    while(bbishops){

        int sq = popLSB(&bbishops);

        U64 moves = bishopAttacks(occ, sq);

        score -= 3*popCount((moves & (~w_p_attacks)) & (~all_b_pieces));

    }

    return score;

}


int pawn_chain(U64 piece[2][7]){
    int score = 0;

    U64 w_defended_west = w_p_defended_from_west(piece[WHITE][PAWN]);
    U64 w_defended_east = w_p_defended_from_east(piece[WHITE][PAWN]);
    U64 b_defended_west = b_p_defended_from_west(piece[BLACK][PAWN]);
    U64 b_defended_east = b_p_defended_from_east(piece[BLACK][PAWN]);

    U64 w_defenders_west = w_p_defenders_from_west(piece[WHITE][PAWN]);
    U64 w_defenders_east = w_p_defenders_from_east(piece[WHITE][PAWN]);
    U64 b_defenders_west = b_p_defenders_from_west(piece[BLACK][PAWN]);
    U64 b_defenders_east = b_p_defenders_from_east(piece[BLACK][PAWN]);

    score += 25*popCount(defended_defenders_west(w_defended_west, w_defenders_west) || defended_defenders_east(w_defended_east,w_defenders_east));
    score -= 25*popCount(defended_defenders_west(b_defended_west, b_defenders_west) || defended_defenders_east(b_defended_east,b_defenders_east));

    score += 17*popCount(defended_ndefenders_west(w_defended_west, w_defenders_west) || defended_ndefenders_east(w_defended_east, w_defenders_east));
    score -= 17*popCount(defended_ndefenders_west(b_defended_west, b_defenders_west) || defended_ndefenders_east(b_defended_east, b_defenders_east));

    score += 10*popCount(ndefended_defenders_west(w_defended_west, w_defenders_west) || ndefended_defenders_east(w_defended_east, w_defenders_east));
    score -= 10*popCount(ndefended_defenders_west(b_defended_west, b_defenders_west) || ndefended_defenders_east(b_defended_east, b_defenders_east));

    return score;
}

int pawn_eval(U64 piece[2][7]){
    int score = 0;
    score += isolated_pawns(piece);
    score += double_pawns(piece);
    score += passed_pawns(piece);
    score += pawn_chain(piece);
    
    return score;
}

int other_eval(U64 piece[2][7]){

    int score =0;

    score += doubled_rooks(piece);
    score += rook_on_7th(piece);
    score += bastion(piece);
    score += rook_on_open_files(piece);
    score += bishop_pair(piece);

    return score;
}

int quiescence(chessboard* cb, int alpha, int beta){
    int stand_pat = eval(cb);
    if(stand_pat >= beta){
        return beta;
    }
    if(alpha < stand_pat){
        alpha = stand_pat;
    }

    moveList l;
    l.count = 0;
    
    generateMoves(cb, &l);

    int move_scores[256] = {0};

    for (int i = 0; i < l.count; i++) {
        if (l.moves[i].captured != EMPTY) {
            move_scores[i] = mvv_lva_table[l.moves[i].captured][l.moves[i].piece];
        } 
        else if (l.moves[i].promo != EMPTY) {
            move_scores[i] = 900; 
        } 
        else {
            move_scores[i] = 0; 
        }
    }

    ld lostdata = create_lostdata2();

    for (int i = 0; i < l.count; i++) {

        // --- ÉTAPE DE TRI ---
        int best_score_idx = i;
        for (int j = i + 1; j < l.count; j++) {
            if (move_scores[j] > move_scores[best_score_idx]) {
                best_score_idx = j;
            }
        }

       
        // on arrête complètement d'explorer les nœuds pour cette position 
        if (move_scores[best_score_idx] == 0) {
            break; 
        }

        move temp_move = l.moves[i];
        l.moves[i] = l.moves[best_score_idx];
        l.moves[best_score_idx] = temp_move;
        
        int temp_score = move_scores[i];
        move_scores[i] = move_scores[best_score_idx];
        move_scores[best_score_idx] = temp_score;

        makeMove_ld(cb, l.moves[i], &lostdata);

        if (!legalmove_check(cb, l.moves[i])) {
            unmakeMove(cb, l.moves[i], &lostdata);
            continue; // Coup pseudo-légal mais qui met/laisse le roi en échec
        }

        int score = -quiescence(cb, -beta, -alpha);
        
        unmakeMove(cb, l.moves[i], &lostdata);

        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
        }
    }
    
    return alpha;
}

int negaMax(chessboard* cb, int depth, int alpha, int beta){

    int originalAlpha = alpha;

    int tt_score;
    move tt_best_move = {0};
    if (probe_tt(cb->hash, depth, alpha, beta, &tt_score, &tt_best_move)) {
        //count++;
        return tt_score;
    }
    
    if(depth == 0){
        return quiescence(cb, alpha, beta);
    }
    
    int max = -100000000;
    moveList l;
    l.count = 0;
    generateMoves(cb, &l);


    int move_scores[256] = {0};
    //On met un score à chaque move (pour le tri)
    for (int i = 0; i < l.count; i++) {
        if (tt_best_move.piece != 0 && l.moves[i].from == tt_best_move.from && l.moves[i].to == tt_best_move.to) {
            move_scores[i] = 1000000; 
        } 
        else if (l.moves[i].captured != EMPTY) {
            move_scores[i] = mvv_lva_table[l.moves[i].captured][l.moves[i].piece];
        } 
        else if (l.moves[i].promo != EMPTY) {
            move_scores[i] = 900; 
        } 
        else {
            move_scores[i] = 0; 
        }
    }

    ld lostdata = create_lostdata2();
    move best_move_for_this_node = {0};
    int legal_moves_played = 0;

    for(int i = 0 ; i < l.count; i++){
        
        // --- ÉTAPE DE TRI ---
        int best_score_idx = i;
        for (int j = i + 1; j < l.count; j++) {
            if (move_scores[j] > move_scores[best_score_idx]) {
                best_score_idx = j;
            }
        }
        
        move temp_move = l.moves[i];
        l.moves[i] = l.moves[best_score_idx];
        l.moves[best_score_idx] = temp_move;
        
        int temp_score = move_scores[i];
        move_scores[i] = move_scores[best_score_idx];
        move_scores[best_score_idx] = temp_score;

        makeMove_ld(cb, l.moves[i], &lostdata);
        if(!legalmove_check(cb, l.moves[i])){
            unmakeMove(cb, l.moves[i], &lostdata);
            continue;
        }

        legal_moves_played++;

        // Calcul en profondeur (NegaMax)
        int score = -negaMax(cb, depth - 1, -beta, -alpha); 
        unmakeMove(cb, l.moves[i], &lostdata);

        if(score > max){
            max = score;
            best_move_for_this_node = l.moves[i];
        }

        if(score > alpha){
            alpha = score;
        }

        if(alpha >= beta){
            break; 
        }
    }


    if (legal_moves_played == 0) {
        if (is_square_attacked(cb, deserialize(cb->piece[cb->turn][KING]), cb->turn)) {
            return -10000 + (100 - depth); // MAT
        } else {
            return 0; // PAT
        }
    }

    // Sauvegarde dans la Table de Transposition
    store_tt(cb->hash, depth, max, originalAlpha, beta, best_move_for_this_node);

    return max;
}



move findBestMove(chessboard* cb, int depth){

    move bestMove = {0,0,0,0,0,0};
    int bestScore = -1000000000;

    int alpha = -1000000000;
    int beta  =  1000000000;

    moveList* l = legalMoveList(cb);
    ld lostdata = {.castle = -1, .enPassantSquare = -1, .fullmove = -1, .halfmoveclock = -1};
    int n_moves = l->count;
    //Recherche en profondeur du meilleur coup
    for(int i = 0 ; i < n_moves; i++){

        makeMove_ld(cb, l->moves[i] , &lostdata);
        //Calcul en profondeur
        int score = -negaMax(cb , depth - 1, -beta, -alpha);

        unmakeMove(cb, l->moves[i] , &lostdata);

        //Attribution du meilleur score
        if(score > bestScore){
            bestScore = score;
            bestMove = l->moves[i];
        }

        if( score > alpha){
            alpha = score;
        }

    }

    free_moveList(l);

    return bestMove;

}


char FENcaptureQueen[92] = "rnbqkbnr/3p1ppp/2p1B3/1p2P2Q/p7/2N5/PPPP1PPP/R1B1K1NR b KQkq - 0 8" ;




/*
Pour le popcount : 1000000000 ->
real : 2.599s
user : 2.592s
sys  : 0.006s



*/

void popCountLSBTest(){
    char FEN[92] = "r1bq1rk1/1p2ppbp/p1np1np1/8/3NP3/2N1BP2/PPPQ2PP/2KR1B1R w - - 0 10";



    
    chessboard* cb = convert_FEN_to_cb(FEN);
    U64 all_pieces = takenSquares(cb);

    U64 iter = all_pieces;
    int junk = 0;

    
    for(int i = 0; i < 100000000 ; i++){
        if(iter == 0){
            iter = all_pieces;
        }
        junk = popLSB(&iter);
    }



    /*
    for(int i = 0; i < 1000000000 ; i++){
        popCount(all_pieces);
    }
    */
    

}


move findBestMove_IDS(chessboard* cb, int depth){

    move bestMove = {0,0,0,0,0,0};
    cb->hash = generate_hash(cb);

    moveList* l = legalMoveList(cb);
    ld lostdata = {.castle = -1, .enPassantSquare = -1, .fullmove = -1, .halfmoveclock = -1};
    int n_moves = l->count;
    //Recherche en profondeur du meilleur coup
    for(int j = 1; j <= depth; j++){
        
        int bestScore = -1000000000;
        int alpha = -1000000000;
        int beta  =  1000000000;
        move bestMoveForThisDepth = {0,0,0,0,0,0};

        for(int i = 0 ; i < n_moves; i++){
            move current_move = l->moves[i];
            
            makeMove_ld(cb,  current_move, &lostdata);
            //Calcul en profondeur
            int score = -negaMax(cb , j - 1, -beta, -alpha);

            unmakeMove(cb, current_move, &lostdata);
            
            //Attribution du meilleur score
            if(score > bestScore){
                bestScore = score;
                bestMoveForThisDepth = current_move;
            }

            if( score > alpha){
                alpha = score;
            }

        }
        bestMove = bestMoveForThisDepth;
    }
    free_moveList(l);
    return bestMove;

}


//////////////////////////////////////////////////// MAIN /////////////////////////////////////////////////////

int main(int argc, char* argv[]){

    fill_tables(); //rooks and bishops
    fill_rectLookup();
    fill_king_Attacks_table();
    fill_knightAttacks_table();
    fill_pawnAttacks_table();
    fill_arrFrontSpans();
    init_zobrist();
    init_tt();


    char FEN[92] ;
    bool running = true;
    while (running)
    {
        //printf("From chess engine: Waiting for a fen\n");
        fgets(FEN, sizeof(FEN), stdin);
        chessboard *cb = convert_FEN_to_cb(FEN);
        print_move(findBestMove_IDS(cb, 7));
        fflush(stdout);
        //running = false;
    }
    //printf("hit :%d\n" , count);
    return 0;
}

//./compile && ./TIPEv6.exe