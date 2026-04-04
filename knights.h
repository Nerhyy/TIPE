#pragma once


#include<stdint.h>
typedef uint64_t U64; 


//Knight moves (voir la rosas)
U64 nNoNoEa_target(U64 n);
U64 nNoNoWe_target(U64 n);
U64 nEaEaNo_target(U64 n);
U64 nEaEaSo_target(U64 n);
U64 nSoSoEa_target(U64 n);
U64 nSoSoWe_target(U64 n);
U64 nWeWeNo_target(U64 n);
U64 nWeWeSo_target(U64 n);
U64 nAttacks(U64 knights);
U64 nfill (U64 knights);
U64 n_fork_targets(U64 targets);
int n_dist2square(U64 knight, U64 square);

void fill_knightAttacks_table();

extern U64 knightAttacks[64];
