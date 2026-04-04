#include "zobrist.h"

U64 zobrist_pieces[2][7][64];
U64 zobrist_castling[16];
U64 zobrist_en_passant[8];
U64 zobrist_black_to_move;

U64 rand64() {
    U64 r1 = (U64)rand() & 0xFFFF;
    U64 r2 = (U64)rand() & 0xFFFF;
    U64 r3 = (U64)rand() & 0xFFFF;
    U64 r4 = (U64)rand() & 0xFFFF;
    return (r1 << 48) | (r2 << 32) | (r3 << 16) | r4;
}

void init_zobrist() {
    for(int c = 0; c < 2; c++)
        for (int p = 1; p < 7; p++)
            for (int sq = 0; sq < 64; sq++)
                zobrist_pieces[c][p][sq] = rand64();
            
    for (int i = 0; i < 16; i++) zobrist_castling[i] = rand64();
    for (int i = 0; i < 8; i++) zobrist_en_passant[i] = rand64();
    
    zobrist_black_to_move = rand64();
}


U64 generate_hash(chessboard* cb) {
    U64 hash = 0;
    
    for (int c = 0; c < 2; c++) { 
        for (int p = 1; p < 7; p++) { 
            
            U64 bitboard = cb->piece[c][p];
            
            while (bitboard) {
                int sq = popLSB(&bitboard);
                hash ^= zobrist_pieces[c][p][sq];
            }
        }
    }
    hash ^= zobrist_castling[cb->castle];

    if (cb->enPassantSquare != -1) {
        int file = cb->enPassantSquare % 8;
        hash ^= zobrist_en_passant[file];
    }
    
    if (cb->turn == BLACK) {
        hash ^= zobrist_black_to_move;
    }
    
    return hash;
}
