#include "move_making.h"

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
#include "zobrist.h"


int abs(int a){
    if(a < 0){
        return -a;
    }
    else{
        return a;
    }
}

void castling_rights(chessboard* cb, move m, U64 from){
    //Detection de perte de droit de roquer___________________________________________________________________
        if(m.piece == KING){
            if(cb->turn == WHITE){
                cb->castle &= 0b0011; //On retire les droits de roque aux blancs
            }
            else{
                cb->castle &= 0b1100; //On retire les droits de roque aux noirs
            }
        }
        if(m.piece == ROOK){
            if(cb->turn == WHITE){
                if(from == 0){
                    cb->castle &= 0b0111; //On retire les droits de roque à gauche aux blancs
                }
                if(from == 7){
                    cb->castle &= 0b1011; //On retire les droits de roque à droite aux blancs
                }
            }
            else{
                if(from == 65){
                    cb->castle &= 0b0010; //On retire les droits de roque à gauche aux noirs
                } 
                if(from == 63){
                    cb->castle &= 0b0001; //On retire les droits de roque à droite aux noirs
                }
            }
        }
        if(m.captured == ROOK){
            if(cb->turn == WHITE){
                if(m.to == 63){
                    cb->castle &= 0b1110;
                }
                if(m.to == 56){
                    cb->castle &= 0b1101;
                }
            }
            if(cb->turn == BLACK){
                if(m.to == 0){
                    cb->castle &= 0b0111;
                }
                if(m.to == 7){
                    cb->castle &= 0b1011;
                }
            }
            
        }
}

//Le pions est déjà bougé, on le transforme en la piece voulue
void promotion(chessboard* cb, move m, U64 to){
    if(cb->turn == WHITE){
        cb->piece[cb->turn][PAWN] &= ~row8; //On retire le pion
        cb->piece[cb->turn][m.promo] |= to;
    }
    else{
        cb->piece[cb->turn][PAWN] &= ~row1;
        cb->piece[cb->turn][m.promo] |= to;
    }
}


