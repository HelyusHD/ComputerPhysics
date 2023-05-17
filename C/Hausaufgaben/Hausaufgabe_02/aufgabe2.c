/***********************************************************************
 *
 * compile with
 *
 *   gcc -Wall -pedantic aufgabe2.c -o aufgabe2 -lm
 *
 * run with
 *   ./aufgabe2
 *
 ***********************************************************************/


#include <stdio.h>
#include <stdlib.h>

int const range_z = 200;
char *filename_read = "potential.txt"; //where to get the data points from
char *filename_write = "electric_field.txt"; //where to write to


typedef struct {
    double z_value;
    double potential_value;
} Tuple;



/***********************************************************************
 * 1. symmetrische Ableitung
 * Nutzung von absoluten Werten und nicht einer Funktionsforschrift
 * f_1 , f_2 sind die Funktionswerte f(z_1), f(z_2)
 * h ist deltha z
 ***********************************************************************/
double deriv_sym_1 (double const h, double const f_1, double const f_2){
  return( ( f_2 - f_1 ) / (2.0*h) );
}



int main() {
    FILE *file;
    Tuple *data_points;
    int numLines = range_z * 2 + 1;

    // Datei zum Lesen öffnen
    file = fopen(filename_read, "r");
    if (file == NULL) {
        printf("Fehler beim Öffnen der Datei.\n");
        return 1;
    }

    // Liste der Tupel erstellen
    data_points = (Tuple*)malloc(numLines * sizeof(Tuple));
    if (data_points == NULL) {
        printf("Fehler bei der Speicherzuweisung.\n");
        return 1;
    }

    // Datei einlesen und Tupel in die Liste speichern
    for (int i = 0; i < numLines; i++) {
        if (fscanf(file, "%lf %lf", &(data_points[i].potential_value), &(data_points[i].z_value)) != 2) { // doesnt work for line 1
            fprintf(stderr,"Fehler beim Lesen der Datei in Zeile %d.\n",i);
            return 1;
        }
    }

    // Datei schließen
    fclose(file);

    //delta z ausrechnen
    double const h = data_points[2].z_value - data_points[1].z_value;


    FILE * fp = fopen ( filename_write, "a" );

    for (int i = 1; i < numLines - 1 ; i++) { //ignore first line, so we start at i = 1
        double electric_field = deriv_sym_1(h ,data_points[i].potential_value ,data_points[i+1].potential_value);
        fprintf(fp ,"%f   %f\n" ,data_points[i].z_value ,electric_field);
    }
    fclose(fp); 

    // Speicher freigeben
    free(data_points);

    return 0;
}
