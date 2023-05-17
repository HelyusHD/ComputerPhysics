#include <stdio.h>
#include <stdlib.h>

int const range_z = 200;
char const file_name ='potential.txt';


typedef struct {
    double z_value;
    double potential_value;
} Tuple;

int main() {
    FILE *file;
    Tuple *data_points;
    int numLines = range_z * 2 + 1;

    // Datei zum Lesen öffnen
    file = fopen(file_name, "r");
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
        if (fscanf(file, "%lf %lf", &(data_points[i].z_value), &(data_points[i].potential_value)) != 2) { // doesnt work for line 1
            fprintf(stderr,"Fehler beim Lesen der Datei in Zeile %d.\n",i);
            return 1;
        }
    }

    // Datei schließen
    fclose(file);

    // Tupel in der Liste verwenden
    for (int i = 0; i < numLines; i++) {
        printf("Tupel %d: %.2lf, %.2lf\n", i + 1, data_points[i].z_value, data_points[i].potential_value);
    }

    // Speicher freigeben
    free(data_points);

    return 0;
}
