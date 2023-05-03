// i want to try out some ways to make recursive sums in C

/**********************************************************
 * things i want to try out
 * -> reading datasets as input
 * -> making more complex sum functions
 * -> implementing in other programms
**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/**********************************************************
 * Summation über n Schritte
**********************************************************/
double sum(int data[], int length);// this is a superior way to define function, it is more easy to read

/**********************************************************
* MAIN PROGRAMM
**********************************************************/
int main(){
    int testArray[] = {1,2,3,4,5,6,7,8,9,0}; // Array definition
    int typeCastingTest = (int)sum(testArray,10); // Type Casting 
    fprintf(stdout, "The sum equals to: %d\n", typeCastingTest);
    return 0;
}

double sum(int data[], int length){
    double sum_value = 0; // Wir definieren uns eine Temporäre Größe wo wir unsere zwischen Ergebnisse immer abspeichern, diese Wert muss auch mit 0 Initialisiert werden.
    for(int i = 0 ; i < length ; i++){ // Schleife welche unsere Summe bildet
        sum_value = sum_value + data[i]; // diese Schleife summiert die Werte in einem Array auf
    }
    return sum_value; // Rückgabe unserer Ergebnisses 
}