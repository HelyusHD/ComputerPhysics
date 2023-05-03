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
//double sum(CubeCounts[], int unsigned length);

int main(int argc, char **argv){
    CubeCounts counts = {0, 0, 0, 0};
    unsigned int a = atof(argv[1]);
    double* weighted = malloc(4*sizeof(int));
    weighted[0] = 0;
    weighted[1] = 0;
    weighted[2] = 0;
    weighted[3] = 0;
    fprintf(stderr, "Fuer den Wuerfel mit der Seitenlaenge %d finden wir folgende Punkte. \n", a);
    for (int i=0 ; i<=a ; i++){
        //fprintf(stderr, "----- cube(%d) -----\n",i);
        cube(i, &counts);
    }
    weighted[0] = counts.center*1;
    weighted[1] = counts.edge*(0.5);
    weighted[2] = counts.face*(0.25);
    weighted[3] = counts.vertex*(0.125);
    printf("Counts: center=%d, edge=%d, face=%d, vertex=%d\n", counts.center, counts.edge, counts.face, counts.vertex);
    printf("weighted: center=%f, edge=%f, face=%f, vertex=%f\n",weighted[0], weighted[1], weighted[2], weighted[3]);

    
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

/* CORRECT EQUATION
double sum(CubeCounts[], int unsigned length){
    double sum_value = 0; // this is a temporary variable for iterating, it has to be initialzed to have a starting point
    
    MAYBE BETTER IN A WHILE LOOP
    for(int i = 0 ; i < length ; i++){
        sum_value = sum_value + (1/(4*M_PI*	8.8541878128*pow(10,-12)))*((FILL IN*1,602 177 33*pow(10,-16) )/FILL IN);
        if (sum_value) {
            printf("Eine Genauigkeit von 10^-5 wurde erreicht.\n");
            break;
        }
    }
    return sum_value; // the function return its solution
}
*/

/*ABORT CRITERIA
    double int_epsabs = 1.e-08;   geforderte max. absolute Aenderung
    double int_epsrel = 1.e-06;   geforderte max. relative Aenderung 


    double diffabs = 2 * int_epsabs;
    double diffrel = 2 * int_epsrel;

    testing if criteria is met
    diffabs = fabs( int_val - val_new );
    diffrel = diffabs / fabs( int_val + val_new ) * 2.; 

    while loop 
    while ( nstep <= int_nstep_max && ( diffrel > int_epsrel || diffabs > int_epsabs ) )
*/