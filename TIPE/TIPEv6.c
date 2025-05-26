// Auteur : Neji Nguyen
// Version : 8
// Date : 23 Mai 2025
// Objectif : Faire une implementation du jeu d'echec en C

///////////////////////////////////////////////// MODULES //////////////////////////////////////////////////

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "pawns.h"
#include "globals.h"
#include "shift.h"
#include "rooks_and_bishops.h"
#include "utils.h"
#include "cb_operators.h"

typedef uint64_t U64;


///////////////////////////////////////////////// To Do List ////////////////////////////////////////////////:

/*
    - Array des attaques des rois/pions/knights pour aller plus vite
    - SDL



*/
 
///////////////////////////////////////////////// PROTOTYPES //////////////////////////////////////////////////


//Knight moves (voir la rosas)
U64 nNoNoEa_target(U64 n);
U64 nNoNoWe_target(U64 n);
U64 nEaEaNo_target(U64 n);
U64 nEaEaSo_target(U64 n);
U64 nSoSoEa_target(U64 n);
U64 nSoSoWe_target(U64 n);
U64 nWeWeNo_target(U64 n);
U64 nWeWeSo_target(U64 n);




///////////////////////////////////////////// CORPS DES FONCTIONS /////////////////////////////////////////////


//Knight moves

//mask de test :
U64 knight_test_mask = 0b0000000000000000100000000000000000000000000000000000000000000000;

U64 nNoNoEa_target(U64 n){
    return (n >> 17) & ~Afile;
}

U64 nNoNoWe_target(U64 n){
    return (n >> 15) & ~Hfile;
}

U64 nEaEaNo_target(U64 n){
    return (n >> 10) & ~(Afile | Bfile);
}

U64 nEaEaSo_target(U64 n){
    return (n << 6) & ~(Afile | Bfile);
}

U64 nSoSoEa_target(U64 n){
    return (n << 15) & ~Afile;
}

U64 nSoSoWe_target(U64 n){
    return (n << 17) & ~Hfile;
}

U64 nWeWeNo_target(U64 n){
    return (n >> 6) & ~(Hfile | Gfile);
}

U64 nWeWeSo_target(U64 n){
    return (n << 10) & ~(Hfile | Gfile);
}

U64 nAttacks(U64 knights) {
    U64 l1 = (knights >> 1) & ~Hfile;
    U64 l2 = (knights >> 2) & ~(Hfile | Gfile);
    U64 r1 = (knights << 1) & ~Afile;
    U64 r2 = (knights << 2) & ~(Afile | Bfile);
    U64 h1 = l1 | r1;
    U64 h2 = l2 | r2;
    return (h1<<16) | (h1>>16) | (h2<<8) | (h2>>8);
}

U64 nfill (U64 knights){
    return (nAttacks(knights)) | knights;
}

//Routine pour trouver les pieces forkables
U64 n_fork_targets(U64 targets){//targets c'est le masque des pieces adverses
    U64 west, east, attak, forks;
    east   = shift_east (targets);
    west   = shift_west (targets);
    attak  =  east << 16;
    forks  = (west << 16) & attak;
    attak |=  west << 16;
    forks |= (east >> 16) & attak;
    attak |=  east >> 16;
    forks |= (west >> 16) & attak;
    attak |=  west >> 16;
    east   = shift_east (east);
    west   = shift_west (west);
    forks |= (east <<  8) & attak;
    attak |=  east <<  8;
    forks |= (west <<  8) & attak;
    attak |=  west <<  8;
    forks |= (east >>  8) & attak;
    attak |=  east >>  8;
    forks |= (west >>  8) & attak;
    return forks;
}

int n_dist2square(U64 knight, U64 square){
    int d = 0;
    while ( (knight & square) == 0){
        knight = nfill(knight);
        d++;
    }
    return d;
}

//---------------------------------------- King -------------------------------------------------------------

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

