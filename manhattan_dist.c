
#include "manhattan_dist.h"

int manhattan_dist[64][64];

int abs(int a){
    if (a > 0){
        return a;
    }
    else{
        return -a;
    }
}

void fill_manhattan_distances() {
    for (int i = 0; i < 64; i++){
        int file_i = i%8;
        int rank_i = i/8;

        for (int j = 0; j < 64; j++) {
            int file_j = j%8;
            int rank_j = j/8;

            // |x1 - x2| + |y1 - y2|
            manhattan_dist[i][j] = abs(file_i-file_j)+abs(rank_i-rank_j);
        }
    }
}