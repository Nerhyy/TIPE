//PIECE ADAPTED

#include "utils.h"

#include "globals.h"
#include "cb_operators.h"
#include "zobrist.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


void print_mask(U64 mask) {
    int i = 57;
    for(int j = 8; j >= 1; j--){
        //printf("valeur j : %d \n", j); //DEBUG
        for(i ; i <= 8*j; i++){ //On itere sur les bits
            //printf("valeur de i : %d \n", i); //DEBUG
            printf("%ld", mask >> (i-1) & 1); //Usage du bitshift
        }
        i -= 16;
        printf("\n");
        //printf("valeur de i (bis): %d \n", i); //DEBUG
    }
    printf("\n");
}


//Fonction affichant l'emplacement de chacun des types de pieces de l'echiquier
void print_piece_positions(chessboard* echiquier){


    printf("\n White Pieces : \n----------------------------------------------\n");

    printf("\n White Bishops : \n");
    print_mask(echiquier->piece[WHITE][BISHOP]);
    printf("\n White Pawns : \n");
    print_mask(echiquier->piece[WHITE][PAWN]);
    printf("\n White King : \n");
    print_mask(echiquier->piece[WHITE][KING]);
    printf("\n White Rooks : \n");
    print_mask(echiquier->piece[WHITE][ROOK]);
    printf("\n White Knights : \n");
    print_mask(echiquier->piece[WHITE][KNIGHT]);
    printf("\n White Queen : \n");
    print_mask(echiquier->piece[WHITE][QUEEN]);


    printf("\n Black Pieces : \n----------------------------------------------\n");

    printf("\n Black Bishops : \n");
    print_mask(echiquier->piece[BLACK][BISHOP]);
    printf("\n Black Pawns : \n");
    print_mask(echiquier->piece[BLACK][PAWN]);
    printf("\n Black King : \n");
    print_mask(echiquier->piece[BLACK][KING]);
    printf("\n Black Rooks : \n");
    print_mask(echiquier->piece[BLACK][ROOK]);
    printf("\n Black Knights : \n");
    print_mask(echiquier->piece[BLACK][KNIGHT]);
    printf("\n Black Queen : \n");
    print_mask(echiquier->piece[BLACK][QUEEN]);

}

//Fonction qui affiche un echiquier
void print_chessboard_white(chessboard* echiquier) {
    U64 maskIter = 0b000000100000000000000000000000000000000000000000000000000000000;
    //printf("Masque de départ\n"); //DEBUG
    //print_mask(maskIter); //DEBUG
    U64 taken = takenSquares(echiquier); //On recupere un masque des emplacements des pieces
    for(int j = 0 ; j < 8; j++){
        for(int i = 0; i < 8; i++){
        //printf("\n j : %d \n i : %d \n", j, i); //DEBUG
        //print_mask(maskIter); //DEBUG
        //printf("\n"); //DEBUG
        //printf("valeur j : %d \n", j); //DEBUG

            

        printf(" ");
        if((taken & maskIter) == voidmask){ //Evaluation paresseuse
            printf(" ");
        }
        else if((echiquier->piece[WHITE][PAWN] & maskIter) == maskIter){
            printf("♟");
        }
        else if((echiquier->piece[BLACK][PAWN] & maskIter) == maskIter){
            printf("♙");
        }
        else if((echiquier->piece[WHITE][ROOK] & maskIter) == maskIter){
            printf("♜");
        }
        else if((echiquier->piece[WHITE][BISHOP] & maskIter) == maskIter){
            printf("♝");
        }
        else if((echiquier->piece[WHITE][KNIGHT] & maskIter) == maskIter){
            printf("♞");
        }
        else if((echiquier->piece[BLACK][ROOK] & maskIter) == maskIter){
            printf("♖");
        }
        else if((echiquier->piece[BLACK][BISHOP] & maskIter) == maskIter){
            printf("♗");
        }
        else if((echiquier->piece[BLACK][KNIGHT] & maskIter) == maskIter){
            printf("♘");
        }
        else if((echiquier->piece[BLACK][KING] & maskIter) == maskIter){
            printf("♔");
        }
        else if((echiquier->piece[WHITE][KING] & maskIter) == maskIter){
            printf("♚");
        }
        else if((echiquier->piece[BLACK][QUEEN] & maskIter) == maskIter){
            printf("♕");
        }
        else if((echiquier->piece[WHITE][QUEEN] & maskIter) == maskIter){
            printf("♛");
        }
        //printf("valeur de i : %d \n", i); //DEBUG
        if (i != 7){ //Pour éviter de faire disparaitre le 1
            maskIter<<=1;
        }
        }
    //printf("-------------------------------------");
    //printf("\n Avant j : %d \n", j);
    //print_mask(maskIter);
    maskIter>>=15; //Retour vers la ligne en dessous
    //printf("\n Apres j : %d \n", j);
    //print_mask(maskIter);
    printf("\n");
    //printf("valeur de i (bis): %d \n", i); //DEBUG
    }
    printf("\n");
}

