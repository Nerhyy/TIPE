//PIECE ADAPTED

#include "knights.h"
#include "globals.h"
#include "shift.h"

typedef uint64_t U64; 


U64 knightAttacks[64];


void fill_knightAttacks_table(){
    U64 iter = 1;
    for(int i = 0; i < 64 ; i++){
        knightAttacks[i] = nAttacks(iter);
        //printf("i : %d\n", i);
        //print_mask(knightAttacks[i]); AFFICHAGE TEST
        iter <<=1;
    }
}

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