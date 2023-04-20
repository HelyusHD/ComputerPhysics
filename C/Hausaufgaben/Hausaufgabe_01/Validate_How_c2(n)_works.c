//scp 'C:\Users\andre\Documents\Git\ComputerPhysics\C\Hausaufgaben\Hausaufgabe_01\Validate_How_c2(n)_works.c' s6anthul@cip-pool.physik.uni-bonn.de:~/Documents/

// We imagine a 2d letice
// We want to calculate the number of points inside a radius "r" if points are spaced evenly with distance = 1

#include <stdio.h> // #include imports lib
#include <stdlib.h>
#include <string.h>  /* speziell für Funktion memset () */
#include <math.h>


    void c2(int r){
        //double * y = (double *)malloc ( r * r * sizeof(double) );
        for ( int i =-r ; i<=r ; i++){
            for ( int j=-r ; j<=r ; j++){
                if (i*i + j*j == r){
                    fprintf(stderr , "pair found: i = %i j = %i \n"  , i , j);
                }
            }
        }
    }




int main(int argc, char **argv){

    unsigned int a ;
    //a = (unsigned int) argv[1];
    a = (unsigned int) 16;
    fprintf(stderr, "Here we have all points inside a circle with radius %f \n", sqrt(a));
    for (int i=0 ; i<=a ; i++){
        fprintf(stderr, "----- C2(%d) -----\n",i);
        c2(i);
    }
    return 0;
}