int en_passant_FEN(char FEN[92], int i){
    if(FEN[i] == '-'){
        return (-1);
    }
    else if(FEN[i] == 'a'){
        if(FEN[i+1] == '3'){
            return a3;
        }
        else
        if(FEN[i+1] == '6'){
            return a6;
        }
    }
    else if(FEN[i] == 'b'){
        if(FEN[i+1] == '3'){
            return b3;
        }
        else
        if(FEN[i+1] == '6'){
            return b6;
        }
    }
    else if(FEN[i] == 'c'){
        if(FEN[i+1] == '3'){
            return c3;
        }
        else
        if(FEN[i+1] == '6'){
            return c6;
        }
    }
    else if(FEN[i] == 'd'){
        if(FEN[i+1] == '3'){
            return d3;
        }
        else
        if(FEN[i+1] == '6'){
            return d6;
        }
    }
    else if(FEN[i] == 'e'){
        if(FEN[i+1] == '3'){
            return e3;
        }
        else
        if(FEN[i+1] == '6'){
            return e6;
        }
    }
    else if(FEN[i] == 'f'){
        if(FEN[i+1] == '3'){
            return f3;
        }
        else
        if(FEN[i+1] == '6'){
            return f6;
        }
    }
    else if(FEN[i] == 'g'){
        if(FEN[i+1] == '3'){
            return g3;
        }
        else
        if(FEN[i+1] == '6'){
            return g6;
        }
    }
    else if(FEN[i] == 'h'){
        if(FEN[i+1] == '3'){
            return h3;
        }
        else
        if(FEN[i+1] == '6'){
            return h6;
        }
    }
    else {
        printf("FEN incorrecte\n");
        printf("FEN :%c%c%c%c%c%c%c \n", FEN[i-2],FEN[i-1],FEN[i], FEN[i+1],FEN[i+2],FEN[i+3],FEN[i+4]);
        return -1; //garde
    }
}

