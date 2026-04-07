#include "globals.h"

#define FLAG_EXACT 0
#define FLAG_LOWER_BOUND 1
#define FLAG_UPPER_BOUND 2

#define TT_SIZE 1048576

typedef struct {
    U64 zobrist_key;
    move best_move;  

    int score             : 22; 
    unsigned int depth    : 8;  
    unsigned int flag     : 2; 

    int age;
} TTEntry;

extern TTEntry TT[TT_SIZE];
extern int current_age;

void init_tt();
void clear_tt();
bool probe_tt(U64 hash, int depth, int alpha, int beta, int *return_score, move* return_best_move);
void store_tt(U64 hash, int depth, int score, int originalAlpha, int beta, move best_move);