#include "tt.h"
#include <string.h>

TTEntry TT[TT_SIZE];
omp_lock_t TT_locks[TT_SIZE];

void init_tt() {
    clear_tt();
    for(int i = 0; i < TT_SIZE; i++) {
        omp_init_lock(&TT_locks[i]); 
    }
}

void clear_tt() {
    memset(TT, 0, sizeof(TTEntry) * TT_SIZE);
}

bool probe_tt(U64 hash, int depth, int alpha, int beta, int *return_score, move* return_best_move) {
    int tt_index = hash % TT_SIZE;
    TTEntry* tte = &TT[tt_index];
    bool hit = false;
    omp_set_lock(&TT_locks[tt_index]); 
    
    if (tte->zobrist_key == hash) {

        *return_best_move = tte->best_move;
        
        if (tte->depth >= depth) {
            if (tte->flag == FLAG_EXACT) {
                *return_score = tte->score;
                hit = true; 
            } 
            else if (tte->flag == FLAG_LOWER_BOUND && tte->score >= beta) {
                *return_score = tte->score;
                hit = true;
            } 
            else if (tte->flag == FLAG_UPPER_BOUND && tte->score <= alpha) {
                *return_score = tte->score;
                hit = true; 
            }
        }
    }
    omp_unset_lock(&TT_locks[tt_index]);

    return hit; 
}

void store_tt(U64 hash, int depth, int score, int originalAlpha, int beta, move best_move) {
    int tt_index = hash % TT_SIZE;
    TTEntry* tte = &TT[tt_index];

    int flag;
    if (score <= originalAlpha) {
        flag = FLAG_UPPER_BOUND;
    } else if (score >= beta) {
        flag = FLAG_LOWER_BOUND;
    } else {
        flag = FLAG_EXACT;
    }

    omp_set_lock(&TT_locks[tt_index]);
    tte->zobrist_key = hash;
    tte->depth = depth;
    tte->score = score;
    tte->flag = flag;
    tte->best_move = best_move;
    omp_unset_lock(&TT_locks[tt_index]);
}