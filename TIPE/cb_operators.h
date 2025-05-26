#pragma once
#include "globals.h"


//Operation sur chessboard
U64 takenSquares(chessboard* echiquier);
chessboard* makeChessboard();
U64 LS1B(U64 mask);
int popCount (U64 x);
int bitScanForward(U64 mask);
U64 serialize(int sq);