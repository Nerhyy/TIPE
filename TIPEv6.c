//PIECE ADAPTED

// Auteur : Neji Nguyen
// Version : 11
// Date : 8 Janvier 2026
// Objectif : Faire une implementation du jeu d'echec en C


// TO DO LIST //////////////////////////////////////////////////////////////////////////////////////////////

/*

- Endgame heuristiques

et c'est tout :)


*/

///////////////////////////////////////////////// MODULES //////////////////////////////////////////////////

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <omp.h>

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
#include "tt.h"
#include "zobrist.h"
#include "arrFrontSpans.h"
#include "eval.h"
#include "manhattan_dist.h"

volatile bool stop_search = false;


typedef uint64_t U64;


const int mvv_lva_table[7][7] = {
    {0, 0, 0, 0, 0, 0, 0}, // 0: EMPTY (Pas de capture)
    {0, 105, 104, 103, 102, 101, 100}, // 1: Pion capturé
    {0, 205, 204, 203, 202, 201, 200}, // 2: Cavalier capturé
    {0, 305, 304, 303, 302, 301, 300}, // 3: Fou capturé
    {0, 405, 404, 403, 402, 401, 400}, // 4: Tour capturée
    {0, 505, 504, 503, 502, 501, 500}, // 5: Reine capturée
    {0, 0, 0, 0, 0, 0, 0}              // 6: Roi (n'est techniquement jamais capturé)
};


int quiescence(chessboard* cb, int alpha, int beta){
    if (stop_search) return 0;
    int stand_pat = eval(cb);
    if(stand_pat >= beta){
        return beta;
    }
    if(alpha < stand_pat){
        alpha = stand_pat;
    }

    moveList l;
    l.count = 0;
    
    generateMoves(cb, &l);

    int move_scores[256] = {0};

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

    ld lostdata = create_lostdata2();

    for (int i = 0; i < l.count; i++) {
        if (stop_search) return 0;
        //Tri
        int best_score_idx = i;
        for (int j = i + 1; j < l.count; j++) {
            if (move_scores[j] > move_scores[best_score_idx]) {
                best_score_idx = j;
            }
        }

       
        // on arrête complètement d'explorer les nœuds pour cette position 
        if (move_scores[best_score_idx] == 0) {
            break; 
        }

        move temp_move = l.moves[i];
        l.moves[i] = l.moves[best_score_idx];
        l.moves[best_score_idx] = temp_move;
        
        int temp_score = move_scores[i];
        move_scores[i] = move_scores[best_score_idx];
        move_scores[best_score_idx] = temp_score;

        makeMove_ld(cb, l.moves[i], &lostdata);

        if (!legalmove_check(cb, l.moves[i])) {
            unmakeMove(cb, l.moves[i], &lostdata);
            continue; // Coup pseudo-légal mais qui met/laisse le roi en échec
        }

        int score = -quiescence(cb, -beta, -alpha);
        
        unmakeMove(cb, l.moves[i], &lostdata);

        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
        }
    }
    
    return alpha;
}

int negaMax(chessboard* cb, int depth, int alpha, int beta){
    if (stop_search) return 0;
    int originalAlpha = alpha;

    int tt_score;
    move tt_best_move = {0};
    if (probe_tt(cb->hash, depth, alpha, beta, &tt_score, &tt_best_move)) {
        //count++;
        return tt_score;
    }
    
    if(depth == 0){
        return quiescence(cb, alpha, beta);
    }
    
    int max = -100000000;
    moveList l;
    l.count = 0;
    generateMoves(cb, &l);


    int move_scores[256] = {0};
    //On met un score à chaque move (pour le tri)
    for (int i = 0; i < l.count; i++) {
        if (tt_best_move.piece != 0 && l.moves[i].from == tt_best_move.from && l.moves[i].to == tt_best_move.to) {
            move_scores[i] = 1000000; 
        } 
        else if (l.moves[i].captured != EMPTY) {
            move_scores[i] = mvv_lva_table[l.moves[i].captured][l.moves[i].piece];
        } 
        else if (l.moves[i].promo != EMPTY) {
            move_scores[i] = 900; 
        } 
        else {
            move_scores[i] = 0; 
        }
    }

    ld lostdata = create_lostdata2();
    move best_move_for_this_node = {0};
    int legal_moves_played = 0;

    for(int i = 0 ; i < l.count; i++){
        if (stop_search) return 0;
        //Tri
        int best_score_idx = i;
        for (int j = i + 1; j < l.count; j++) {
            if (move_scores[j] > move_scores[best_score_idx]) {
                best_score_idx = j;
            }
        }
        
        move temp_move = l.moves[i];
        l.moves[i] = l.moves[best_score_idx];
        l.moves[best_score_idx] = temp_move;
        
        int temp_score = move_scores[i];
        move_scores[i] = move_scores[best_score_idx];
        move_scores[best_score_idx] = temp_score;

        makeMove_ld(cb, l.moves[i], &lostdata);
        if(!legalmove_check(cb, l.moves[i])){
            unmakeMove(cb, l.moves[i], &lostdata);
            continue;
        }

        legal_moves_played++;

        // Calcul en profondeur (NegaMax)
        int score = -negaMax(cb, depth - 1, -beta, -alpha); 
        unmakeMove(cb, l.moves[i], &lostdata);

        if(score > max){
            max = score;
            best_move_for_this_node = l.moves[i];
        }

        if(score > alpha){
            alpha = score;
        }

        if(alpha >= beta){
            break; 
        }
    }


    if (legal_moves_played == 0) {
        if (is_square_attacked(cb, deserialize(cb->piece[cb->turn][KING]), cb->turn)) {
            return -10000 + (100 - depth); // MAT
        } else {
            return 0; // PAT
        }
    }

    // Sauvegarde dans la Table de Transposition
    store_tt(cb->hash, depth, max, originalAlpha, beta, best_move_for_this_node);

    return max;
}



