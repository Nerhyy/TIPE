
#include "eval.h"
#include "manhattan_dist.h"


//CONSTANTE DE PLACEMENT POUR LES BLANCS___________________________________________________


const int w_pawn_place[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50, 
    10, 10, 20, 30, 30, 20, 10, 10, 
     5, 10, 10, 25, 25, 10, 10,  5, 
     0,  0,  0, 25, 25,  0,  0,  0, 
     5, -5, 10,  0,  0, 10, -5,  5, 
     5, 10, 10,-20,-20, 10, 10,  5, 
     0,  0,  0,  0,  0,  0,  0,  0
};

const int w_knight_place[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -40,  5, 10, 15, 15, 10,  5,-40,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};


const int w_bishop_place[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

const int w_rook_place[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
     5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -10,  0,  0,  0,  0,  0,  0, -10,
     0,  0,  0,  5,  5,  0,  0,  0
};

const int w_queen_place[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -5,  0,  5,  5,  5,  5,  0, -5,
     0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20

};

const int w_king_place[64] = {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    20, 20,  0,  0,  0,  0, 20, 20,
    20, 40, 10,  0,  0, 10, 40, 20
};

//CONSTANTE DE PLACEMENT POUR LES NOIRS___________________________________________________

const int b_pawn_place[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
     5, 10, 10,-20,-20, 10, 10,  5, 
     5, -5, 10,  0,  0, 10, -5,  5, 
     0,  0,  0, 25, 25,  0,  0,  0, 
     5,  5, 10, 25, 25, 10,  5,  5,
    10, 10, 20, 30, 30, 20, 10, 10,
    50, 50, 50, 50, 50, 50, 50, 50,
     0,  0,  0,  0,  0,  0,  0,  0
};

const int b_knight_place[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -40,  5, 10, 15, 15, 10,  5,-40,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

const int b_bishop_place[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};


const int b_rook_place[64] = {
     0,  0,  0,  5,  5,  0,  0,  0,
    -10,  0,  0,  0,  0,  0,  0, -10,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
     5, 10, 10, 10, 10, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0,
};

const int b_queen_place[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -10,  5,  5,  5,  5,  5,  0,-10,
     0,  0,  5,  5,  5,  5,  0, -5,
    -5,  0,  5,  5,  5,  5,  0, -5,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20,
    
};

const int b_king_place[64] = {
    20, 40, 10,  0,  0, 10, 40, 20,
    20, 20,  0,  0,  0,  0, 20, 20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30
};

const int* all_placements[2][8] = {
    // ================= BLANCS =================
    [WHITE] = {
        [0] = 0,
        [PAWN]         = w_pawn_place,
        [KNIGHT]       = w_knight_place,
        [BISHOP]       = w_bishop_place,
        [ROOK]         = w_rook_place,
        [QUEEN]        = w_queen_place,
        [KING]         = w_king_place,
    },

    // ================= NOIRS ==================
    [BLACK] = {
        [0] = 0,
        [PAWN]         = b_pawn_place,
        [KNIGHT]       = b_knight_place,
        [BISHOP]       = b_bishop_place,
        [ROOK]         = b_rook_place,
        [QUEEN]        = b_queen_place,
        [KING]         = b_king_place,
    }
};


//________________________________ ENDGAME ______________________________

const int w_pawn_place_endgame[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
     80, 80, 80, 80, 80, 80, 80, 80,
     50, 50, 50, 50, 50, 50, 50, 50,
     30, 30, 30, 30, 30, 30, 30, 30,
     20, 20, 20, 20, 20, 20, 20, 20,
     10, 10, 10, 10, 10, 10, 10, 10,
     10, 10, 10,-20,-20, 10, 10, 10,
      0,  0,  0,  0,  0,  0,  0,  0
};

const int w_knight_place_endgame[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

const int w_bishop_place_endgame[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5, 10, 10, 10, 10,  5,-10,
    -10,  5, 10, 10, 10, 10,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

const int w_rook_place_endgame[64] = {
     20, 20, 20, 20, 20, 20, 20, 20,
     50, 50, 50, 50, 50, 50, 50, 50,
      0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0
};

const int w_queen_place_endgame[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -10,  5, 10, 10, 10, 10,  5,-10,
     -5,  5, 10, 15, 15, 10,  5, -5,
     -5,  5, 10, 15, 15, 10,  5, -5,
    -10,  5, 10, 10, 10, 10,  5,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};

const int w_king_place_endgame[64] = {
    -50,-40,-30,-20,-20,-30,-40,-50,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -50,-30,-30,-30,-30,-30,-30,-50
};

const int b_pawn_place_endgame[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
     10, 10, 10,-20,-20, 10, 10, 10,
     10, 10, 10, 10, 10, 10, 10, 10,
     20, 20, 20, 20, 20, 20, 20, 20,
     30, 30, 30, 30, 30, 30, 30, 30,
     50, 50, 50, 50, 50, 50, 50, 50,
     80, 80, 80, 80, 80, 80, 80, 80,
      0,  0,  0,  0,  0,  0,  0,  0
};

const int b_knight_place_endgame[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

const int b_bishop_place_endgame[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5, 10, 10, 10, 10,  5,-10,
    -10,  5, 10, 10, 10, 10,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

const int b_rook_place_endgame[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,
     50, 50, 50, 50, 50, 50, 50, 50,
     20, 20, 20, 20, 20, 20, 20, 20
};

const int b_queen_place_endgame[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -10,  5, 10, 10, 10, 10,  5,-10,
     -5,  5, 10, 15, 15, 10,  5, -5,
     -5,  5, 10, 15, 15, 10,  5, -5,
    -10,  5, 10, 10, 10, 10,  5,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};

const int b_king_place_endgame[64] = {
    -50,-30,-30,-30,-30,-30,-30,-50,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -50,-40,-30,-20,-20,-30,-40,-50,
};

const int* all_placements_endgame[2][8] = {
    // ================= BLANCS =================
    [WHITE] = {
        [0] = 0,
        [PAWN]         = w_pawn_place_endgame,
        [KNIGHT]       = w_knight_place_endgame,
        [BISHOP]       = w_bishop_place_endgame,
        [ROOK]         = w_rook_place_endgame,
        [QUEEN]        = w_queen_place_endgame,
        [KING]         = w_king_place_endgame,
    },

    // ================= NOIRS ==================
    [BLACK] = {
        [0] = 0,
        [PAWN]         = b_pawn_place_endgame,
        [KNIGHT]       = b_knight_place_endgame,
        [BISHOP]       = b_bishop_place_endgame,
        [ROOK]         = b_rook_place_endgame,
        [QUEEN]        = b_queen_place_endgame,
        [KING]         = b_king_place_endgame,
    }
};

const int piece_value[7] = {0,100,320,350,520,1000,200000}; //[EMPTY][PAWN][KNIGHT][BISHOP][ROOK][QUEEN][KING]

//score pour un camp sur la pos de départ : 204 085

int piece_eval(U64 piece[2][7]){

    int score = 0; //Score calculé

        //Heuristique matérielle pour les blancs
        for(int i = 1; i < 7 ; i++){   //On itère sur les pièces

            U64 tempbitboard = piece[WHITE][i];

            if(piece[WHITE][i] != 0){        //On check si le masque est vide (OPTIMISATION)

                while(tempbitboard){
                    int sq = __builtin_ctzll(tempbitboard);
                    score += piece_value[i] + all_placements[WHITE][i][sq];
                    //printf("score [0][%d][%d] : %f", i, cur_sq ,score);
                    tempbitboard &= (tempbitboard - 1);
                }
                 
            }
    
        }

        

        //Heuristique matérielle pour les noirs
        for(int i = 1; i < 7 ; i++){   //On itère sur les pièces

            U64 tempbitboard = piece[BLACK][i];

            if(piece[BLACK][i] != 0){        //On check si le masque est vide (OPTIMISATION)

                while(tempbitboard){

                    int sq = __builtin_ctzll(tempbitboard);
                    score -= piece_value[i] + all_placements[BLACK][i][sq];
                    //printf("score [1][%d][%d] : %f", i, cur_sq ,score);
                    tempbitboard &= (tempbitboard - 1);

                }

            }
    
        }

    return score;

}

int piece_eval_endgame(U64 piece[2][7]){

    int score = 0; //Score calculé

        //Heuristique matérielle pour les blancs
        for(int i = 1; i < 7 ; i++){   //On itère sur les pièces

            U64 tempbitboard = piece[WHITE][i];

            if(piece[WHITE][i] != 0){        //On check si le masque est vide (OPTIMISATION)

                while(tempbitboard){       //Tant que le mask iter est dans la range de l'echiquier
                    int sq = __builtin_ctzll(tempbitboard);
                    score += piece_value[i] + all_placements_endgame[WHITE][i][sq];
                    //printf("score [0][%d][%d] : %f", i, cur_sq ,score);
                    tempbitboard &= (tempbitboard - 1);
                }
                 
            }
    
        }

        

        //Heuristique matérielle pour les noirs
        for(int i = 1; i < 7 ; i++){   //On itère sur les pièces

            U64 tempbitboard = piece[BLACK][i];

            if(piece[BLACK][i] != 0){        //On check si le masque est vide (OPTIMISATION)

                while(tempbitboard){       //Tant que le mask iter est dans la range de l'echiquier

                    int sq = __builtin_ctzll(tempbitboard);
                    score -= piece_value[i] + all_placements_endgame[BLACK][i][sq];
                    //printf("score [1][%d][%d] : %f", i, cur_sq ,score);
                    tempbitboard &= (tempbitboard - 1);

                }

            }
    
        }

    return score;

}

int double_pawns(U64 piece[2][7]){

    int score = 0;

    for(int i = 0; i < 8; i++){
        int popcw = popCount(piece[WHITE][PAWN] & files[i]);
        int popcb = popCount(piece[BLACK][PAWN] & files[i]);

        if(popcw >= 2){

            if(popcw >= 3){ //Pions triplés ou plus
                score -= 60;
            }
            else{ //Pions doublés
                score -= 30;
            }

        }

        if(popcb >= 2){

            if(popcb >= 3){ //Pions triplés ou plus
                score += 60;
            }
            else{ //Pions doublés
                score += 30;
            }

        }
    }

    return score;

}

void open_files(U64 piece[2][7], bool* white_openfiles, bool* black_openfiles){ //Remplit les tableaux

    for(int i = 0; i < 8; i++){ //Il y a t il des pions sur les colonnes ?
        white_openfiles[i] = (piece[WHITE][PAWN] & files[i]) != 0;
        black_openfiles[i] = (piece[BLACK][PAWN] & files[i]) != 0;
    }

}

int isolated_pawns(U64 piece[2][7]){ //A FACTORISER AVEC LES ROOKS ON OPEN FILES

    int score = 0;

    bool istherepawnfileiw[8];
    bool istherepawnfileib[8];

    open_files(piece, istherepawnfileiw, istherepawnfileib);

    for(int i = 1 ; i < 7; i++){

        bool possede_voisin_w = false;
        bool possede_voisin_b = false;
        possede_voisin_w = istherepawnfileiw[i - 1] || istherepawnfileiw[i + 1];
        possede_voisin_b = istherepawnfileib[i - 1] || istherepawnfileib[i + 1];

        if(istherepawnfileiw[i] && !possede_voisin_w){
            score -= 15;
        }
        if(istherepawnfileib[i] && !possede_voisin_b){
            score += 15;
        }
    }
    if(istherepawnfileiw[0] && !istherepawnfileiw[1]){
        score -= 15;
    }
    if(istherepawnfileib[0] && !istherepawnfileiw[1]){
        score += 15;
    }
    if(istherepawnfileiw[7] && !istherepawnfileiw[6]){
        score -= 15;
    }
    if(istherepawnfileib[7] && !istherepawnfileiw[6]){
        score += 15;
    }
    return score;

}

int bishop_pair(int piece_count[2][7]){

    int score = 0;

    if (piece_count[WHITE][BISHOP] == 2){
        score += 50;
    }
    if(piece_count[BLACK][BISHOP] == 2){
        score -= 50;
    }

    return score;

}

int rook_on_open_files(U64 piece[2][7]){

    int score = 0;

    bool istherepawnfileiw[8];
    bool istherepawnfileib[8];

    open_files(piece, istherepawnfileiw, istherepawnfileib); //Je pense on peut s'en passer

    for(int i = 0; i < 8; i++){
        if(((files[i] & piece[WHITE][ROOK]) != 0) && (!(istherepawnfileiw[i]))){ //Il y a une tour sur la colonne, et il y n'a pas de pions sur la colonnes (du camps)
            if(!istherepawnfileib[i]){ //y'a aucun pion sur la colonne
                score += 20;
            } 
            else{
                score += 10;
            }
        }
        if(((files[i] & piece[BLACK][ROOK]) != 0) && (!(istherepawnfileiw[i] & istherepawnfileib[i]))){ //Il y a une tour sur la colonne, et il y n'a pas de pions sur la colonnes (du camps)
            if(!istherepawnfileiw[i]){ //y'a aucun pion sur la colonne
                score -= 20;
            }
            else{
                score -= 10;
            }
        }
    }

    return score;

}

int bastion(U64 piece[2][7]){ //Si un cavalier ou bishop est protégé par un pion loin dans le camps adverse

    int score = 0;

    U64 wknights = piece[WHITE][KNIGHT] & (row5 | row6 | row7); //On prend les knights sur la 5eme et 6 rangée
    U64 bknights = piece[BLACK][KNIGHT] & (row4 | row3 | row2); //On prend les knights sur la 5eme et 6 rangée

    while(wknights){
        int sq = popLSB(&wknights);

        if((pawnAttacks[BLACK][sq] & piece[WHITE][PAWN]) != 0) { //On regarde si des pions protège le cavalier (bastion)
            if((arrFrontSpans[WHITE][sq] & piece[BLACK][PAWN]) == 0){ //Il y a t il un pion en face du bastion ?
                score += 40; //A REDEFINIR
            }
            else{
                score+= 15;
            }
        }
    }

    while(bknights){
        int sq = popLSB(&bknights);


        if((pawnAttacks[WHITE][sq] & piece[BLACK][PAWN]) != 0) { //On regarde si des pions protège le cavalier (bastion)
            if((arrFrontSpans[BLACK][sq] & piece[WHITE][PAWN]) == 0){ //Il y a t il un pion en face du bastion ?
                score -= 40; //A REDEFINIR
            }
            else{
                score -= 15;
            }
        }
    }

    return score;
}


int passed_pawns(U64 piece[2][7]){ // A FACTORISER

    int score = 0;

    U64 wpawns = piece[WHITE][PAWN];
    U64 bpawns = piece[BLACK][PAWN];

    while(wpawns){
        int sq = __builtin_ctzll(wpawns);
        if((arrFrontSpans[WHITE][sq] & piece[BLACK][PAWN]) == 0){
            score += 20; //A MODIFIER
        }
        wpawns &= (wpawns -1);
    }

    while(bpawns){
        int sq = __builtin_ctzll(bpawns);
        if((arrFrontSpans[BLACK][sq] & piece[WHITE][PAWN]) == 0){
            score -= 20; //A MODIFIER
        }
        bpawns &= (bpawns -1);
    }


    return score;
}

int passed_pawns_endgame(U64 piece[2][7]){ // A FACTORISER

    int score = 0;

    U64 wpawns = piece[WHITE][PAWN];
    U64 bpawns = piece[BLACK][PAWN];

    while(wpawns){
        int sq = __builtin_ctzll(wpawns);
        if((arrFrontSpans[WHITE][sq] & piece[BLACK][PAWN]) == 0){
            score += 40; //A MODIFIER
        }
        wpawns &= (wpawns -1);
    }

    while(bpawns){
        int sq = __builtin_ctzll(bpawns);
        if((arrFrontSpans[BLACK][sq] & piece[WHITE][PAWN]) == 0){
            score -= 40; //A MODIFIER
        }
        bpawns &= (bpawns -1);
    }


    return score;
}



int rook_on_7th(U64 piece[2][7]){

    int score = 0;

    if(popCount(piece[WHITE][ROOK] & row7) == 2){ //Si deux tours sont sur la 7eme rangée
        score +=10;
        if((piece[BLACK][KING] & row8) != 0){ //Si le roi est sur la 8eme rangée
            score += 20;
        }
    }

    if(popCount(piece[BLACK][ROOK] & row2) == 2){ //Si deux tours sont sur la 2eme rangée
        score -=10;
        if((piece[WHITE][KING] & row1) != 0){ //Si le roi est sur la 8eme rangée
            score -= 20;
        }
    }

    return score;

}

int doubled_rooks(U64 piece[2][7]){
    
    int score= 0;
    
    for(int i = 0; i < 8; i++){
        if(popCount(piece[WHITE][ROOK] & files[i]) >= 2){
            score+= 15;
        }
        if(popCount(piece[BLACK][ROOK] & files[i]) >= 2){
            score-= 15;
        }
    }

    return score;

}

int mobility (U64 piece[2][7]){

    int score = 0;
    U64 w_p_attacks = wp_attacks_all(piece[WHITE][PAWN]);
    U64 b_p_attacks = bp_attacks_all(piece[BLACK][PAWN]);

    U64 all_w_pieces = whitePieces_p(piece);
    U64 all_b_pieces = blackPieces_p(piece);

    U64 occ = all_w_pieces | all_b_pieces;

    U64 wknights = piece[WHITE][KNIGHT] ;
    U64 wrooks   = piece[WHITE][ROOK]   ;
    U64 wbishops = piece[WHITE][BISHOP] ;

    U64 bknights = piece[BLACK][KNIGHT] ;
    U64 brooks   = piece[BLACK][ROOK]   ;
    U64 bbishops = piece[BLACK][BISHOP] ;

    while(wknights){

        int sq = popLSB(&wknights);

        U64 moves = knightAttacks[sq];

        score += 4*popCount((moves & (~b_p_attacks)) & (~all_w_pieces));
        
    }
    while(wrooks){

        int sq = popLSB(&wrooks);

        U64 moves = rookAttacks(occ, sq);

        score += 2*popCount((moves & (~b_p_attacks)) & (~all_w_pieces));

    }
    while(wbishops){

        int sq = popLSB(&wbishops);

        U64 moves = bishopAttacks(occ, sq);

        score += 3*popCount((moves & (~b_p_attacks)) & (~all_w_pieces));

    }
    while(bknights){

        int sq = popLSB(&bknights);

        U64 moves = knightAttacks[sq];

        score -= 4*popCount((moves & (~w_p_attacks)) & (~all_b_pieces));

    }
    while(brooks){

        int sq = popLSB(&brooks);

        U64 moves = rookAttacks(occ, sq);

        score -= 2*popCount((moves & (~w_p_attacks)) & (~all_b_pieces));

    }
    while(bbishops){

        int sq = popLSB(&bbishops);

        U64 moves = bishopAttacks(occ, sq);

        score -= 3*popCount((moves & (~w_p_attacks)) & (~all_b_pieces));

    }

    return score;

}


int pawn_chain(U64 piece[2][7]){
    int score = 0;

    U64 w_defended_west = w_p_defended_from_west(piece[WHITE][PAWN]);
    U64 w_defended_east = w_p_defended_from_east(piece[WHITE][PAWN]);
    U64 b_defended_west = b_p_defended_from_west(piece[BLACK][PAWN]);
    U64 b_defended_east = b_p_defended_from_east(piece[BLACK][PAWN]);

    U64 w_defenders_west = w_p_defenders_from_west(piece[WHITE][PAWN]);
    U64 w_defenders_east = w_p_defenders_from_east(piece[WHITE][PAWN]);
    U64 b_defenders_west = b_p_defenders_from_west(piece[BLACK][PAWN]);
    U64 b_defenders_east = b_p_defenders_from_east(piece[BLACK][PAWN]);

    score += 30*popCount(defended_defenders_west(w_defended_west, w_defenders_west) || defended_defenders_east(w_defended_east,w_defenders_east));
    score -= 30*popCount(defended_defenders_west(b_defended_west, b_defenders_west) || defended_defenders_east(b_defended_east,b_defenders_east));

    score += 20*popCount(defended_ndefenders_west(w_defended_west, w_defenders_west) || defended_ndefenders_east(w_defended_east, w_defenders_east));
    score -= 20*popCount(defended_ndefenders_west(b_defended_west, b_defenders_west) || defended_ndefenders_east(b_defended_east, b_defenders_east));

    score += 12*popCount(ndefended_defenders_west(w_defended_west, w_defenders_west) || ndefended_defenders_east(w_defended_east, w_defenders_east));
    score -= 12*popCount(ndefended_defenders_west(b_defended_west, b_defenders_west) || ndefended_defenders_east(b_defended_east, b_defenders_east));

    return score;
}

int pawn_eval(U64 piece[2][7]){
    int score = 0;
    score += isolated_pawns(piece);
    score += double_pawns(piece);
    score += passed_pawns(piece);
    score += pawn_chain(piece);
    
    return score;
}

int pawn_eval_endgame(U64 piece[2][7]){
    int score = 0;
    score += isolated_pawns(piece);
    score += double_pawns(piece);
    score += passed_pawns_endgame(piece);
    score += pawn_chain(piece);
    
    return score;
}

int other_eval(U64 piece[2][7], int piece_count[2][7]){

    int score =0;

    score += doubled_rooks(piece);
    score += rook_on_7th(piece);
    score += bastion(piece);
    score += rook_on_open_files(piece);
    score += bishop_pair(piece_count);

    return score;
}

int castling_eval(chessboard* cb){

    int score =0;
    if(((cb->castle & 0x0001) == 0) && ((rectLookup[d8][g8] && cb->piece[BLACK][KING]) != 0)){
        score += 20;
    }
    if(((cb->castle & 0x0010) == 0) && ((rectLookup[f8][b8] && cb->piece[BLACK][KING]) != 0)){
        score += 20;
    }
    if(((cb->castle & 0x0100) == 0) && ((rectLookup[d1][g1] && cb->piece[WHITE][KING]) != 0) ){
        score -= 20;
    }
    if(((cb->castle & 0x1000) == 0) && ((rectLookup[f1][b1] && cb->piece[WHITE][KING]) != 0) ){
        score -= 20;
    }

    return score;
}

//Retourne la phase du jeu (24 c'est le début, 0 la fin)
int get_game_phase(chessboard* cb){
    
    int phase = 0; //entre 0 à 24

    phase += ((cb->piece_count[WHITE][KNIGHT])+(cb->piece_count[BLACK][KNIGHT]));
    phase += ((cb->piece_count[WHITE][BISHOP])+(cb->piece_count[BLACK][BISHOP]));
    phase += ((cb->piece_count[WHITE][ROOK])  +(cb->piece_count[BLACK][ROOK]))*2;
    phase += ((cb->piece_count[WHITE][QUEEN]) +(cb->piece_count[BLACK][QUEEN]))*4;

    return phase;
}

const int king_centre_dist[64] = {
        6, 5, 4, 3, 3, 4, 5, 6,
        5, 4, 3, 2, 2, 3, 4, 5,
        4, 3, 1, 0, 0, 1, 3, 4,
        3, 2, 0, 0, 0, 0, 2, 3,
        3, 2, 0, 0, 0, 0, 2, 3,
        4, 3, 1, 0, 0, 1, 3, 4,
        5, 4, 3, 2, 2, 3, 4, 5,
        6, 5, 4, 3, 3, 4, 5, 6
};

int white_material(chessboard* cb){
    return cb->piece_count[WHITE][PAWN]*piece_value[PAWN]
            + cb->piece_count[WHITE][KNIGHT]*piece_value[KNIGHT]
            + cb->piece_count[WHITE][BISHOP]*piece_value[BISHOP]
            + cb->piece_count[WHITE][ROOK]*piece_value[ROOK]
            + cb->piece_count[WHITE][QUEEN]*piece_value[QUEEN];

}

int black_material(chessboard* cb){
    return cb->piece_count[BLACK][PAWN]*piece_value[PAWN]
            + cb->piece_count[BLACK][KNIGHT]*piece_value[KNIGHT]
            + cb->piece_count[BLACK][BISHOP]*piece_value[BISHOP]
            + cb->piece_count[BLACK][ROOK]*piece_value[ROOK]
            + cb->piece_count[BLACK][QUEEN]*piece_value[QUEEN];

}

int material_advantage_is_huge(chessboard* cb){ // -1 pour None, 0 pour WHITE et 1 pour BLACK

    int w_m = white_material(cb);
    int b_m = black_material(cb);

    int diff = w_m - b_m;

    if((diff > 450) && (b_m < 400)){
        return WHITE;
    }

    else if((diff < -450) && (w_m < 400)){
        return BLACK;
    }

    else{
        return -1;
    }

}

int mop_up(U64 piece[2][7], int winning_side){

    int score = 0;

    int l_king_sq = deserialize(piece[!winning_side][KING]);
    int w_king_sq = deserialize(piece[ winning_side][KING]);

    //On veut ramener le roi losing vers les bords
    score += king_centre_dist[l_king_sq]*45;

    //On rapproche les rois
    score += (14 - manhattan_dist[w_king_sq][l_king_sq])*40;

    return score;
}

int eval_midgame(chessboard* cb){

    int score = 0;
   
    score += piece_eval(cb->piece);
    score += mobility(cb->piece);
    score += pawn_eval(cb->piece);
    score += other_eval(cb->piece, cb->piece_count);

    //score += castling_eval(cb); //A AJOUTER

    return score;

}

int eval_endgame(chessboard* cb){

    int score = 0;
   
    score += piece_eval_endgame(cb->piece);
    score += mobility(cb->piece);
    score += pawn_eval_endgame(cb->piece);
    score += other_eval(cb->piece, cb->piece_count);


    int maih = material_advantage_is_huge(cb); // -1 pour None, 0 pour WHITE et 1 pour BLACK
    if(maih != -1){
        (maih == WHITE) ? (score += mop_up(cb->piece, maih)) : (score -= mop_up(cb->piece, maih));
    }

    return score;

}

int eval(chessboard* cb){

    int phase = cb->phase; //On récupère la phase entre 0 et 24

    int score_mg = eval_midgame(cb); //Eval midgame
    int score_eg = eval_endgame(cb); //Eval endgame

    int final_score = ((score_mg*phase)+(score_eg*(24-phase)))/24; //On interpole

    return (cb->turn == WHITE) ? final_score : -final_score;

     //return (cb->turn == WHITE) ? score_mg : -score_mg; //décommenter les trucs pour remettre l'endgame

}