#include <stdio.h> // #include imports lib
#include <stdlib.h>
#include <string.h>  /* speziell für Funktion memset () */
#include <math.h>


    void c3(int r){
        //double * y = (double *)malloc ( r * r * sizeof(double) );
        for ( int i =-r ; i<=r ; i++){
            for ( int j=-r ; j<=r ; j++){
                for( int k=-r ; k<=r ; k++){
                    if (i*i + j*j + k*k == r){
                       fprintf(stderr , "pair found: i = %i j = %i k = %i \n"  , i , j, k);
                    }
                }                    
            }
        }
    }

int main(int argc, char **argv){

    unsigned int a ;
    a = atof (argv[1]);  // atof is a function which can change from a typ string into an intager typ 
    double b = sqrt(a);
    int c = (int)b;
    fprintf(stderr, "Here we have all points inside a circle with radius %d \n", c);
    for (int i=0 ; i<=a ; i++){
        fprintf(stderr, "----- C3(%d) -----\n",i);
        c3(i);
    }
    return 0;
}