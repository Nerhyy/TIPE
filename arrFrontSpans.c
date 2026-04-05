#include "arrFrontSpans.h"

U64 arrFrontSpans[2][64];

void fill_arrFrontSpans(){




    U64 maskIter = 1;
 
    for(int i = 0; i < 64 ; i++){ //

        if(i < 56){ //Pour les blancs
            //printf("maskUp au départ %d : \n", i);
            U64 maskUp = maskIter << 8;
            //print_mask(maskUp);

            //printf("maskUp apres %d : \n", i);
            maskUp = maskUp | ((maskUp<<1) & ~Afile);
            maskUp = maskUp | ((maskUp>>1) & ~Hfile);
            //print_mask(maskUp);

            arrFrontSpans[WHITE][i] = maskUp;

            for(int j = 0 ; j < ((8 - (i/8)) -2) ; j++){
                arrFrontSpans[WHITE][i] |= arrFrontSpans[WHITE][i]<<8;
            }
        }
        else{
           arrFrontSpans[WHITE][i] = 0; 
        }
        if(i > 7){ //Pour les noirs
            U64 maskDown = maskIter >> 8;
            maskDown = maskDown | ((maskDown<<1) & ~Afile);
            maskDown = maskDown | ((maskDown>>1) & ~Hfile);

            arrFrontSpans[BLACK][i] = maskDown;

            for(int j = 0 ; j < i/8 -1 ; j++){
                arrFrontSpans[BLACK][i] |= arrFrontSpans[BLACK][i]>>8;
            }
        }
        else{
           arrFrontSpans[BLACK][i] = 0; 
        }

        maskIter <<=1;

        
    }
}