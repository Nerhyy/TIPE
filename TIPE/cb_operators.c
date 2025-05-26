
#include "globals.h"



chessboard* makeChessboard(){
    chessboard* cb = malloc(sizeof(chessboard));
    cb->whitePawns    = 0;
    cb->whiteRooks    = 0;
    cb->whiteBishops  = 0;
    cb->whiteKnights  = 0;
    cb->whiteQueens   = 0;
    cb->whiteKing    = 0;

    cb->blackPawns    = 0;
    cb->blackRooks    = 0;
    cb->blackBishops  = 0;
    cb->blackKnights  = 0;
    cb->blackQueens   = 0;
    cb->blackKing    = 0;

    return cb;
}


//Renvoie un masque des cases occupees
U64 takenSquares(chessboard* echiquier){
    return echiquier->whitePawns |
    echiquier->whiteRooks |
    echiquier->whiteBishops |
    echiquier->whiteKing |
    echiquier->whiteQueens |
    echiquier->whiteKnights |
    echiquier->blackPawns |
    echiquier->blackRooks |
    echiquier->blackBishops |
    echiquier->blackKnights |
    echiquier->blackKing |
    echiquier->blackQueens;
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
    x =  x       - ((x >> 1)  & k1); /* put count of each 2 bits into those 2 bits */
    x = (x & k2) + ((x >> 2)  & k2); /* put count of each 4 bits into those 4 bits */
    x = (x       +  (x >> 4)) & k4 ; /* put count of each 8 bits into those 8 bits */
    x = (x * kf) >> 56; /* retourne les 8 bits de point fort  de x + (x<<8) + (x<<16) + (x<<24) + ...  */
    return (int) x;
}

int bitScanForward(U64 mask) { //Renvoie l'indice du bit de point faible en LERFM
    U64 lsb;
    if(mask == 0){
        return -1;
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

U64 serialize(int sq){
    U64 square = 1;
    for(int i = 0 ; i < sq ; i++){
        square <<=1;
    }
    return square;
}