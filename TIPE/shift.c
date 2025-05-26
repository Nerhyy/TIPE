#include "globals.h"

//Shift de masques (voir systeme de coordonnee)
U64 shift_south(U64 mask){
    return (mask>>8 & ~row1);
}
U64 shift_north(U64 mask){
    return (mask<<8 & ~row8);
}
U64 shift_east(U64 mask){
    return (mask<<1 & ~Afile);
}
U64 shift_west(U64 mask){
    return (mask>>1 & ~Hfile);
}
U64 shift_northwest(U64 mask){
    return (mask<<7 & ~Hfile);
}
U64 shift_northeast(U64 mask){
    return (mask<<9 & ~Afile);
}
U64 shift_southeast(U64 mask){
    return (mask>>7 &~Afile);
}
U64 shift_southwest(U64 mask){
    return (mask>>9 & ~Hfile);
}
