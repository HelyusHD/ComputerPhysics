// We imagine a 2d letice
// We want to calculate the number of points inside a radius "r" if points are spaced evenly with distance = 1

#include <stdio.h> // #include imports lib
#include <stdlib.h>
#include <string.h>  /* speziell für Funktion memset () */
#include <math.h>


    void c2(unsigned int r){
        //double * y = (double *)malloc ( r * r * sizeof(double) );
        for ( unsigned int i =0 ; i<=r ; i++){
            for ( unsigned int j=0 ; j<=r ; j++){
                if (i*i + j*j == r){
                    fprint("pair found: i = %i j = %i " , i , j);
                }
            }
        }
    }




int main(int argc, char **argv){

    c2(argv[1]);
    return 0;
}