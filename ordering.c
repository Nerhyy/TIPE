#include "ordering.h"

const int mvv_lva_table[7][7] = {
    {0, 0, 0, 0, 0, 0, 0}, // 0: EMPTY (Pas de capture)
    {0, 105, 104, 103, 102, 101, 100}, // 1: Pion capturé
    {0, 205, 204, 203, 202, 201, 200}, // 2: Cavalier capturé
    {0, 305, 304, 303, 302, 301, 300}, // 3: Fou capturé
    {0, 405, 404, 403, 402, 401, 400}, // 4: Tour capturée
    {0, 505, 504, 503, 502, 501, 500}, // 5: Reine capturée
    {0, 0, 0, 0, 0, 0, 0}              // 6: Roi (n'est techniquement jamais capturé)
};

void attribute_order_score(int move_scores[], moveList l){
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