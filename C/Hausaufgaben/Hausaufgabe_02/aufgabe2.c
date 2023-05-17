#include <stdio.h>
#include <stdlib.h>

int const range_z = 200;

typedef struct {
    double z_value;
    double potential_value;
} Tuple;

int main() {
    FILE *file;
    Tuple *tupleList;
    int numLines = range_z * 2 + 1;

    // Datei zum Lesen öffnen
    file = fopen("potential.txt", "r");
    if (file == NULL) {
        printf("Fehler beim Öffnen der Datei.\n");
        return 1;
    }

    // Liste der Tupel erstellen
    tupleList = (Tuple*)malloc(numLines * sizeof(Tuple));
    if (tupleList == NULL) {
        printf("Fehler bei der Speicherzuweisung.\n");
        return 1;
    }

    // Datei einlesen und Tupel in die Liste speichern
    for (int i = 0; i < numLines; i++) {
        if (fscanf(file, "%lf %lf", &(tupleList[i].z_value), &(tupleList[i].potential_value)) != 2) { // doesnt work for line 1
            printf("Fehler beim Lesen der Datei.\n");
            return 1;
        }
    }

    // Datei schließen
    fclose(file);

    // Tupel in der Liste verwenden
    for (int i = 0; i < numLines; i++) {
        printf("Tupel %d: %.2lf, %.2lf\n", i + 1, tupleList[i].z_value, tupleList[i].potential_value);
    }

    // Speicher freigeben
    free(tupleList);

    return 0;
}