//On va verifié que le move m était pseudo-legal
void makeMove(chessboard* cb, move m){ //                     A TESTER
    U64 to = serialize(m.to);
    U64 from = serialize(m.from);


    if(m.flag == CASTLE){
        if(cb->turn){
            cb->piece[cb->turn][KING] &= ~from;   // On retire la piece
            cb->piece[cb->turn][KING] |= to;     //On pose la nouvelle piece sur la nouvelle case

            if(m.to == c1){
                cb->piece[cb->turn][ROOK] &= ~0x0000000000000001;
                cb->piece[cb->turn][ROOK] |= 0x0000000000000008;
            }
            if(m.to == g1){
                cb->piece[cb->turn][ROOK] &= ~0x0000000000000080;
                cb->piece[cb->turn][ROOK] |= 0x0000000000000020;
            }

            if(!cb->turn){ //la clock des coups
                cb->fullmove += 1; 
            }

            cb->halfmoveclock +=1; //la clock des demis-coups

            cb->castle &= 0b0011; //On retire les droits de roque

            cb->enPassantSquare = -1;

            cb->turn = !cb->turn; //On change le tour

        }
        else if (cb->turn == BLACK){
            cb->piece[cb->turn][KING] &= ~from;   // On retire la piece
            cb->piece[cb->turn][KING] |= to;     //On pose la nouvelle piece sur la nouvelle case

            if(m.to == c8){
                cb->piece[cb->turn][ROOK] &= ~0x0100000000000000;
                cb->piece[cb->turn][ROOK] |= 0x0800000000000000;
            }
            if(m.to == g8){
                cb->piece[cb->turn][ROOK] &= ~0x8000000000000000;
                cb->piece[cb->turn][ROOK] |= 0x2000000000000000;
            }

            if(!cb->turn){ //la clock des coups
                cb->fullmove += 1;
            }

            cb->halfmoveclock +=1; //la clock des demis-coups

            cb->castle &= 0b1100; //On retire les droits de roque

            cb->enPassantSquare = -1;

            cb->turn = !cb->turn; //On change le tour
        }
        



    }
    else if(m.captured != 0){ //Coup avec capture_________________________________________________________________________________________
        cb->piece[cb->turn][m.piece] &= ~from;   // On retire la piece
        cb->piece[cb->turn][m.piece] |= to;     //On pose la nouvelle piece sur la nouvelle case


        //En cas de capture EN PASSANT________________________________________________________________________________
        if(m.flag == ENPASSANT){


            
            if(cb->turn == WHITE){
                cb->piece[!cb->turn][m.captured] &= ~(to >> 8); //On enleve la piece capturée                              A VERIFIER
            }
            if(cb->turn == BLACK){
                cb->piece[!cb->turn][m.captured] &= ~(to << 8); //On enleve la piece capturée                              A VERIFIER
            }
            cb->enPassantSquare = -1;
            
        }
        else{
            cb->piece[!cb->turn][m.captured] &= ~to; //On enleve la piece capturée
        }

        if(!cb->turn){ //la clock des coups
            cb->fullmove += 1;
        }

        cb->halfmoveclock +=1; //la clock des demis-coups

        //Detection de perte de droit de roquer___________________________________________________________________
        castling_rights(cb, m, from);

        //Promotions______________________________________________________________________________________________
        if(m.promo != 0){
            promotion(cb, m, to);
        }

        cb->enPassantSquare = -1;

        cb->turn = !cb->turn; //On change le tour
        


    }
    else{ //Coups normaux______________________________________________________________________


        cb->piece[cb->turn][m.piece] &= ~from; // On retire la piece
        cb->piece[cb->turn][m.piece] |= to;   //On pose la nouvelle piece sur la nouvelle case

        if(!cb->turn){ //la clock des coups
            cb->fullmove += 1; 
        }

        cb->halfmoveclock +=1; //la clock des demis-coups

        cb->enPassantSquare = -1;


        if(m.piece == PAWN){ //Detection de en passant
            if(abs(m.to - m.from) == 16){
                //printf("yo le en passsssssssssssssssssssssssaaaaaaaaaaaaaaaannnnnnnnntttttttttttttt\n");
                if(cb->turn == WHITE){
                    cb->enPassantSquare = (m.from + 8); //ajoute du enPassantSquare
                }
                else if (cb->turn == BLACK){
                    cb->enPassantSquare = (m.from - 8); //ajoute du enPassantSquare
                }
                else{
                    printf("ERROR MAKE MOVE------------------------------------------------------------\n");
                }
            }
        }

        //Detection de perte de droit de roquer___________________________________________________________________
        castling_rights(cb, m, from);

        //Promotions_______________________________________________________________________________________________
        if(m.promo != 0){
            promotion(cb, m, to);
        }

        cb->turn = !cb->turn; //On change le tour
    }

}

//Creation d'un lost data pour le unmake move
ld* create_lostdata(){

    ld* lostdata = malloc(sizeof(ld));

    lostdata->castle = -1;
    lostdata->enPassantSquare = -1;
    lostdata->halfmoveclock = -1;
    lostdata->fullmove = -1;

    return lostdata;
}

ld create_lostdata2(){

    ld lostdata;

    lostdata.castle = -1;
    lostdata.enPassantSquare = -1;
    lostdata.halfmoveclock = -1;
    lostdata.fullmove = -1;

    return lostdata;
}

void reset_lostdata(ld* lostdata){
    lostdata->castle = -1;
    lostdata->enPassantSquare = -1;
    lostdata->halfmoveclock = -1;
    lostdata->fullmove = -1;
}

