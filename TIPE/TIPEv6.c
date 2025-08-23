// Auteur : Neji Nguyen
// Version : 9
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
    - SDL
    - En passant pour les pions (generatepawnmoves)
    - generation pour les pions noirs



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
            | ((kingsAttacks_sq(sq))               & kings          )
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

U64 rectLookup[64][64];


void fill_rectLookup_w_zeros(){
    for(int i = 0; i < 64 ; i++){
        for(int j = 0; j < 64; j++){
            rectLookup[i][j] = 0;
        }
    }    
}

bool edge_compare(int i){
    return     i != 0
            && i != 1
            && i != 2
            && i != 3
            && i != 4
            && i != 5
            && i != 6
            && i != 7
            && i != 8
            && i != 16
            && i != 24
            && i != 32
            && i != 40
            && i != 48
            && i != 56
            && i != 57
            && i != 58
            && i != 59
            && i != 60
            && i != 61
            && i != 62
            && i != 63
            && i != 55
            && i != 47
            && i != 39
            && i != 31
            && i != 23
            && i != 15;
}


void fill_rectLookup(){ //Remplissage de la table de InBetween
    fill_rectLookup_w_zeros();
    for(int i = 0; i < 64 ; i++){

        //printf("Valeur de i :%d\n", i);

        U64 startingpoint_i = serialize(i); //case de départ
        U64 pivot = startingpoint_i<<1; //pivot remplissant la table
        U64 stopper = pivot; //condition d'arrêt pour les diagonales



        int j = i+2; //On se décale pour éviter les cas des pièces collées
        while(j < 8*(i/8) + 8){//vers la droite horizontale
            rectLookup[i][j] = pivot;
            //printf("j : %d\n",j);
            //print_mask(pivot);
            pivot |= shift_east(pivot);
            j++;
        }

        pivot = startingpoint_i>>1;
        int k = i - 2;
        while(k > 8*(i/8)-1){//vers la gauche horizontale
            rectLookup[i][k] = pivot;
            //printf("k : %d\n",k);
            //print_mask(pivot);
            pivot |= shift_west(pivot);
            k--;
        }

        pivot = startingpoint_i << 8;
        int l = i + 16; //Décalage
        while( l < 65 ){ //vers le haut
            rectLookup[i][l] = pivot;
            //printf("l : %d\n",l);
            //print_mask(pivot);
            pivot |= shift_north(pivot);
            l += 8;
        }

        pivot = startingpoint_i >> 8;
        int m = i - 16 ; //Décalage
        while( m > - 1 ){ //vers le bas
            rectLookup[i][m] = pivot;
            //printf("m : %d\n",m);
            //print_mask(pivot);
            pivot |= shift_south(pivot);
            m -= 8;
        }

        pivot = startingpoint_i << 9;
        stopper = pivot;
        int n = i + 18;
        while(bitScanForward(stopper<<9) - bitScanForward(stopper) == 9){ //diag ascendante haut -> on compare les distances dans le LEFRM pour savoir quand on atteint bord
            rectLookup[i][n] = pivot;
            //printf("n : %d\n",n);
            //print_mask(pivot);
            pivot |= shift_northeast(pivot);
            stopper <<= 9;
            n += 9;
        }

        pivot = startingpoint_i >> 9;
        stopper = pivot;
        int p = i - 18;
        while(bitScanForward(stopper) - bitScanForward(stopper>>9) == 9){ //diag ascendante bas
            rectLookup[i][p] = pivot;
            //printf("p : %d\n",p);
            //print_mask(pivot);
            pivot |= shift_southwest(pivot);
            stopper >>= 9;
            p -= 9;
        }

        pivot = startingpoint_i << 7;
        stopper = pivot;
        int q = i + 14;
        while(bitScanForward(stopper<<7) - bitScanForward(stopper) == 7 && q != 63){ //diag descendante haut
            rectLookup[i][q] = pivot;
            //printf("q : %d\n",q);
            //print_mask(pivot);
            pivot |= shift_northwest(pivot);
            stopper <<= 7;
            q += 7;
        }

        pivot = startingpoint_i >> 7;
        int r = i - 14;
        while(bitScanForward(stopper) - bitScanForward(stopper>>7) == 7 && r != 0){ //diag descendante bas
            rectLookup[i][r] = pivot;
            //printf("r : %d\n",r);
            //print_mask(pivot);
            pivot |= shift_southeast(pivot);
            stopper >>=7;
            r -= 7;
        }
    }
}


//Preconditions: avoir remplit l'array de zeros
U64 InBetween(int from, int to){ //from et to sont les cases sérialisées
    return rectLookup[from][to];
}

bool mayMove(int from, int to, U64 occ){
    return (InBetween(from, to) & occ) == 0; //le cas de true correspond à aucune pieces sur le chemin
}


U64 xrayRookAttacks(U64 occ, U64 blockers, int sq){
    U64 r_att = rookAttacks(occ, sq);                //Récupération des attaques de la tour
    blockers &= r_att;                               
    return r_att ^ rookAttacks(occ ^ blockers, sq);   
}

