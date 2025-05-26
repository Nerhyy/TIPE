
#include "globals.h"
#include "cb_operators.h"
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
    print_mask(echiquier->whiteBishops);
    printf("\n White Pawns : \n");
    print_mask(echiquier->whitePawns);
    printf("\n White King : \n");
    print_mask(echiquier->whiteKing);
    printf("\n White Rooks : \n");
    print_mask(echiquier->whiteRooks);
    printf("\n White Knights : \n");
    print_mask(echiquier->whiteKnights);
    printf("\n White Queen : \n");
    print_mask(echiquier->whiteQueens);


    printf("\n Black Pieces : \n----------------------------------------------\n");

    printf("\n Black Bishops : \n");
    print_mask(echiquier->blackBishops);
    printf("\n Black Pawns : \n");
    print_mask(echiquier->blackPawns);
    printf("\n Black King : \n");
    print_mask(echiquier->blackKing);
    printf("\n Black Rooks : \n");
    print_mask(echiquier->blackRooks);
    printf("\n Black Knights : \n");
    print_mask(echiquier->blackKnights);
    printf("\n Black Queen : \n");
    print_mask(echiquier->blackQueens);

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
        else if((echiquier->whitePawns & maskIter) == maskIter){
            printf("♙");
        }
        else if((echiquier->blackPawns & maskIter) == maskIter){
            printf("♟");
        }
        else if((echiquier->whiteRooks & maskIter) == maskIter){
            printf("♖");
        }
        else if((echiquier->whiteBishops & maskIter) == maskIter){
            printf("♗");
        }
        else if((echiquier->whiteKnights & maskIter) == maskIter){
            printf("♘");
        }
        else if((echiquier->blackRooks & maskIter) == maskIter){
            printf("♜");
        }
        else if((echiquier->blackBishops & maskIter) == maskIter){
            printf("♝");
        }
        else if((echiquier->blackKnights & maskIter) == maskIter){
            printf("♞");
        }
        else if((echiquier->blackKing & maskIter) == maskIter){
            printf("♚");
        }
        else if((echiquier->whiteKing & maskIter) == maskIter){
            printf("♔");
        }
        else if((echiquier->blackQueens & maskIter) == maskIter){
            printf("♛");
        }
        else if((echiquier->whiteQueens & maskIter) == maskIter){
            printf("♕");
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
            cb->blackRooks |= maskIter;

        }
        else if(FEN[i] == 'n'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->blackKnights |= maskIter;
        }
        else if(FEN[i] == 'b'){
            maskIter <<= 1;
            i += 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->blackBishops |= maskIter;
        }
        else if(FEN[i] == 'q'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->blackQueens |= maskIter;
        }
        else if(FEN[i] == 'k'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->blackKing |= maskIter;
        }
        else if(FEN[i] == 'p'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->blackPawns |= maskIter;
        }

        else if(FEN[i] == 'R'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->whiteRooks |= maskIter;
        }
        else if(FEN[i] == 'N'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->whiteKnights |= maskIter;
        }
        else if(FEN[i] == 'B'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->whiteBishops |= maskIter;
        }
        else if(FEN[i] == 'Q'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->whiteQueens |= maskIter;
        }
        else if(FEN[i] == 'K'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->whiteKing |= maskIter;
        }
        else if(FEN[i] == 'P'){
            i += 1;
            maskIter <<= 1;
            if(lastline == true){
                maskIter >>=1;
                lastline = false;
            }
            cb->whitePawns |= maskIter;
        }
    }
    
    return cb;
}