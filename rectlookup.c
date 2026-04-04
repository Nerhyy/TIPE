#include "rectlookup.h"

#include "shift.h"
#include "utils.h"
#include "cb_operators.h"
#include "globals.h"

//RECTLOOKUP

U64 rectLookup[64][64];


void fill_rectLookup_w_zeros(){
    for(int i = 0; i < 64 ; i++){
        for(int j = 0; j < 64; j++){
            rectLookup[i][j] = 0;
        }
    }    
}

bool edge_compare(int i){
    return     i != 0
            && i != 1
            && i != 2
            && i != 3
            && i != 4
            && i != 5
            && i != 6
            && i != 7
            && i != 8
            && i != 16
            && i != 24
            && i != 32
            && i != 40
            && i != 48
            && i != 56
            && i != 57
            && i != 58
            && i != 59
            && i != 60
            && i != 61
            && i != 62
            && i != 63
            && i != 55
            && i != 47
            && i != 39
            && i != 31
            && i != 23
            && i != 15;
}
void fill_rectLookup() {
    fill_rectLookup_w_zeros();
    for(int i = 0; i < 64 ; i++) {
        U64 startingpoint_i = 1ULL << i;

        //DROITE (EST)
        U64 pivot = 0;
        int j = i + 1;
        while(j < 8*(i/8) + 8) {
            rectLookup[i][j] = pivot;
            pivot |= (1ULL << j);
            j++;
        }

        //GAUCHE (OUEST)
        pivot = 0;
        int k = i - 1;
        while(k >= 8*(i/8)) {
            rectLookup[i][k] = pivot;
            pivot |= (1ULL << k);
            k--;
        }

        //HAUT (NORD)
        pivot = 0;
        int l = i + 8;
        while(l < 64) {
            rectLookup[i][l] = pivot;
            pivot |= (1ULL << l);
            l += 8;
        }

        //BAS (SUD)
        pivot = 0;
        int m = i - 8;
        while(m >= 0) {
            rectLookup[i][m] = pivot;
            pivot |= (1ULL << m);
            m -= 8;
        }

        //DIAG NORD-EST
        pivot = 0;
        int n = i + 9;
        while(n < 64 && (n % 8) > (i % 8)) { 
            rectLookup[i][n] = pivot;
            pivot |= (1ULL << n);
            if ((n % 8) == 7){
                 break;
            }
            n += 9;
        }

        //DIAG SUD-OUEST
        pivot = 0;
        int p = i - 9;
        while(p >= 0 && (p % 8) < (i % 8)) {
            rectLookup[i][p] = pivot;
            pivot |= (1ULL << p);
            if ((p % 8) == 0){
                 break;
            }
            p -= 9;
        }

        //DIAG NORD-OUEST
        pivot = 0;
        int q = i + 7;
        while(q < 64 && (q % 8) < (i % 8)) {
            rectLookup[i][q] = pivot;
            pivot |= (1ULL << q);
            if ((q % 8) == 0){
                break;
            }
            q += 7;
        }

        //DIAG SUD-EST
        pivot = 0;
        int r = i - 7;
        while(r >= 0 && (r % 8) > (i % 8)) {
            rectLookup[i][r] = pivot;
            pivot |= (1ULL << r);
            if ((r % 8) == 7){
                 break;
            }
            r -= 7;
        }
    }
}

/*void affichage_rect(){
    for(int i = 0 ; i < 64 ; i++){
        for(int j = 0 ; j < 64 ; j++){
            char* stringIter = fprintf_mask(rectLookup[i][j]);

          FILE *pF = fopen("./rect.txt", "a");
          fprintf(pF, "\n");
          fprintf(pF, "i : %d \t j : %d\n", i, j);
          
          int k = 57;
          for(int m = 8; m >= 1; m--){
            for(i ; k <= 8*m; k++){ //On itere sur les bits
              fprintf(pF, "%d", stringIter[k]);
            }
            fprintf(pF, "\n");
            k -= 16;
          }

          fclose(pF);
        }
    }
}*/

//Preconditions: avoir remplit l'array de zeros
U64 InBetween(int from, int to){ //from et to sont les cases sérialisées
    return rectLookup[from][to];
}

bool mayMove(int from, int to, U64 occ){
    return (InBetween(from, to) & occ) == 0; //le cas de true correspond à aucune pieces sur le chemin
}