U64 xrayBishopAttacks(U64 occ, U64 blockers, int sq){
    U64 b_att = bishopAttacks(occ, sq);                //Récupération des attaques de la tour
    blockers &= b_att;                               
    return b_att ^ bishopAttacks(occ ^ blockers, sq);   
}


//---------------------------------------- CASTLE ------------------------------------------------------------

void castle_left_w(chessboard *cb){
    printf("castle left\n");
    if((rectLookup[bitScanForward(cb->whiteKing)][bitScanForward(cb->whiteRooks)] & takenSquares(cb)) == 0){
        cb->whiteKing   = LS1B(cb->whiteRooks) << 1;
        cb->whiteRooks  = cb->whiteRooks - LS1B(cb->whiteRooks);
        cb->whiteRooks += cb->whiteKing << 1;
        printf("castle succesful\n");
    }
    else{
        printf("Something is in the way !\n");
    }

}

void castle_right_w(chessboard *cb){
    if((rectLookup[bitScanForward(cb->whiteKing)][bitScanForward(cb->whiteRooks - LS1B(cb->whiteRooks))] & takenSquares(cb)) == 0){
        cb->whiteKing = (cb->whiteRooks - LS1B(cb->whiteRooks)) >> 1;
        cb->whiteRooks = LS1B(cb->whiteRooks);
        cb->whiteRooks += cb->whiteKing >> 1;
        printf("castle succesful\n");
    }
    else{
        printf("Something is in the way !\n");
    }

}

void castle_left_b(chessboard *cb){
    printf("castle left\n");
    if((rectLookup[bitScanForward(cb->blackKing)][bitScanForward(cb->blackRooks)] & takenSquares(cb)) == 0){
        cb->blackKing   = LS1B(cb->blackRooks) << 1;
        cb->blackRooks  = cb->blackRooks - LS1B(cb->blackRooks);
        cb->blackRooks += cb->blackKing << 1;
        printf("castle succesful\n");
    }
    else{
        printf("Something is in the way !\n");
    }

}

void castle_right_b(chessboard *cb){
    if((rectLookup[bitScanForward(cb->blackKing)][bitScanForward(cb->blackRooks - LS1B(cb->blackRooks))] & takenSquares(cb)) == 0){
        cb->blackKing = (cb->blackRooks - LS1B(cb->blackRooks)) >> 1;
        cb->blackRooks = LS1B(cb->blackRooks);
        cb->blackRooks += cb->blackKing >> 1;
        printf("castle succesful\n");
    }
    else{
        printf("Something is in the way !\n");
    }

}


void affichage_rect(){
    for(int i = 0 ; i < 64 ; i++){
        for(int j = 0 ; j < 64 ; j++){
            char* stringIter = fprintf_mask(rectLookup[i][j]);

          FILE *pF = fopen("./rect.txt", "a");
          fprintf(pF, "\n");
          fprintf(pF, "i : %d \t j : %d\n", i, j);
          
          int k = 57;
          for(int m = 8; m >= 1; m--){
            for(i ; k <= 8*m; k++){ //On itere sur les bits
              fprintf(pF, "%d", stringIter[k]);
            }
            fprintf(pF, "\n");
            k -= 16;
          }

          fclose(pF);
        }
    }
}

///////////////////////////////////////////Precomputing///////////////////////////////////////////////////////////

U64 knightAttacks[64];
U64 kingAttacks[64];
U64 pawnAttacks[2][64]; //0 -> white
                        //1 -> black

void fill_knightAttacks_table(){
    U64 iter = 1;
    for(int i = 0; i < 64 ; i++){
        knightAttacks[i] = nAttacks(iter);
        //printf("i : %d\n", i);
        //print_mask(knightAttacks[i]); AFFICHAGE TEST
        iter <<=1;
    }
}

void fill_king_Attacks_table(){
    for(int i = 0; i < 64 ; i++){
        kingAttacks[i] = kingsAttacks_sq(i);
        //printf("i : %d\n", i);
        //print_mask(kingAttacks[i]);

    }
}

void fill_pawnAttacks_table(){

    U64 iter = 1;
    for(int i = 0 ; i < 64 ; i++){
        pawnAttacks[0][i] = wp_attacks_all(iter);
        pawnAttacks[1][i] = bp_attacks_all(iter);
        //printf("i : %d\n", i);
        //print_mask(pawnAttacks[0][i]);
        //print_mask(pawnAttacks[1][i]);

        iter <<=1;
    }
}

//////////////////////////////////////////////////// MOVE GENERATION //////////////////////////////////////////

int what_black_p(chessboard* cb, U64 bit){
    if(cb->blackPawns & bit != 0){
        return PAWN;
    }
    else if (cb->blackKnights & bit != 0){
        return KNIGHT;
    }
    else if (cb->blackRooks & bit != 0){
        return ROOK;
    }
    else if (cb->blackBishops & bit != 0){
        return BISHOP;
    }
    else if (cb->blackQueens & bit != 0){
        return QUEEN;
    }
    else{
        return EMPTY;
    }
    

}