void makeMove_castle_ld(chessboard* cb, move m, ld* lostdata, U64 from, U64 to)
{
    if(cb->turn == WHITE){
        cb->piece[cb->turn][KING] &= ~from;   // On retire la piece
        cb->piece[cb->turn][KING] |= to;     //On pose la nouvelle piece sur la nouvelle case
        cb->hash ^= zobrist_pieces[WHITE][KING][m.from];
        cb->hash ^= zobrist_pieces[WHITE][KING][m.to];

        if(m.to == c1){
            cb->piece[cb->turn][ROOK] &= ~0x0000000000000001;
            cb->piece[cb->turn][ROOK] |= 0x0000000000000008;

            cb->hash ^= zobrist_pieces[WHITE][ROOK][a1];
            cb->hash ^= zobrist_pieces[WHITE][ROOK][d1];
        }
        if(m.to == g1){
            cb->piece[cb->turn][ROOK] &= ~0x0000000000000080;
            cb->piece[cb->turn][ROOK] |= 0x0000000000000020;
            cb->hash ^= zobrist_pieces[WHITE][ROOK][h1];
            cb->hash ^= zobrist_pieces[WHITE][ROOK][f1];
        }

        if(!cb->turn){ //la clock des coups
            lostdata->fullmove = cb->fullmove;
            cb->fullmove += 1; 
        }

        lostdata->halfmoveclock = cb->halfmoveclock;
        cb->halfmoveclock +=1; //la clock des demis-coups

        lostdata->castle = cb->castle;
        cb->castle &= 0b0011; //On retire les droits de roque

        lostdata->enPassantSquare = cb->enPassantSquare;
        cb->enPassantSquare = -1;



    }
    else if (cb->turn == BLACK){
        cb->piece[cb->turn][KING] &= ~from;   // On retire la piece
        cb->piece[cb->turn][KING] |= to;     //On pose la nouvelle piece sur la nouvelle case
        cb->hash ^= zobrist_pieces[BLACK][KING][m.from];
        cb->hash ^= zobrist_pieces[BLACK][KING][m.to];

        if(m.to == c8){
            cb->piece[cb->turn][ROOK] &= ~0x0100000000000000;
            cb->piece[cb->turn][ROOK] |= 0x0800000000000000;

            cb->hash ^= zobrist_pieces[BLACK][ROOK][a8];
            cb->hash ^= zobrist_pieces[BLACK][ROOK][d8];
        }
        if(m.to == g8){
            cb->piece[cb->turn][ROOK] &= ~0x8000000000000000;
            cb->piece[cb->turn][ROOK] |= 0x2000000000000000;

            cb->hash ^= zobrist_pieces[BLACK][ROOK][h8];
            cb->hash ^= zobrist_pieces[BLACK][ROOK][f8];
        }

        if(!cb->turn){ //la clock des coups
            lostdata->fullmove = cb->fullmove;
            cb->fullmove += 1;
        }

        lostdata->halfmoveclock = cb->halfmoveclock;
        cb->halfmoveclock +=1; //la clock des demis-coups

        lostdata->castle = cb->castle;
        cb->castle &= 0b1100; //On retire les droits de roque

        lostdata->enPassantSquare = cb->enPassantSquare;
        cb->enPassantSquare = -1;

    }
}
//modifié
void makeMove_capture_ld(chessboard* cb, move m, ld* lostdata, U64 from, U64 to)
{
    cb->piece[cb->turn][m.piece] &= ~from;   // On retire la piece
    cb->piece[cb->turn][m.piece] |= to;     //On pose la nouvelle piece sur la nouvelle case

    cb->hash ^= zobrist_pieces[cb->turn][m.piece][m.from];
    cb->hash ^= zobrist_pieces[cb->turn][m.piece][m.to];

    int other_turn = 1 - cb->turn;
    // En cas de capture EN PASSANT________________________________________________________________________________
    if(m.flag == ENPASSANT){


        
        if(cb->turn == WHITE){
            cb->piece[other_turn][m.captured] &= ~(to >> 8); //On enleve la piece capturée                              A VERIFIER

            cb->hash ^= zobrist_pieces[other_turn][PAWN][m.to - 8];
        }
        if(cb->turn == BLACK){
            cb->piece[other_turn][m.captured] &= ~(to << 8); //On enleve la piece capturée                              A VERIFIER
            cb->hash ^= zobrist_pieces[other_turn][PAWN][m.to + 8];
        }
        lostdata->enPassantSquare = cb->enPassantSquare;
        cb->enPassantSquare = -1;
        
    }
    else{
        cb->piece[other_turn][m.captured] &= ~to; //On enleve la piece capturée

        cb->hash ^= zobrist_pieces[other_turn][m.captured][m.to];
    }

    if(other_turn == WHITE){ //la clock des coups
        lostdata->fullmove = cb->fullmove;
        cb->fullmove += 1;
    }

    lostdata->halfmoveclock = cb->halfmoveclock;
    cb->halfmoveclock +=1; //la clock des demis-coups

    //Detection de perte de droit de roquer___________________________________________________________________
    lostdata->castle = cb->castle;
    castling_rights(cb, m, from);

    //Promotions______________________________________________________________________________________________
    if(m.promo != 0){
        promotion(cb, m, to);

        cb->hash ^= zobrist_pieces[cb->turn][PAWN][m.to];
        cb->hash ^= zobrist_pieces[cb->turn][m.promo][m.to];
    }

    lostdata->enPassantSquare = cb->enPassantSquare;
    cb->enPassantSquare = -1;
}
//modifié
void makeMove_default_ld(chessboard* cb, move m, ld* lostdata, U64 from, U64 to)
{

    cb->piece[cb->turn][m.piece] &= ~from; // On retire la piece
    cb->piece[cb->turn][m.piece] |= to;   //On pose la nouvelle piece sur la nouvelle case

    cb->hash ^= zobrist_pieces[cb->turn][m.piece][m.from];
    cb->hash ^= zobrist_pieces[cb->turn][m.piece][m.to];

    if(!cb->turn){ //la clock des coups
        lostdata->fullmove = cb->fullmove;
        cb->fullmove += 1; 
    }

    lostdata->halfmoveclock = cb->halfmoveclock;
    cb->halfmoveclock +=1; //la clock des demis-coups

    lostdata->enPassantSquare = cb->enPassantSquare;
    cb->enPassantSquare = -1;


    if(m.piece == PAWN){ //Detection de en passant
        if(abs(m.to - m.from) == 16){

            if(cb->turn == WHITE){
                lostdata->enPassantSquare = cb->enPassantSquare;
                cb->enPassantSquare = (m.from + 8); //ajoute du enPassantSquare
            }
            else if (cb->turn == BLACK){
                lostdata->enPassantSquare = cb->enPassantSquare;
                cb->enPassantSquare = (m.from - 8); //ajoute du enPassantSquare
            }
            else{
                printf("ERROR MAKE MOVE------------------------------------------------------------\n");
            }
        }
    }

    //Detection de perte de droit de roquer___________________________________________________________________
    lostdata->castle = cb->castle;
    castling_rights(cb, m, from);

    //Promotions_______________________________________________________________________________________________
    if(m.promo != 0){
        promotion(cb, m, to);

        cb->hash ^= zobrist_pieces[cb->turn][PAWN][m.to];
        cb->hash ^= zobrist_pieces[cb->turn][m.promo][m.to];
    }
}
//modifié
void makeMove_ld(chessboard* cb, move m, ld* lostdata){ //                     A TESTER

    lostdata->hash = cb->hash;

    cb->hash ^= zobrist_castling[cb->castle];
    if(cb->enPassantSquare != -1){
        cb->hash ^= zobrist_en_passant[cb->enPassantSquare % 8];
    }

    U64 to = serialize(m.to);
    U64 from = serialize(m.from);


    if(m.flag == CASTLE){
        makeMove_castle_ld(cb, m, lostdata, from, to);
    }
    else if(m.captured != 0){ //Coup avec capture_________________________________________________________________________________________
        makeMove_capture_ld(cb, m, lostdata, from, to);
    }
    else{ //Coups normaux______________________________________________________________________
        makeMove_default_ld(cb, m, lostdata, from, to);
    }

    cb->hash ^= zobrist_castling[cb->castle];
    if(cb->enPassantSquare != -1){
        cb->hash ^= zobrist_en_passant[cb->enPassantSquare % 8];
    }

    cb->turn = 1 - cb->turn; //On change le tour

    cb->hash ^= zobrist_black_to_move;

    // --- LE TEST ULTIME (À ENLEVER QUAND TOUT MARCHE) ---
    U64 verif_hash = generate_hash(cb);
    if (cb->hash != verif_hash) {
        printf("ALERTE BUG ZOBRIST ! Coup joué : de %d à %d\n", m.from, m.to);
        assert(cb->hash == verif_hash); // Fait crasher le programme volontairement
    }
}


