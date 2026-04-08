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
#include "ordering.h"


typedef uint64_t U64;





int quiescence(chessboard* cb, int alpha, int beta){
    int stand_pat = eval(cb);
    if(stand_pat >= beta){
        return beta;
    }
    if(alpha < stand_pat){
        alpha = stand_pat;
    }

    moveList l;
    l.count = 0;
    
    generateCaptures(cb, &l);

    int move_scores[256] = {0};

    attribute_quiescence_order_score(move_scores,l);

    ld lostdata = create_lostdata2();

    for (int i = 0; i < l.count; i++) {

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

int negaMax(chessboard* cb, int depth, int alpha, int beta,int ply){

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
    attribute_order_score(move_scores,l,tt_best_move,ply);

    ld lostdata = create_lostdata2();
    move best_move_for_this_node = {0};
    int legal_moves_played = 0;

    for(int i = 0 ; i < l.count; i++){
        
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
        int score = -negaMax(cb, depth - 1, -beta, -alpha,ply+1); 
        unmakeMove(cb, l.moves[i], &lostdata);

        if(score > max){
            max = score;
            best_move_for_this_node = l.moves[i];
        }

        if(score > alpha){
            alpha = score;
        }

        if(alpha >= beta){
            if (l.moves[i].captured == EMPTY && l.moves[i].promo == EMPTY) {
                if (killer_moves[ply][0].from != l.moves[i].from || 
                    killer_moves[ply][0].to != l.moves[i].to) {
                    killer_moves[ply][1] = killer_moves[ply][0];
                    killer_moves[ply][0] = l.moves[i];
                }
            }
            break; // Coupure Bêta
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



/*move findBestMove(chessboard* cb, int depth){

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

}*/


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
    clear_killer_moves();
    for(int j = 1; j <= depth; j++){
        
        int bestScore = -1000000000;
        int alpha = -1000000000;
        int beta  =  1000000000;
        move bestMoveForThisDepth = {0,0,0,0,0,0};

        if (j > 1) {
            for (int k = 0; k < n_moves; k++) {
                if (l->moves[k].from == bestMove.from && 
                    l->moves[k].to == bestMove.to && 
                    l->moves[k].promo == bestMove.promo) {
                    
                    move temp = l->moves[0];
                    l->moves[0] = l->moves[k];
                    l->moves[k] = temp;
                    break;
                }
            }
        }

        for(int i = 0 ; i < n_moves; i++){
            move current_move = l->moves[i];
            
            makeMove_ld(cb,  current_move, &lostdata);
            //Calcul en profondeur
            int score = -negaMax(cb , j - 1, -beta, -alpha, 1);

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


    char FEN[92];
    bool running = true;
    while (running)
    {
        //printf("From chess engine: Waiting for a fen\n");
        fgets(FEN, sizeof(FEN), stdin);
        chessboard *cb = convert_FEN_to_cb(FEN);
        print_move(findBestMove_IDS(cb, MAX_DEPTH));
        fflush(stdout);
        current_age++;
        //running = false;
    }
    //printf("hit :%d\n" , count);
    return 0;
}

//./compile && ./TIPEv6.exe