void addMove(moveList* list, int from, int to, int piece, int captured, int promo){
    move m = {to, from, piece, captured, promo};
    list->moves[list->count + 1] = m;
    list->count++;
}


moveList* generatePawnMoves_w(chessboard* cb, moveList* list){

    U64 pawns =  cb->whitePawns;   // les cases occupées par les pions
    U64 empty = ~takenSquares(cb); // les cases libres
    U64 b_pieces = blackPieces(cb);

    //Simple pawn push________________________________________________________________

    U64 singlepush = (pawns << 8) & empty;
    U64 quietmoves = singlepush & ~row8;
    U64 promotions = singlepush &  row8;

    while(quietmoves){ //on s'arrête quand quietmove est égal à 0
        int to = popLSB(&quietmoves);
        int from = to - 8;
        addMove(list, from, to, PAWN, EMPTY, EMPTY);
    }

    while(promotions){
        int to   = popLSB(&promotions);
        int from = to - 8;
        addMove(list, from, to, PAWN, EMPTY, QUEEN);
        addMove(list, from, to, PAWN, EMPTY, ROOK);
        addMove(list, from, to, PAWN, EMPTY, KNIGHT);
        addMove(list, from, to, PAWN, EMPTY, BISHOP);
    }

    //Double pawn push_______________________________________________________________

    U64 doublepush = ((((pawns << 8) & empty) << 8) & empty) & row4; //c'est pas beau, mais ça fait l'affaire
    while(doublepush != 0){
        int to   = popLSB(&doublepush);
        int from = to - 16;
        addMove(list, from, to, PAWN, EMPTY, EMPTY);
    }

    //Left capture___________________________________________________________________

    U64 left_cap = (pawns << 7) & b_pieces & ~Hfile;
    while(left_cap){
        int to = popLSB(&left_cap);
        int from = to -7;
        int capturedPiece = what_black_p(cb, serialize(to));
        if(to >= 56){
            addMove(list, from, to, PAWN, capturedPiece, QUEEN);
            addMove(list, from, to, PAWN, capturedPiece, ROOK);
            addMove(list, from, to, PAWN, capturedPiece, KNIGHT);
            addMove(list, from, to, PAWN, capturedPiece, BISHOP);

        }
        else{
            addMove(list, from, to, PAWN, capturedPiece, EMPTY);
        }
    }

    //Right capture___________________________________________________________________

    U64 right_cap = (pawns << 9) & b_pieces & ~Afile;
    while(right_cap){
        int to = popLSB(&right_cap);
        int from = to -9;
        int capturedPiece = what_black_p(cb, serialize(to));
        if(to >= 56){
            addMove(list, from, to, PAWN, capturedPiece, QUEEN);
            addMove(list, from, to, PAWN, capturedPiece, ROOK);
            addMove(list, from, to, PAWN, capturedPiece, KNIGHT);
            addMove(list, from, to, PAWN, capturedPiece, BISHOP);

        }
        else{
            addMove(list, from, to, PAWN, capturedPiece, EMPTY);
        }
    }

    //AJOUTER LE EN PASSANT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



    return list;
}








 
//////////////////////////////////////////////////// MAIN /////////////////////////////////////////////////////

int main(int argc, char* argv[]){

    fill_tables(); //rooks and bishops
    fill_rectLookup();
    fill_king_Attacks_table();
    fill_knightAttacks_table();
    fill_pawnAttacks_table();
    printf("\n");


    

    /*--------------------------------------------------------------TEST POUR LE JEU INGAME
    chessboard* og = makeChessboard_og();
    U64 start = serialize(9);
    U64 end   = serialize(18);

    print_chessboard_white(og);

    og->whitePawns ^= start;
    og->whitePawns |= end;

    print_chessboard_white(og);

    */




    char FEN_og[92] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR ";

    char FEN[92] = "r1bq1rk1/pp2ppbp/2np1np1/8/3NP3/2N1BP2/PPPQ2PP/2KR1B1R " ;

    char FEN2[92] = "rnb1kb1r/pp3ppp/1n2p3/2pPP3/2P5/2N2NP1/PP5P/R1BqKB1R w KQkq - 0 11" ;

    //char FEN2[92] = "k7/8/8/2q1q3/1q3q2/3K4/1q3q2/2n1q3 w - - 0 1";

    chessboard* cb = convert_FEN_to_cb(FEN2);

    moveList* l = malloc(sizeof(moveList));
    generatePawnMoves_w(cb , l);

    printf("count : %d\n", l->count);


    print_chessboard_white(cb);









    
    return 0;
}

//gcc chess_implementation.c -o chess_implementation.exe && ./chess_implementation.exe
//./compile && ./TIPEv6.exe