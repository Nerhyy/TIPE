#pragma once
#include <stdint.h>
typedef uint64_t U64; 

//Pawn push
U64 wp_push_target(U64 wpawns, U64 empty); //Si un pion peut target une case (1)
U64 wp_Dbpush_target(U64 wpawns , U64 empty); //Si un pion peut target une case (2)
U64 wp_able2_push(U64 wpawns , U64 empty); //Si un pion peut avancer (1)
U64 wp_able2_Dblpush(U64 wpawns , U64 empty); //Si un pion peut avancer (2)

U64 bp_push_target(U64 bpawns, U64 empty);//Si un pion peut target une case (1)
U64 bp_Dbpush_target(U64 wpawns , U64 empty);//Si un pion peut target une case (2)
U64 bp_able2_push(U64 bpawns , U64 empty); //Si un pion peut avancer (1)
U64 bp_able2_Dblpush(U64 bpawns , U64 empty); //Si un pion peut avancer (2)

//Pawn attacks
U64 wp_attacks_all(U64 wpawns);
U64 wp_attacks_east(U64 wpawns);
U64 wp_attacks_west(U64 wpawns);
U64 wp_attacks_Db(U64 wpawns);
U64 wp_attacks_single(U64 wpawns);

U64 bp_attacks_all(U64 bpawns);
U64 bp_attacks_east(U64 bpawns);
U64 bp_attacks_west(U64 bpawns);
U64 bp_attacks_Db(U64 bpawns);
U64 bp_attacks_single(U64 bpawns);

//safe squares
U64 wsafe_pawn_squares(U64 wpawns, U64 bpawns);
U64 bsafe_pawn_squares(U64 bpawns, U64 wpawns);

//Able to capture
U64 wp_able2capture_east(U64 wpawns, U64 bpawns);
U64 wp_able2captures_west(U64 wpawns, U64 bpawns);
U64 wp_all_able2captures(U64 wpawns, U64 bpawns);

U64 bp_able2captures_east(U64 bpawns, U64 wpawns);
U64 bp_able2captures_west(U64 bpawns, U64 wpawns);
U64 bp_all_able2captures(U64 bpawns, U64 wpawns);
