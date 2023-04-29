#include <stdio.h> // #include imports lib
#include <stdlib.h>
#include <string.h>  /* speziell für Funktion memset () */
#include <math.h>

double cube(int n);
//double gewichtung()

int main(int argc, char **argv){

    unsigned int a ;
    a = atof (argv[1]);  // atof is a function which can change from a typ string into an intager typ 
    fprintf(stderr, "Here we have all points inside a circle with radius %d \n", a);
    for (int i=0 ; i<=a ; i++){
        fprintf(stderr, "----- cube(%d) -----\n",i);
        cube(i);
    }
    return 0;
}

double cube(int n){
n = 1;
unsigned int anzahl[4]={0,0,0,0}; // anzahl[1]*1, anzahl[2]*1/2, anzahl[3]*1/4, anzahl[4]*1/8
for( int j = -n ; j <= n ; j++ ){   
    for( int k = -n ; k <= n ; k++ ){   
        for( int l = -n ; l <= n ; l++ ){
            if(j != k && j != l && k != l) anzahl[1] += 1; //mittelpunkte
            if((j == k && j != l) || (j == l && j != k) || (k == l && k != j)) anzahl[2] += 1; // seiten
            if((j == k && j == l)) anzahl[3] += 1; //kanten
            if(j == k && j == l){//ecken
                anzahl[4] += 1;
                fprintf(stderr, "x and %d \n", anzahl[4]);
            } 
            }
        }
    }
    fprintf(stderr,"(%d;%d;%d;%d)", anzahl[1], anzahl[2], anzahl[3], anzahl[4]);
return 0;
}
// ich möchte mir nochmal die bedingungen besser anschauen, und überlegen warum meine anzahlen sich so komisch verhalten.




