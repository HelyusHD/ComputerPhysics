/********************************************************************************
 Programm kompelier Befehl:
 gcc -Wall -pedantic madelung.c -o madelung -lm

 Programm aufruf Befehl:
 madelung 
 ********************************************************************************/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int center;
    int edge;
    int face;
    int corner;
} CubeCounts; // Type definiert um die Gewichteten Werte zu bestimmten

int* cube(int n, CubeCounts *counts); // Funktion welches jeder Koordinate deren Positionierung zuweist 
//double sum(CubeCounts[], int unsigned length);

int main(int argc, char **argv){ 
    CubeCounts counts = {0, 0, 0, 0}; // Counts sind die Anzahl an Stellen, diese werden mit 0 Initialisiert
    // seitenlaenge sollte von abbruchkrit. abh unsigned int seitenlaenge = atof(argv[1]); // Terminaleingabe der Seitenlaenge des Würfels wird eingelesen
    int seitenlaenge = //NOCH EINTRAGEN
    double* weighted = malloc(4*sizeof(int)); // Speicherreservierung für die gewichteten Werte
    weighted[0] = 0;
    weighted[1] = 0;
    weighted[2] = 0;
    weighted[3] = 0; // gewichtete Werte werden mit 0 initialisiert
    fprintf(stderr, "Fuer den Wuerfel mit der Seitenlaenge %d finden wir folgende Punkte. \n", seitenlaenge); 
    for (int i=0 ; i<=seitenlaenge ; i++){ // Schleife die die Funktions cube über i bis eingegebener Seitenlaenge itteriert 
        cube(i, &counts);// Die Funktion Cube wird mit den Parametern i(aktuelle itterierte Seitenlaenge) aufgerufen und schreibt auf die Adresse von Counts ein.
    }
    weighted[0] = counts.center*1; // Mittelpunkte werden mit dem Faktor 1 gewichtet
    weighted[1] = counts.edge*(0.5); // Seitenpunkte werden mit dem Faktor 0.5 gewichtet
    weighted[2] = counts.face*(0.25); // Kantenpunkte werden mit dem Faktor 0.25 gewichtet
    weighted[3] = counts.corner*(0.125); // Eckpunkte werden mit dem Faktor 0.125 gewichtet
    printf("Counts: center=%d, edge=%d, face=%d, vertex=%d\n", counts.center, counts.edge, counts.face, counts.corner);
    printf("weighted: center=%f, edge=%f, face=%f, vertex=%f\n",weighted[0], weighted[1], weighted[2], weighted[3]);

    
    return 0;
}

int* cube(int n, CubeCounts *counts){ // Funktion cube nimmt eine bestimmte Seitenlaenge n und gibt die Anzahl an Mittel-, Seiten-, Kanten- und Eckpunkten zurück
    for(int x = -n ; x <= n ; x++){   
        for(int y = -n ; y <= n ; y++){   
            for(int z = -n ; z <= n ; z++){
                if (x == 0 && y == 0 && z == 0) {
                    counts->center++; // Anzahl an Mittelpunkten wird erhöht wenn die Bedingung gilt.
                } else if ((x == 0 && y == 0 && (z == n || z == -n))
                           || (x == 0 && z == 0 && (y == n || y == -n))
                           || (y == 0 && z == 0 && (x == n || x == -n))){
                    counts->edge++; // Anzahl an Seitenpunkten wird erhöht wenn die Bedingung gilt.
                } else if ((x == 0 && (y == n || y == -n) && (z == n || z == -n))
                           || (y == 0 && (x == n || x == -n) && (z == n || z == -n))
                           || (z == 0 && (x == n || x == -n) && (y == n || y == -n))){
                    counts->face++; // Anzahl an Kantenpunkten wird erhöht wenn die Bedingung gilt.
                } else if (x == y && x == z && z == y){
                    counts->corner++; // Anzahl an Eckpunkten wird erhöht wenn die Bedingung gilt.
                }
                //fprintf(stderr, "(%d;%d;%d;%d)\n", counts->center, counts->edge, counts->face, counts->vertex);
            }
        }
    }
    return 0;
}

/* CORRECT EQUATION
double sum(weight[], int unsigned nstep){
    double sum_value = 0; // this is a temporary variable for iterating, it has to be initialzed to have a starting point
    int unsigned nstep_max = 1000 // einfach ein sehr großer Wert
    while ( nstep <= int_nstep_max && ( diffrel > int_epsrel ) ){
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
    wir wollen nur den relativen Fehlerbetrachten
*/