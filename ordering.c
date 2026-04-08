#include "ordering.h"

move killer_moves[MAX_PLY][2];

const int mvv_lva_table[7][7] = {
    {0, 0, 0, 0, 0, 0, 0}, // 0: EMPTY (Pas de capture)
    {0, 105, 104, 103, 102, 101, 100}, // 1: Pion capturé
    {0, 205, 204, 203, 202, 201, 200}, // 2: Cavalier capturé
    {0, 305, 304, 303, 302, 301, 300}, // 3: Fou capturé
    {0, 405, 404, 403, 402, 401, 400}, // 4: Tour capturée
    {0, 505, 504, 503, 502, 501, 500}, // 5: Reine capturée
    {0, 0, 0, 0, 0, 0, 0}              // 6: Roi (n'est techniquement jamais capturé)
};

void clear_killer_moves() {
    memset(killer_moves, 0, sizeof(killer_moves));
}

void attribute_order_score(int move_scores[], moveList l, move tt_best_move, int ply){
    for (int i = 0; i < l.count; i++) {

        if (l.moves[i].from == tt_best_move.from && 
            l.moves[i].to == tt_best_move.to && 
            l.moves[i].promo == tt_best_move.promo) {
            move_scores[i] = 1000000; 
        } 
        else if (l.moves[i].captured != EMPTY) {
            move_scores[i] = mvv_lva_table[l.moves[i].captured][l.moves[i].piece];
        } 
        else if (l.moves[i].promo != EMPTY) {
            move_scores[i] = 900; 
        } 
        else if (l.moves[i].from == killer_moves[ply][0].from && 
                 l.moves[i].to == killer_moves[ply][0].to && 
                 l.moves[i].piece == killer_moves[ply][0].piece) {
            move_scores[i] = 90; 
        }
        else if (l.moves[i].from == killer_moves[ply][1].from && 
                 l.moves[i].to == killer_moves[ply][1].to && 
                 l.moves[i].piece == killer_moves[ply][1].piece) {
            move_scores[i] = 80;
        }
        else {
            move_scores[i] = 0; 
        }
    }
}

void attribute_quiescence_order_score(int move_scores[], moveList l) {
    for (int i = 0; i < l.count; i++) {
        
        if (l.moves[i].captured != EMPTY) {
            move_scores[i] = mvv_lva_table[l.moves[i].captured][l.moves[i].piece];
        } 
        else if (l.moves[i].promo != EMPTY) {
            move_scores[i] = 900; 
        } 
        else {
            move_scores[i] = 0; 
        }
    }
}