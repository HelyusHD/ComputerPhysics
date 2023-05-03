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



double cube(unsigned int n){ // Funktion cube nimmt eine bestimmte Seitenlaenge n und gibt die Anzahl an Mittel-, Seiten-, Kanten- und Eckpunkten zurück
    //double* SurfacePotential = malloc(sizeof(double));
    double SurfacePotential = 0;
    int* Vector3 = malloc(3 * sizeof(int));
    memset(Vector3, 0, 3 * sizeof(int));
    int* p = malloc(9 * sizeof(int));
    p[0]=0;p[1]=1;p[2]=2;p[3]=0;p[4]=1; // pairings for x y z which are used to compact the process of summing up points on the cubes surface
    int sign = 1;
    for (unsigned int i = 0 ; i<=3 ; i++){
        Vector3[p[i]] = n;
        if (n%2 == 1){sign = -1;} // keeping track of the charge of the ions at a lattice point
            for ( Vector3[p[i+1]] = -n ; Vector3[p[i+1]] <= n ; Vector3[p[i+1]]++){
                if (Vector3[p[i+1]]%2 == 1){sign = -sign;} 
                for ( Vector3[p[i+2]] = 1-n ; Vector3[p[i+2]] <= n-1 ; Vector3[p[i+2]]++){
                    if (Vector3[p[i+2]]%2 == 1){sign = -sign;}
                    SurfacePotential = SurfacePotential + (sign / sqrt(Vector3[0] * Vector3[0] + Vector3[1] * Vector3[1] + Vector3[2] * Vector3[2]));
                    //fprintf(stderr,"00 SurfacePotential = %f\n",SurfacePotential);
                    //fprintf(stderr, "x,y,z = %d %d %d SurfacePotential = %f\n",Vector3[0],Vector3[1],Vector3[2], SurfacePotential);
                    //fprintf(stderr, "sign = %d   squrt = %f\n",sign, sqrt(Vector3[0] * Vector3[0] + Vector3[1] * Vector3[1] + Vector3[2] * Vector3[2]));
                }
            }
            if (n%2 == 1){sign = -1;}  else {sign = 1;}
            SurfacePotential = SurfacePotential + 4 * sign / sqrt(3 * n * n);
    }
    return SurfacePotential;
}

int main(int argc, char **argv){ 
    unsigned int sidelenght = atof(argv[1]) ;
    fprintf(stderr, "Fuer den Wuerfel mit der Seitenlaenge %d finden wir folgende Punkte. \n", sidelenght);
    //double* VolumePotentioal = malloc(sizeof(double));
    double VolumePotentioal = 0;
    for (unsigned int n=1 ; n<=sidelenght ; n++){
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