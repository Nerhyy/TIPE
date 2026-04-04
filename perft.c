#include "perft.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "perft.h"
#include "globals.h"

typedef uint64_t U64; 


//PERFT FILE

//EXAMPLES ON CHESSWIKI
//https://www.chessprogramming.org/Perft_Results

char FENempty[92] = "8/8/8/8/8/8/8/8 w - - 0 1" ;
char fenpft1[92] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ";
char fenpft2[92] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
char fenpft3[92] = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 ";
char fenpft4[92] = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
char fenpft5[92] = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ";



/*--------------------------------COUNTING STATS-------------------------------------------*/

int cpt_capt = 0;
int cpt_check = 0;
int cpt_ep = 0;
int cpt_promo = 0;
int cpt_castle = 0;
int cpt_checkmate = 0;

/*--------------------------------DATA PERFT-------------------------------------------

MES STATS

perft(cb, 1) = 20
perft(cb, 2) = 420
perft(cb, 3) = 9322
perft(cb, 4) = 192 095 / 208 161



STATS ATTENDUES

perft(cb, 1) = 20
perft(cb, 2) = 420
perft(cb, 3) = 9322
perft(cb, 4) = 206 603

*/


void counting_stats(chessboard* cb, move m, int depth){
        //Dénombrement des en passants
        if(m.flag == ENPASSANT){
            //printf("EN PASSANT DETECTED %d : \n", cb->enPassantSquare );
            //print_chessboard_white(cb);
            cpt_ep++;
        }

        //Dénombrement des captures
        if(m.captured != 0){
            //printf("CAPTURE DETECTED %d AT DEPTH %d : \n", cpt_capt, depth);
            //print_chessboard_white(cb);
            cpt_capt++;
        }

        //Dénombrement des checks
        if(is_square_attacked(cb, deserialize(cb->piece[cb->turn][KING]), cb->turn)){
            //printf("CHECK DETECTED : \n");
            //print_chessboard_white(cb);
            cpt_check++;
        }

        //Dénombrement des promotions
        if(m.promo != 0){
            //printf("PROMOTION DETECTED %d : \n", m.promo );
            //print_chessboard_white(cb);
            cpt_promo++;
        }

        //Dénombrement des castles
        if(m.flag == CASTLE){
            printf("CASTLE DETECTED AT DEPTH %d \n", depth);
            printf("TURN : %d \n", cb->turn);
            print_move(m);
            print_info_cb(cb);
            print_chessboard_white(cb);
            cpt_castle++;
        }
}

U64 perft(chessboard *cb, int depth){


    if((depth == 0)){
        return 0;
    }


    moveList *l = legalMoveList(cb);
    ld* lostdata = create_lostdata();

    U64 nodes = 0;

    //printf("je vais rentrer dans la boucle\n");

    //printf("count : %d \n", l->count);
    
    if(l->count == 0){
        printf("CHECKMATE DETECTED : \n");
        print_chessboard_white(cb);
        cpt_checkmate++;
    }

    //printf("count : %d \n", l->count);

    for(int i = 0 ; i < l->count; i++){

        //printf("%d\n", i);

        //printf("-----------------------------------------------\n");
        //print_chessboard_white(cb);

        makeMove_ld(cb, l->moves[i] , lostdata);



        counting_stats(cb, l->moves[i], depth);

        //printf("Je passe le makemove\n");

        nodes++;

        
        //printf("move done : \n");
        //print_move(l->moves[i]);
        //print_chessboard_white(cb);
        


        nodes += perft(cb, depth-1);

        unmakeMove(cb, l->moves[i] , lostdata);
        //print_info_cb(cb);
    }

    free(lostdata);
    return nodes;
    
}

U64 perft2(chessboard *cb, int depth) {
    moveList *l = legalMoveList(cb);
    ld* lostdata = create_lostdata();
    if (depth == 1) return l->count;
    U64 nodes = 0;
    for (int i = 0; i < l->count; i++) {
        makeMove_ld(cb, l->moves[i], lostdata);
        nodes += perft(cb, depth - 1);
        unmakeMove(cb, l->moves[i], lostdata);
    }
    return nodes;
}

void affiche_tab_perft(int* tab, int depth){
    for(int i = 0; i < depth; i++){
        printf(" DEPTH : %d \n %d", depth + 1, tab[i]);
    }
}

void test_perft(chessboard* cb, int n){
    U64 nodes = perft(cb, n);
    printf("nombre de coups : %ld \n", nodes);
    printf("cpt_ep : %d \n", cpt_ep);
    printf("cpt_check : %d \n", cpt_check);
    printf("cpt_capt : %d \n", cpt_capt);
    //affiche_tab_perft(cpt_capt_d, 10);
    printf("cpt_promotions : %d \n", cpt_promo);
    printf("cpt_castle : %d \n", cpt_castle);
    printf("cpt_checkmate : %d \n", cpt_checkmate);

}

