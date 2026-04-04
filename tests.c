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


typedef uint64_t U64;


void rookTestPerf(int i){
    char FEN[92] = "4k3/r7/p6p/8/8/P6P/R7/3K4 w - - 0 1" ;
    chessboard* cb = convert_FEN_to_cb(FEN);

    move mw1 = { a2, h2 , ROOK, EMPTY, EMPTY, EMPTY};
    move mw2 = { h2, a2, ROOK, EMPTY, EMPTY, EMPTY};

    move mb1 = { a7, h7 , ROOK, EMPTY, EMPTY, EMPTY};
    move mb2 = { h7, a7, ROOK, EMPTY, EMPTY, EMPTY};

    int j = 0;
    while(j < i){
        makeMove(cb, mw1);
        legalmove_check(cb, mw1);

        makeMove(cb, mb1);
        legalmove_check(cb, mb1);
        
        makeMove(cb, mw2);
        legalmove_check(cb , mw2);

        makeMove(cb, mb2);
        legalmove_check(cb, mb2);

        if(j%1000000 == 0){
            printf("Progression ... %d iterations\n", j);
        }
        j++;
    }

    printf("rookTestPerf SUCCESSFUL\n");

}
