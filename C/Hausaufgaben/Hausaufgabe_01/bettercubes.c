#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int center;
    int edge;
    int face;
    int corner;
} CubeCounts; // definition eines eigenes Types um die Gewichtung zu berechnen

int* cube(int n, CubeCounts *counts); // Funktion der uns die Anzahl an Mittel-, Seiten-, Kanten- und Eckpunkten ausgibt

int main(int argc, char **argv){
    CubeCounts counts = {0, 0, 0, 0};
    unsigned int a = atof(argv[1]);
    fprintf(stderr, "Fuer den Wuerfel mit der Seitenlaenge %d finden wir folgende Punkte. \n", a);
    for (int i=0 ; i<=a ; i++){
        //fprintf(stderr, "----- cube(%d) -----\n",i);
        cube(i, &counts);
    }
    printf("Counts: center=%d, edge=%d, face=%d, vertex=%d\n", counts.center, counts.edge, counts.face, counts.corner);

    
    return 0;
}

int* cube(int n, CubeCounts *counts){
    for(int x = -n ; x <= n ; x++){   
        for(int y = -n ; y <= n ; y++){   
            for(int z = -n ; z <= n ; z++){ // Itteration des 3D Raums 
                if (x == 0 && y == 0 && z == 0) {
                    counts->center++; // Prüfe ob es sich bei der Koordinate um einen Mittelpunkt handelt
                } else if ((x == 0 && y == 0 && (z == n || z == -n))
                           || (x == 0 && z == 0 && (y == n || y == -n))
                           || (y == 0 && z == 0 && (x == n || x == -n))){
                    counts->edge++; // Prüfe ob es sich bei der Koordinate um einen Seitenpunkt handelt
                } else if ((x == 0 && (y == n || y == -n) && (z == n || z == -n))
                           || (y == 0 && (x == n || x == -n) && (z == n || z == -n))
                           || (z == 0 && (x == n || x == -n) && (y == n || y == -n))){
                    counts->face++; // Prüfe ob es sich bei der Koordinate um einen Kantenpunkt handelt
                } else if (x == y && x == z && z == y){
                    counts->corner++; // Prüfe ob es sich bei der Koordinate um einen Eckpunkt handelt
                }
                //fprintf(stderr, "(%d;%d;%d;%d)\n", counts->center, counts->edge, counts->face, counts->vertex);
            }
        }
    }
    return 0;
}


