#pragma once
#include "globals.h"


//Operation sur chessboard
U64 takenSquares(chessboard* echiquier);
U64 whitePieces(chessboard* echiquier);
U64 blackPieces(chessboard* echiquier);
chessboard* makeChessboard();
U64 LS1B(U64 mask);
int popCount (U64 x);
int bitScanForward(U64 mask);
U64 serialize(int sq);
chessboard* makeChessboard_og();
int popLSB(U64* mask);