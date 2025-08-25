#include "pawns.h"
#include "globals.h"
#include "shift.h"
//---------------------------------------- Pawns -------------------------------------------------------------

//Push targets

U64 wp_push_target(U64 wpawns , U64 empty){
    return shift_north(wpawns) & empty;
}

U64 bp_push_target(U64 bpawns , U64 empty){
    return shift_south(bpawns) & empty;
}

U64 wp_Dbpush_target(U64 wpawns , U64 empty){
    U64 singlePushs = wp_push_target(wpawns, empty);
    return shift_north(singlePushs) & empty & row4;
}

U64 bp_Dbpush_target(U64 wpawns , U64 empty){
    U64 singlePushs = wp_push_target(wpawns, empty);
    return shift_south(singlePushs) & empty & row5;
}

U64 wp_able2_push(U64 wpawns , U64 empty){
    return shift_south(empty) & wpawns;
}

U64 wp_able2_Dblpush(U64 wpawns , U64 empty) {
    U64 emptyRank3 = shift_south(empty & row4) & empty;
    return wp_able2_push(wpawns, emptyRank3);
}

U64 bp_able2_push(U64 bpawns , U64 empty){
    return shift_north(empty) & bpawns;
}

U64 bp_able2_Dblpush(U64 bpawns , U64 empty) {
    U64 emptyRank6 = shift_north(empty & row5) & empty;
    return bp_able2_push(bpawns, emptyRank6);
}

//Se souvenir du generalized pawnpush si on veut opti

//Attack targets

U64 wp_attacks_all(U64 wpawns){ //trouve toutes les cases attaquees pas des pions blancs
    return shift_northeast(wpawns) | shift_northwest(wpawns);
}

U64 wp_attacks_east(U64 wpawns){ //cases attaquees par l'attaque droite des pions blancs
    return shift_northeast(wpawns);
}

U64 wp_attacks_west(U64 wpawns){ //cases attaquees par l'attaque gauche des pions blancs
    return shift_northwest(wpawns) ;
}

U64 wp_attacks_Db(U64 wpawns){ //trouve les cases attaquees pas des pions 2 fois blancs
    return shift_northeast(wpawns) & shift_northwest(wpawns);
}

U64 wp_attacks_single(U64 wpawns){ //trouve les cases attaquees pas des pions qu'une fois blancs
    return shift_northeast(wpawns) ^ shift_northwest(wpawns);
}



U64 bp_attacks_all(U64 bpawns){ //trouve toutes les cases attaquees pas des pions noirs
    return shift_southwest(bpawns) | shift_southeast(bpawns);
}

U64 bp_attacks_east(U64 bpawns){ //cases attaquees par l'attaque droite des pions noirs
    return shift_southeast(bpawns);
}

U64 bp_attacks_west(U64 bpawns){ //cases attaquees par l'attaque gauche des pions noirs
    return shift_southwest(bpawns);
}

U64 bp_attacks_Db(U64 bpawns){ //trouve les cases attaquees pas des pions 2 fois noirs
    return shift_southeast(bpawns) & shift_southwest(bpawns);
}

U64 bp_attacks_single(U64 bpawns){ //trouve les cases attaquees pas des pions qu'une fois noirs
    return shift_southeast(bpawns) ^ shift_southwest(bpawns);
}


U64 wsafe_pawn_squares(U64 wpawns, U64 bpawns){ // A TESTER
    U64 wp_attacks_east_mask =  wp_attacks_east(wpawns); //pions blancs attaque droite
    U64 wp_attacks_west_mask =  wp_attacks_west(wpawns); //pions blancs attaque gauche
    U64 bp_attacks_east_mask =  bp_attacks_east(bpawns); //pions noirs attaque droite
    U64 bp_attacks_west_mask =  bp_attacks_west(bpawns); //pions noirs attaque gauche

    U64 wp_attacks_Db_mask  =  wp_attacks_east_mask &  wp_attacks_west_mask; // pions blancs double attaque
    U64 wp_oddattacks_mask  =  wp_attacks_east_mask ^ wp_attacks_west_mask; //oddattacks ? A TESTER
    U64 bp_attacks_Db_mask  =  bp_attacks_east_mask &  bp_attacks_west_mask; //pions noirs double attaque
    U64 bp_attacks_all_mask =  bp_attacks_east_mask | bp_attacks_west_mask; //toutes les attaques des pions noirs

    return wp_attacks_Db_mask |~bp_attacks_all_mask  | (wp_oddattacks_mask  &~bp_attacks_Db_mask); //
}

U64 bsafe_pawn_squares(U64 bpawns, U64 wpawns){ // A TESTER
    U64 bp_attacks_east_mask =  bp_attacks_east(bpawns); //pions noirs attaque droite
    U64 bp_attacks_west_mask =  bp_attacks_west(bpawns); //pions noirs attaque gauche
    U64 wp_attacks_east_mask =  wp_attacks_east(wpawns); //pions blancs attaque droite
    U64 wp_attacks_west_mask =  wp_attacks_west(wpawns); //pions blancs attaque gauche

    U64 bp_attacks_Db_mask   = bp_attacks_east_mask &  bp_attacks_west_mask; // pions blancs double attaque
    U64 bp_oddattacks_mask   = bp_attacks_east_mask ^ bp_attacks_west_mask; //oddattacks ? A TESTER
    U64 wp_attacks_Db_mask   = wp_attacks_east_mask &  wp_attacks_west_mask; //pions noirs double attaque
    U64 wp_attacks_all_mask  = wp_attacks_east_mask | wp_attacks_west_mask; //toutes les attaques des pions noirs
    
    return bp_attacks_Db_mask |~wp_attacks_all_mask  | (bp_oddattacks_mask  &~wp_attacks_Db_mask); //
} 

//NOTE: Ces fonctions sont pratique pour les coups candidats, on peut regarder lesbackward pawns aussi

//Pawn captures

U64 wp_able2capture_east(U64 wpawns, U64 bpawns){ //les pions blancs qui peuvent capturer a droite
    return wpawns & bp_attacks_east(bpawns);
}

U64 wp_able2captures_west(U64 wpawns, U64 bpawns){ //les pions blancs qui peuvent capturer a gauche
    return wpawns & bp_attacks_west(bpawns);
}

U64 wp_all_able2captures(U64 wpawns, U64 bpawns){  //les pions blancs qui peuvent capturer a gauche et droite
    return wpawns & bp_attacks_all(bpawns);
}



U64 bp_able2captures_east(U64 bpawns, U64 wpawns){ //les pions noirs qui capturer prendre a droite
    return bpawns & wp_attacks_east(wpawns);
}

U64 bp_able2captures_west(U64 bpawns, U64 wpawns){ //les pions noirs qui capturer prendre a gauche
    return bpawns & wp_attacks_west(wpawns);
}

U64 bp_all_able2captures(U64 bpawns, U64 wpawns){ //les pions noirs qui peuvent capturer a gauche et a droite
    return bpawns & wp_attacks_all(wpawns);
}

