

// We imagine a 2d letice
// We want to calculate the number of points inside a radius "r" if points are spaced evenly with distance = 1

#include <stdio.h> // #include imports lib
#include <stdlib.h>
#include <string.h>  /* speziell für Funktion memset () */
#include <math.h>


    void c2(int r){ // Funktion welches die Punkte innerhalb eines Radius r prüft
        //double * y = (double *)malloc ( r * r * sizeof(double) );
        for ( int i =-r ; i<=r ; i++){ // Interation der i.ten Richtung
            for ( int j=-r ; j<=r ; j++){ // Interation der j.ten Richtung
                if (i*i + j*j == r){ // Prüfung, ob im radius r die Punkte liegen
                    fprintf(stderr , "pair found: i = %i j = %i \n"  , i , j); // Ausgabe der Gefunden Koordinaten
                }
            }
        }
    }

int main(int argc, char **argv){ // Main Funktion der eine Terminal eingabe annimmt, mit der der Radius gesetzt wird.

    unsigned int a ;
    a = atof (argv[1]);  // a wird auf von eingabe Typ auf integer Typ gecastet
    fprintf(stderr, "Here we have all points inside a circle with radius %f \n", sqrt(a));
    for (int i=0 ; i<=a ; i++){
        fprintf(stderr, "----- C2(%d) -----\n",i);
        c2(i); // Die Funktion c2 wird für alle i bis zur eingegebem radius interriert 
    }
    return 0;
}
