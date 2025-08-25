#pragma once
#include <stdint.h>
typedef uint64_t U64; 


typedef struct GMagic{
  U64 mask; //masque d'occupation pertinent
  U64 magic; //le nombre magic
}gmagic;

extern U64 b_attacks[64][2048];
extern U64 r_attacks[64][4096];

extern gmagic bishop_table[64];
extern gmagic rook_table[64];


extern const U64 RMagic[64];

extern const U64 BMagic[64];

extern const unsigned int magicmoves_r_shift[64];

extern const unsigned int magicmoves_b_shift[64];

//####################################################### PROTOYPES ######################################

U64 random_U64();
U64 random_U64_fewbits();
U64 count_1s(U64 b);
int pop_1st_bit(U64* bb);
U64 index_to_U64(int index, int bits, U64 m);
U64 rmask(int sq);
//void print_mask(U64 mask); //METTRE DANS UTILES
char* fprintf_mask(U64 mask); //METTRE DANS UTILES
U64 bmask(int sq);
U64 ratt(int sq, U64 block);
U64 batt(int sq, U64 block);
int transform(U64 b, U64 magic, int bits);
U64 find_magic(int sq, int m, int bishop);
void fill_table_attacks_bishops(int sq, int m);
void fill_table_attacks_rooks(int sq, int m);
void print_string(char* c); //METTRE DANS UTILES
U64 bishopAttacks(U64 occ, int sq);
U64 rookAttacks(U64 occ, int sq);

void fill_tables(); //La fonction mère