U64 all_attacks(chessboard *cb ,int sq){ //renvoie les attaques d'une super piece en partant d'une case
    U64 knights;
    U64 kings;
    U64 rookQueens;
    U64 bishopQueens;

    knights       = cb->whiteKnights  | cb->blackKnights;
    kings         = cb->whiteKing     | cb->blackKing;
    rookQueens    = cb->whiteQueens   | cb->blackQueens;
    bishopQueens  = rookQueens;
    rookQueens   |= cb->whiteRooks     | cb->blackRooks;
    bishopQueens |= cb->whiteBishops  | cb->blackBishops;

    return    (rookAttacks(takenSquares(cb) , sq)  & rookQueens     )
            | (bishopAttacks(takenSquares(cb), sq) & bishopQueens   )
            | ((kingsAttacks_sq(sq))                & kings          )
            | (nAttacks(serialize(sq))             & knights        )
            | (wp_attacks_all(serialize(sq))       & cb->whitePawns )
            | (bp_attacks_all(serialize(sq))       & cb->blackPawns )
            ;
}



U64 any_attack_by_side(chessboard *cb, int sq , bool color){ //le "bool color" correspond à blanc = 1, noir = 0
    if(color){
        return all_attacks(cb,sq) & (cb->whiteBishops | cb->whiteKing | cb->whiteKnights | cb->whitePawns | cb->whiteQueens | cb->whiteRooks);
    }
    else{
        return all_attacks(cb,sq) & (cb->blackBishops | cb->blackKing | cb->blackKnights | cb->blackPawns | cb->blackQueens | cb->blackRooks);
    }
}


U64 all_color_piece(chessboard* cb, bool color){
    if(color){
        return (cb->whiteBishops | cb->whiteKing | cb->whiteKnights | cb->whitePawns | cb->whiteQueens | cb->whiteRooks);
    }
    else{
        return (cb->blackBishops | cb->blackKing | cb->blackKnights | cb->blackPawns | cb->blackQueens | cb->blackRooks);
    }
}



bool is_square_attacked(chessboard *cb, int sq,bool color) { //on peut optimiser les serialize(sq)

   if(!color){ //Si piece noire attaquée
   if ((bp_attacks_all(serialize(sq)) & cb->whitePawns) != 0){
        return true;
   }
   if (((nAttacks(serialize(sq))) & cb->whiteKnights) != 0 ){
        return true;
   }
   if ((kingsAttacks_sq(serialize(sq)) & cb->whiteKing) != 0 ){
        return true;
   }
   if ( (bishopAttacks(takenSquares(cb), sq) & (cb->whiteQueens | cb->whiteBishops)) != 0 ){
        return true;
   }
   if ((rookAttacks(takenSquares(cb), sq) & (cb->whiteQueens | cb->whiteRooks)) != 0 ){
        return true;
   }
   return false;
   }


   else{ //si piece blanche est attaquée
   if ((wp_attacks_all(serialize(sq)) & cb->blackPawns) != 0){
        return true;
   }
      //print_mask(nAttacks(serialize(sq)) & knights);
   if ((nAttacks(serialize(sq)) & cb->blackKnights) != 0 ){
        return true;
   }
   if ((kingsAttacks_sq(serialize(sq)) & (cb->whiteKing|cb->blackKing)) != 0 ){
        return true;
   }
   if ((bishopAttacks(takenSquares(cb), sq) & (cb->blackQueens | cb->blackBishops)) != 0 ){
        return true;
   }
   if ((rookAttacks(takenSquares(cb), sq) & (cb->blackQueens | cb->blackRooks)) != 0 ){
        return true;
   }
   return false;
   }
}


/////////////////////////////////////////////// TESTS UNITAIRES ///////////////////////////////////////////////



 
//////////////////////////////////////////////////// MAIN /////////////////////////////////////////////////////

int main(int argc, char* argv[]){
    fill_tables();
    printf("\n");


    char FEN[92] = "r1bq1rk1/pp2ppbp/2np1np1/8/3NP3/2N1BP2/PPPQ2PP/2KR1B1R " ;

    chessboard* cb = convert_FEN_to_cb(FEN);


    print_chessboard_white(cb);

    //print_mask(any_attack_by_side(cb, 19, true));

    printf("bool : %d\n", is_square_attacked(cb, 24 , false));


    //print_piece_positions(cb);
    //print_chessboard_white(cb);

    //U64 occ = Efile |Cfile|Dfile| Gfile | Ffile | row4 | row6 ;
    //print_mask(rookAttacks(occ, 42));

    //print_mask(all_attacks((ogChessBoard), 1));








    
    return 0;
}

//gcc chess_implementation.c -o chess_implementation.exe && ./chess_implementation.exe
//gcc TIPE.c -o TIPE.exe && ./TIPE.exe