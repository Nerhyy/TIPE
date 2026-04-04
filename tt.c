#include "tt.h"
#include <string.h>

TTEntry TT[TT_SIZE];


void init_tt() {
    clear_tt();
}

void clear_tt() {
    memset(TT, 0, sizeof(TTEntry) * TT_SIZE);
}

bool probe_tt(U64 hash, int depth, int alpha, int beta, int *return_score) {
    int tt_index = hash % TT_SIZE;
    TTEntry* tte = &TT[tt_index];

    if (tte->zobrist_key == hash && tte->depth >= depth) {
        
        if (tte->flag == FLAG_EXACT) {
            *return_score = tte->score;
            return true; 
        } 
        else if (tte->flag == FLAG_LOWER_BOUND && tte->score >= beta) {
            *return_score = tte->score;
            return true;
        } 
        else if (tte->flag == FLAG_UPPER_BOUND && tte->score <= alpha) {
            *return_score = tte->score;
            return true; 
        }
    }
    return false; 
}

void store_tt(U64 hash, int depth, int score, int originalAlpha, int beta) {
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
    
    tte->zobrist_key = hash;
    tte->depth = depth;
    tte->score = score;
    tte->flag = flag;
}