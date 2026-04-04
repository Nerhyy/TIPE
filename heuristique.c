
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

typedef uint64_t U64;


U64 xrayRookAttacks(U64 occ, U64 blockers, int sq){
    U64 r_att = rookAttacks(occ, sq);                //Récupération des attaques de la tour
    blockers &= r_att;                               
    return r_att ^ rookAttacks(occ ^ blockers, sq);   
}

U64 xrayBishopAttacks(U64 occ, U64 blockers, int sq){
    U64 b_att = bishopAttacks(occ, sq);                //Récupération des attaques du bishop
    blockers &= b_att;                               
    return b_att ^ bishopAttacks(occ ^ blockers, sq);   
}