void unmakeMove(chessboard* cb, move m, ld* lostdata){ //FINIR DE DEBUG
    U64 to = serialize(m.to);
    U64 from = serialize(m.from);


    if(m.flag == CASTLE){ //Roque

        if(cb->piece[!cb->turn][KING] == G1){

            cb->piece[!cb->turn][KING] &= ~G1;
            cb->piece[!cb->turn][ROOK] &= ~F1;

            cb->piece[!cb->turn][KING] |= E1;
            cb->piece[!cb->turn][ROOK] |= H1;

        }
        else if(cb->piece[!cb->turn][KING] == G8){

            cb->piece[!cb->turn][KING] &= ~G8;
            cb->piece[!cb->turn][ROOK] &= ~F8;

            cb->piece[!cb->turn][KING] |= E8;
            cb->piece[!cb->turn][ROOK] |= H8;

        }
        else if(cb->piece[!cb->turn][KING] == C1){

            cb->piece[!cb->turn][KING] &= ~C1;
            cb->piece[!cb->turn][ROOK] &= ~D1;

            cb->piece[!cb->turn][KING] |= E1;
            cb->piece[!cb->turn][ROOK] |= A1;

        }
        else if(cb->piece[!cb->turn][KING] == C8){

            cb->piece[!cb->turn][KING] &= ~C8;
            cb->piece[!cb->turn][ROOK] &= ~D8;

            cb->piece[!cb->turn][KING] |= E8;
            cb->piece[!cb->turn][ROOK] |= A8;

        }

        cb->halfmoveclock   = lostdata->halfmoveclock;
        cb->fullmove        = lostdata->fullmove;
        cb->enPassantSquare = lostdata->enPassantSquare;
        cb->castle          = lostdata->castle;

        reset_lostdata(lostdata);
        cb->turn = !cb->turn; //On change le tour

    }
    else if(m.captured != 0){ // Coups avec capture

        if(m.promo != 0){ //En cas de promotion
            cb->piece[!cb->turn][m.promo] &= ~to;  //On retire la piece promut
            cb->piece[!cb->turn][m.piece] |= from; //On pose la nouvelle piece sur la case ou elle fut avant
            cb->piece[cb->turn][m.captured] |= to; //On remet l'ancienne piece

            cb->halfmoveclock   = lostdata->halfmoveclock;
            cb->fullmove        = lostdata->fullmove;
            cb->enPassantSquare = lostdata->enPassantSquare;
            cb->castle          = lostdata->castle;

        }
        
        else if(m.flag == ENPASSANT){

            cb->piece[!cb->turn][m.piece] &= ~to; // On retire la piece
            cb->piece[!cb->turn][m.piece] |= from;   //On pose la nouvelle piece sur la nouvelle case
    

            
            if (!cb->turn == WHITE) { 
                cb->piece[cb->turn][m.captured] |= to >> 8; //On remet l'ancienne piece
            } 
                // Si les Noirs ont joué (coup blanc annulé, !cb->turn == BLACK)
            else { 
                cb->piece[cb->turn][m.captured] |= to << 8; //On remet l'ancienne piece
            }

            cb->halfmoveclock   = lostdata->halfmoveclock;
            cb->fullmove        = lostdata->fullmove;
            cb->enPassantSquare = lostdata->enPassantSquare;
            cb->castle          = lostdata->castle;

        }
             
        else{
            cb->piece[!cb->turn][m.piece] &= ~to; // On retire la piece
            cb->piece[!cb->turn][m.piece] |= from;   //On pose la nouvelle piece sur la nouvelle case
            cb->piece[cb->turn][m.captured] |= to; //On remet l'ancienne piece


            cb->halfmoveclock   = lostdata->halfmoveclock;
            cb->fullmove        = lostdata->fullmove;
            cb->enPassantSquare = lostdata->enPassantSquare;
            cb->castle          = lostdata->castle;
        }



        reset_lostdata(lostdata);
        cb->turn = !cb->turn; //On change le tour



    }
    else{ //Coup normaux

        if(m.promo != 0){ //En cas de promotion
        cb->piece[!cb->turn][m.promo] &= ~to;
        cb->piece[!cb->turn][m.piece] |= from;   //On pose la nouvelle piece sur la nouvelle case

        cb->halfmoveclock   = lostdata->halfmoveclock;
        cb->fullmove        = lostdata->fullmove;
        cb->enPassantSquare = lostdata->enPassantSquare;
        cb->castle          = lostdata->castle;

        }

        
        cb->piece[!cb->turn][m.piece] &= ~to; // On retire la piece
        cb->piece[!cb->turn][m.piece] |= from;   //On pose la nouvelle piece sur la nouvelle case

        cb->halfmoveclock   = lostdata->halfmoveclock;
        cb->fullmove        = lostdata->fullmove;
        cb->enPassantSquare = lostdata->enPassantSquare;
        cb->castle          = lostdata->castle;
        cb->hash = lostdata->hash;


        reset_lostdata(lostdata);
        cb->turn = !cb->turn; //On change le tour
    }
}

