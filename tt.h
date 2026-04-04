#include "globals.h"

#define FLAG_EXACT 0
#define FLAG_LOWER_BOUND 1
#define FLAG_UPPER_BOUND 2

#define TT_SIZE 1048576

typedef struct {
    U64 zobrist_key; // La clé unique de la position
    int depth;            // La profondeur à laquelle on l'a calculée
    int score;            // L'évaluation
    int flag;             // EXACT, LOWER_BOUND ou UPPER_BOUND
} TTEntry;

extern TTEntry TT[TT_SIZE];

void init_tt();
void clear_tt();
bool probe_tt(U64 hash, int depth, int alpha, int beta, int *return_score);
void store_tt(U64 hash, int depth, int score, int originalAlpha, int beta);