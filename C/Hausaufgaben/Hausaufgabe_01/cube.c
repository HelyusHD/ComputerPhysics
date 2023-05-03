#include <stdio.h> // #include imports lib
#include <stdlib.h>
#include <string.h>  /* speziell für Funktion memset () */
#include <math.h>

int* cube(int n);
//double gewichtung()

int main(int argc, char **argv){
    int* anzahl = malloc(4 * sizeof(int)); // Dynamische Speicherzuweisung für ein int-Array mit 4 Elementen
    anzahl[0] = 0;
    anzahl[1] = 0;
    anzahl[2] = 0;
    anzahl[3] = 0;// anzahl[0]]*1, anzahl[1]*1/2, anzahl[2]*1/4, anzahl[3]*1/8
    //hier null setzen, sonst setzt er mir jedesmal bei neuem aufruf von cube die Werte auf 0
    unsigned int a ;
    a = atof (argv[1]);  // atof is a function which can change from a typ string into an intager typ 
    fprintf(stderr, "Fuer den Wuerfel mit der Seitenlaenge %d finden wir folgende Punkte. \n", a);
    for (int i=0 ; i<=a ; i++){
        fprintf(stderr, "----- cube(%d) -----\n",i);
        cube(i);
    }
    return 0;
}

int cube(int n, int anzahl[4]){

for( int j = -n ; j <= n ; j++ ){   
    for( int k = -n ; k <= n ; k++ ){   
        for( int l = -n ; l <= n ; l++ ){
            if (j == 0 && k == 0 && l == 0) {
                anzahl[0] += 1; 
                //fprintf(stderr, "a and %d \n", anzahl[0]);
            } // mittelpunkte
            if (((j == 0 && k == 0 && (l == n || l == -n))
             || (j == 0 && l == 0 && (k == n || k == -n))
             || (k == 0 && l == 0 && (j == n || j == -n)))){
             //&& abs(j + k + l) == n) {
                anzahl[1] += 1; 
                //fprintf(stderr, "b and %d \n", anzahl[1]);
            } // seiten
            if ((j == 0 && (k == n || k == -n) && (l == n || l == -n))
    	     || (k == 0 && (j == n || j == -n) && (l == n || l == -n))
             || (l == 0 && (j == n || j == -n) && (k == n || k == -n))){
                anzahl[2] += 1;
                //fprintf(stderr, "c and %d \n", anzahl[2]);
            } // kanten
            if(j == k && j == l && l == k){
                anzahl[3] += 1;
                //fprintf(stderr, "d and %d \n", anzahl[3]);
            } // ecken
            fprintf(stderr,"(%d;%d;%d;%d)\n", anzahl[0], anzahl[1], anzahl[2], anzahl[3]);
            }
        }
    }
    //fprintf(stderr,"(%d;%d;%d;%d)", anzahl[0], anzahl[1], anzahl[2], anzahl[3]);
return anzahl;
}
// ich möchte mir nochmal die bedingungen besser anschauen, und überlegen warum meine anzahlen sich so komisch verhalten.