//true  -> legal move
//false -> illegal move
bool legalmove_check(chessboard* cb, move m){
    //printf("legalmovecheck! \n");
    if((cb->piece[cb->turn][KING] == 0) || (cb->piece[!cb->turn][KING] == 0)){
        return false; //GARDE
    }
    if(m.flag == CASTLE){
        if(cb->turn == WHITE){

            if(cb->piece[!cb->turn][KING] == G1){
                return !is_square_attacked(cb, g1, !cb->turn)
                && !is_square_attacked(cb, f1, !cb->turn)
                && !is_square_attacked(cb, e1, !cb->turn);
            }

            if(cb->piece[!cb->turn][KING] == C1){
                return !is_square_attacked(cb, c1, !cb->turn)
                && !is_square_attacked(cb, d1, !cb->turn)
                && !is_square_attacked(cb, e1, !cb->turn);
            }

        }
        if(cb->turn == BLACK){

            if(cb->piece[!cb->turn][KING] == G8){
                return !is_square_attacked(cb, f8, !cb->turn)
                && !is_square_attacked(cb, g8, !cb->turn)
                && !is_square_attacked(cb, e8, !cb->turn);
            }

            if(cb->piece[!cb->turn][KING] == C8){
                return !is_square_attacked(cb,c8 , !cb->turn)
                && !is_square_attacked(cb,d8 , !cb->turn)
                && !is_square_attacked(cb, e8, !cb->turn);
            }

        }
    }
    return !is_square_attacked(cb, deserialize(cb->piece[!cb->turn][KING]), !cb->turn);
}


