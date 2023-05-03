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



double cube(int n){ // Funktion cube nimmt eine bestimmte Seitenlaenge n und gibt die Anzahl an Mittel-, Seiten-, Kanten- und Eckpunkten zurück
    double SurfacePotential = 0;
    int sign = 1;
        int x = n;
        for (int y=-n ; y<=n ; y++){
            for (int z=-n ; z<=n ; z++){
                if (abs(x+y+z)%2 == 0){sign = -1;}else{sign = 1;} // keeping track of the charge of the ions at a lattice point
                SurfacePotential += (sign / sqrt(x*x + y*y + z*z));
                }
            }
        x = -n;
        for (int y=-n ; y<=n ; y++){
            for (int z=-n ; z<=n ; z++){
                if (abs(x+y+z)%2 == 0){sign = -1;}else{sign = 1;} // keeping track of the charge of the ions at a lattice point
                SurfacePotential += (sign / sqrt(x*x + y*y + z*z));
                }
            }
        int y = n;
        for (int x=-n+1 ; x<=n-1 ; x++){
            for (int z=-n ; z<=n ; z++){
                if (abs(x+y+z)%2 == 0){sign = -1;}else{sign = 1;} // keeping track of the charge of the ions at a lattice point
                SurfacePotential += (sign / sqrt(x*x + y*y + z*z));
                }
            }
        y = -n;
        for (int x=-n+1 ; x<=n-1 ; x++){
            for (int z=-n ; z<=n ; z++){
                if (abs(x+y+z)%2 == 0){sign = -1;}else{sign = 1;} // keeping track of the charge of the ions at a lattice point
                SurfacePotential += (sign / sqrt(x*x + y*y + z*z));
                }
            }
        int z = n;
        for (int x=-n+1 ; x<=n-1 ; x++){
            for (int y=-n+1 ; y<=n-1 ; y++){
                if (abs(x+y+z)%2 == 0){sign = -1;}else{sign = 1;} // keeping track of the charge of the ions at a lattice point
                SurfacePotential += (sign / sqrt(x*x + y*y + z*z));
                }
            }
        z = -n;
        for (int x=-n+1 ; x<=n-1 ; x++){
            for (int y=-n+1 ; y<=n-1 ; y++){
                if (abs(x+y+z)%2 == 0){sign = -1;}else{sign = 1;} // keeping track of the charge of the ions at a lattice point
                SurfacePotential += (sign / sqrt(x*x + y*y + z*z));
                }
            }

    return SurfacePotential;
}

int main(int argc, char **argv){ 
    int sidelenght = atof(argv[1]) ;
    fprintf(stderr, "Fuer den Wuerfel mit der Seitenlaenge %d finden wir folgende Punkte. \n", sidelenght);
    //double* VolumePotentioal = malloc(sizeof(double));
    double VolumePotentioal = 0;
    for (int n=1 ; n<=sidelenght ; n++){
        VolumePotentioal = VolumePotentioal + cube(n);//sums 
    }
    fprintf(stderr,"the total potential sum = %f\n", VolumePotentioal);

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