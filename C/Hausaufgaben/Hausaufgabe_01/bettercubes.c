#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int center;
    int edge;
    int face;
    int vertex;
} CubeCounts;

int* cube(int n, CubeCounts *counts);

int main(int argc, char **argv){
    CubeCounts counts = {0, 0, 0, 0};
    unsigned int a = atof(argv[1]);
    fprintf(stderr, "Fuer den Wuerfel mit der Seitenlaenge %d finden wir folgende Punkte. \n", a);
    for (int i=0 ; i<=a ; i++){
        //fprintf(stderr, "----- cube(%d) -----\n",i);
        cube(i, &counts);
    }
    printf("Counts: center=%d, edge=%d, face=%d, vertex=%d\n", counts.center, counts.edge, counts.face, counts.vertex);
    return 0;
}

int* cube(int n, CubeCounts *counts){
    for(int x = -n ; x <= n ; x++){   
        for(int y = -n ; y <= n ; y++){   
            for(int z = -n ; z <= n ; z++){
                if (x == 0 && y == 0 && z == 0) {
                    counts->center++;
                } else if ((x == 0 && y == 0 && (z == n || z == -n))
                           || (x == 0 && z == 0 && (y == n || y == -n))
                           || (y == 0 && z == 0 && (x == n || x == -n))){
                    counts->edge++;
                } else if ((x == 0 && (y == n || y == -n) && (z == n || z == -n))
                           || (y == 0 && (x == n || x == -n) && (z == n || z == -n))
                           || (z == 0 && (x == n || x == -n) && (y == n || y == -n))){
                    counts->face++;
                } else if (x == y && x == z && z == y){
                    counts->vertex++;
                }
                //fprintf(stderr, "(%d;%d;%d;%d)\n", counts->center, counts->edge, counts->face, counts->vertex);
            }
        }
    }
    return 0;
}