void free_moveList(moveList* l){
    free(l);
}

moveList* legalMoveList (chessboard* cb){

    moveList l;
    l.count = 0;
    generateMoves(cb , &l);

    int i = 1;
    ld lostdata = {.castle = -1, .enPassantSquare = -1, .fullmove = -1, .halfmoveclock = -1};
    moveList* ll = createList();

    int cpt = 0;

    while((l.moves[i].from !=  l.moves[i].to) && (i < l.count + 1)){
        makeMove_ld(cb, l.moves[i], &lostdata);
        if(!legalmove_check(cb, l.moves[i])){
            unmakeMove(cb, l.moves[i], &lostdata);
        }
        else{
            ll->moves[cpt] = l.moves[i];
            ll->count++;
            cpt++;
            unmakeMove(cb, l.moves[i], &lostdata);
        }
        i++;
    }
    return ll;
}

moveList* legalCaptureMoveList (chessboard* cb){

    moveList l;
    l.count = 0;
    generateMoves(cb , &l);

    int i = 1;
    ld lostdata = {.castle = -1, .enPassantSquare = -1, .fullmove = -1, .halfmoveclock = -1};
    moveList* ll = createList();
    int cpt = 0;
    //print_lostdata(lostdata);

    //print_move(l->moves[1]);
    while((l.moves[i].from !=  l.moves[i].to) && (i < l.count + 1)){
        if(l.moves[i].captured != 0){
            makeMove_ld(cb, l.moves[i], &lostdata);

            if(!legalmove_check(cb, l.moves[i])){
                unmakeMove(cb, l.moves[i], &lostdata);

            }
            else{
                ll->moves[cpt] = l.moves[i];
                ll->count++;
                cpt++;
                unmakeMove(cb, l.moves[i], &lostdata);

            }
        }
        i++;
    }
    return ll;
}

void affiche_legalmoveList(moveList* l){
    move current;
    int i = 1;
    while(i < l->count + 1){
        current = l->moves[i];
        if((l->moves[i].from == -1) && (l->moves[i].to == -1)){
            i++;
        }
        else{
            printf("[%d][%d][%d][%d][%d][%d]\n", current.from, current.to, current.piece, current.captured, current.promo, current.flag);
            i++;
        }
    }
}
