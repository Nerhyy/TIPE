//PIECE ADAPTED

#include "globals.h"
#include <stdlib.h>
#include<stdio.h>
#include<stdbool.h>



U64 voidmask = 0 ;  //ok
U64 univers = 0xFFFFFFFFFFFFFFFF ; //ok

const U64 files[8] = { // de A = 0 à H = 7 
    0x0101010101010101 ,
    0x0202020202020202 ,
    0x0404040404040404 ,
    0x0808080808080808 ,
    0x1010101010101010 ,
    0x2020202020202020 ,
    0x4040404040404040 ,
    0x8080808080808080 ,
};

U64 Afile = 0x0101010101010101 ; //ok
U64 Bfile = 0x0202020202020202 ; //ok
U64 Cfile = 0x0404040404040404 ; //ok
U64 Dfile = 0x0808080808080808 ; //ok   
U64 Efile = 0x1010101010101010 ; //ok
U64 Ffile = 0x2020202020202020 ; //ok
U64 Gfile = 0x4040404040404040 ; //ok
U64 Hfile = 0x8080808080808080 ; //ok

U64 row1 = 0x00000000000000FF ; //ok
U64 row2 = 0x000000000000FF00 ; //ok
U64 row3 = 0x0000000000FF0000 ; //ok
U64 row4 = 0x00000000FF000000 ; //ok
U64 row5 = 0x000000FF00000000 ; //ok
U64 row6 = 0x0000FF0000000000 ; //ok
U64 row7 = 0x00FF000000000000 ; //ok
U64 row8 = 0xFF00000000000000 ; //ok

U64 lightSquares = 0x55AA55AA55AA55AA ; //ok
U64 blackSquares = 0xAA55AA55AA55AA55 ; //ok

move emptymove = {0,0,0,0,0,0};