move findBestMove(chessboard* cb, int depth){

    move bestMove = {0,0,0,0,0,0};
    int bestScore = -1000000000;

    int alpha = -1000000000;
    int beta  =  1000000000;

    moveList* l = legalMoveList(cb);
    ld lostdata = {.castle = -1, .enPassantSquare = -1, .fullmove = -1, .halfmoveclock = -1};
    int n_moves = l->count;
    //Recherche en profondeur du meilleur coup
    for(int i = 0 ; i < n_moves; i++){

        makeMove_ld(cb, l->moves[i] , &lostdata);
        //Calcul en profondeur
        int score = -negaMax(cb , depth - 1, -beta, -alpha);

        unmakeMove(cb, l->moves[i] , &lostdata);

        //Attribution du meilleur score
        if(score > bestScore){
            bestScore = score;
            bestMove = l->moves[i];
        }

        if( score > alpha){
            alpha = score;
        }

    }

    free_moveList(l);

    return bestMove;

}


char FENcaptureQueen[92] = "rnbqkbnr/3p1ppp/2p1B3/1p2P2Q/p7/2N5/PPPP1PPP/R1B1K1NR b KQkq - 0 8" ;




/*
Pour le popcount : 1000000000 ->
real : 2.599s
user : 2.592s
sys  : 0.006s



*/

move findBestMove_IDS(chessboard* cb, int depth){

    move bestMove = {0,0,0,0,0,0};
    cb->hash = generate_hash(cb);

    moveList* l = legalMoveList(cb);
    ld lostdata = {.castle = -1, .enPassantSquare = -1, .fullmove = -1, .halfmoveclock = -1};
    int n_moves = l->count;
    //Recherche en profondeur du meilleur coup
    for(int j = 1; j <= depth; j++){
        
        int bestScore = -1000000000;
        int alpha = -1000000000;
        int beta  =  1000000000;
        move bestMoveForThisDepth = {0,0,0,0,0,0};

        for(int i = 0 ; i < n_moves; i++){
            move current_move = l->moves[i];
            
            makeMove_ld(cb,  current_move, &lostdata);
            //Calcul en profondeur
            int score = -negaMax(cb , j - 1, -beta, -alpha);

            unmakeMove(cb, current_move, &lostdata);
            
            //Attribution du meilleur score
            if(score > bestScore){
                bestScore = score;
                bestMoveForThisDepth = current_move;
            }

            if( score > alpha){
                alpha = score;
            }

        }
        bestMove = bestMoveForThisDepth;
    }
    free_moveList(l);
    return bestMove;

}

move findBestMove_IDS_Thread(chessboard* cb, int depth, int thread_id){

    move bestMove = {0,0,0,0,0,0};
    cb->hash = generate_hash(cb);

    moveList* l = legalMoveList(cb);
    ld lostdata = {.castle = -1, .enPassantSquare = -1, .fullmove = -1, .halfmoveclock = -1};
    int n_moves = l->count;
    int start_depth = 1;

    if (thread_id != 0 && n_moves > 1) {

        start_depth = (thread_id % 7) + 1; 

        int swap_idx = thread_id % n_moves;
        if (swap_idx != 0) {
            move temp = l->moves[0];
            l->moves[0] = l->moves[swap_idx];
            l->moves[swap_idx] = temp;
        }
    }
    //Recherche en profondeur du meilleur coup
    for(int j = start_depth; j <= depth; j++){
        
        int bestScore = -1000000000;
        int alpha = -1000000000;
        int beta  =  1000000000;
        move bestMoveForThisDepth = {0,0,0,0,0,0};

        for(int i = 0 ; i < n_moves; i++){
            if (stop_search) break;

            move current_move = l->moves[i];
            
            makeMove_ld(cb,  current_move, &lostdata);
            //Calcul en profondeur
            int score = -negaMax(cb , j - 1, -beta, -alpha);

            unmakeMove(cb, current_move, &lostdata);
            
            //Attribution du meilleur score
            if(score > bestScore){
                bestScore = score;
                bestMoveForThisDepth = current_move;
            }

            if( score > alpha){
                alpha = score;
            }

        }
        bestMove = bestMoveForThisDepth;
    }
    free_moveList(l);
    return bestMove;

}


//////////////////////////////////////////////////// MAIN /////////////////////////////////////////////////////

int main(int argc, char* argv[]){

    fill_tables(); //rooks and bishops
    fill_rectLookup();
    fill_king_Attacks_table();
    fill_knightAttacks_table();
    fill_pawnAttacks_table();
    fill_arrFrontSpans();
    fill_manhattan_distances();
    init_zobrist();
    init_tt();


    char FEN[92] = "r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P1Q1/2N5/PPPP1PPP/R1B1K1NR b KQkq - 5 4";
    bool running = true;
    while (running)
    {
        //printf("From chess engine: Waiting for a fen\n");

        //fgets(FEN, sizeof(FEN), stdin);
        chessboard *cb = convert_FEN_to_cb(FEN);
        move global_best_move = {0};
        stop_search = false;
        #pragma omp parallel num_threads(8)
        {
            int thread_id = omp_get_thread_num();

            chessboard local_cb = *cb;
            if (thread_id == 0) {

                global_best_move = findBestMove_IDS_Thread(&local_cb, 8,thread_id);
        
                stop_search = true; 
            }
            else {
                findBestMove_IDS_Thread(&local_cb,100,thread_id);
            }

        }
        print_move(global_best_move);
        fflush(stdout);
        running = false;
    }
    //printf("hit :%d\n" , count);
    return 0;
}

//./compile && ./TIPEv6.exe