chessboard* convert_FEN_to_cb(char FEN[92]){
    bool lastline = false;
    chessboard* cb = makeChessboard();
    U64 maskIter = 1;
    maskIter <<= 55;
    int i = 0 ;

    while(FEN[i] != ' '){

        
        //Skips des cases vides
        if(FEN[i] == '/'){
            if((maskIter & row2) != 0){
                maskIter >>=15;
                lastline = true;
            }
            else{
                maskIter >>=16;
            }
            i += 1;
        }
        else if(FEN[i] == '1'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
        }
        else if(FEN[i] == '2'){
            i += 1;
            maskIter <<= 2;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
        }
        else if(FEN[i] == '3'){
            i += 1;
            maskIter <<= 3;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
        }
        else if(FEN[i] == '4'){
            i += 1;
            maskIter <<= 4;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
        }
        else if(FEN[i] == '5'){
            i += 1;
            maskIter <<= 5;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
        }
        else if(FEN[i] == '6'){
            i += 1;
            maskIter <<= 6;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
        }
        else if(FEN[i] == '7'){
            i += 1;
            maskIter <<= 7;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
        }
        else if(FEN[i] == '8'){
            i += 1;
            maskIter <<= 8;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
        }

        else if(FEN[i] == 'r'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->piece[BLACK][ROOK] |= maskIter;

        }
        else if(FEN[i] == 'n'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->piece[BLACK][KNIGHT] |= maskIter;
        }
        else if(FEN[i] == 'b'){
            maskIter <<= 1;
            i += 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->piece[BLACK][BISHOP] |= maskIter;
        }
        else if(FEN[i] == 'q'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->piece[BLACK][QUEEN] |= maskIter;
        }
        else if(FEN[i] == 'k'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->piece[BLACK][KING] |= maskIter;
        }
        else if(FEN[i] == 'p'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->piece[BLACK][PAWN] |= maskIter;
        }

        else if(FEN[i] == 'R'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->piece[WHITE][ROOK] |= maskIter;
        }
        else if(FEN[i] == 'N'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->piece[WHITE][KNIGHT] |= maskIter;
        }
        else if(FEN[i] == 'B'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->piece[WHITE][BISHOP] |= maskIter;
        }
        else if(FEN[i] == 'Q'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->piece[WHITE][QUEEN] |= maskIter;
        }
        else if(FEN[i] == 'K'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->piece[WHITE][KING] |= maskIter;
        }
        else if(FEN[i] == 'P'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->piece[WHITE][PAWN] |= maskIter;
        }
    }

    if(FEN[i+1] == 'w'){
        cb->turn = WHITE;
    }
    else{
        cb->turn = BLACK;
    }

    int j = i + 3;
    bool stop = false;
    while((j < i + 7) && !stop){
        //printf("FEN[j = %d]: '%c'\n", j, FEN[j]);
        stop = true;
        bool emptycastle = false;
        if(FEN[j] == '-'){
            emptycastle = true;
            //printf("EMPTYCASTLE\n");
        }
        if(FEN[j] == 'K'){
            stop = false;
            cb->castle += 8;
            //printf("+8\n");
        }
        else if(FEN[j] == 'Q'){
            stop = false;
            cb->castle += 4;
            //printf("+4\n");
        }
        else if(FEN[j] == 'k'){
            stop = false;
            cb->castle += 2;
            //printf("+2\n");
        }
        else if(FEN[j] == 'q'){
            stop = false;
            cb->castle += 1;
            //printf("+1\n");
        }
        if(stop == false || emptycastle){
            j++;
        }
    }

    cb->enPassantSquare = en_passant_FEN(FEN, j+1);
    j++;
    
    while(FEN[j] != ' '){
        j++;
    }

    j++;

    //CLOCK

    int k=0;
    int k2=0;
    int cpt2 = 0;

    //printf("k = %d, k2 = %d\n", k , k2);

    int j2 = j;

    while((cpt2 == 0 || cpt2 == 1) && (FEN[j] != '\0')){
        //printf("- FEN[%d] = '%c'\n", j,FEN[j]);

        if(cpt2 == 1){

            if(FEN[j] == ' '){
                cpt2++;
                j++;

            }
            else{
                j++;
                k2++;
            }
            
        }
        else if(cpt2 == 0){

            if(FEN[j] == ' '){
                cpt2++;
                j++;

            }
            else{
                j++;
                k++;
            }

        }

    }

    //printf("k = %d, k2 = %d\n", k , k2);

    char* hmc = malloc((k+1)*sizeof(char));

    char* fm  = malloc((k2+1)*sizeof(char));

    strncpy(hmc, FEN + (j2) ,k);
    hmc[k] = '\0';

    strncpy(fm, FEN + (j2 + k +1), k2 );
    fm[k2] = '\0';


    //printf("hmc : '%s'\n", hmc);
    //printf("fm : '%s'\n", fm);

    cb->halfmoveclock = atoi(hmc);
    cb->fullmove      = atoi(fm);

    //printf("test : %d %d\n", cb->halfmoveclock, cb->fullmove);

    cb->hash = generate_hash(cb);
    init_piece_counts(cb);
    
    return cb;
}

void print_info_cb(chessboard* cb){
    if(cb->turn == WHITE){
        printf("turn : WHITE \n");
    }
    else{
        printf("turn : BLACK \n");
    }

    printf("castle : ");
    if((cb->castle & 0b1000) != 0){
        printf("1");
    }
    else{
        printf("0");
    }
    if((cb->castle & 0b0100) != 0){
        printf("1");
    }
    else{
        printf("0");
    }
    if((cb->castle & 0b0010) != 0){
        printf("1");
    }
    else{
        printf("0");
    }
    if((cb->castle & 0b0001) != 0){
        printf("1\n");
    }
    else{
        printf("0\n");
    }

    printf("enPassantSquare : %d\n", cb->enPassantSquare);
    printf("halfmove clock : %d\n", cb->halfmoveclock);
    printf("fullmove : %d\n", cb->fullmove);

    

}

void print_move(move m){
    printf("[from : %d]", m.from);
    printf("[to : %d]", m.to);
    printf("[piece : %d]", m.piece);
    printf("[captured : %d]", m.captured);
    printf("[promo : %d]", m.promo);
    printf("[flag : %d]\n", m.flag);
}

void print_lostdata(ld* lostdata){
    printf("lostdata______________________ \n\n");
    printf("castle : ");
    if((lostdata->castle & 0b1000) != 0){
        printf("1");
    }
    else{
        printf("0");
    }
    if((lostdata->castle & 0b0100) != 0){
        printf("1");
    }
    else{
        printf("0");
    }
    if((lostdata->castle& 0b0010) != 0){
        printf("1");
    }
    else{
        printf("0");
    }
    if((lostdata->castle & 0b0001) != 0){
        printf("1\n");
    }
    printf("enPassantSquare : %d \n", lostdata->enPassantSquare);
    printf("halfmoveclock : %d \n", lostdata->halfmoveclock);
    printf("fullmove : %d \n\n", lostdata->fullmove);

}