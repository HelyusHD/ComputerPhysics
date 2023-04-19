/***********************************************************************
 *
 * kompilieren  mit
 *
 *   gcc -Wall -pedantic A1.c -o A1 -lm
 *
 *   Die Optionen fuer gcc koennen mit "man gcc" nachgelesen werden.
 *
 * run with
 *   ./A1
 *
 ***********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/***********************************************************************
 * Einlesen von dataset
 *
 * Array dout speichert die Daten, die aus Datei "filename" gelesen werden;
 * 
 * dout und nout werden als Zeiger uebergeben ( Adresse von Zeiger dout und nout ),
 * um die Manipulationen des Inhalts auch ausserhalb der Funktion read_dataset
 * zu erhalten ( ansonsten Aenderungen der Funktionsargumente nur innerhalb der Funktion wirksam )
 *
 *
 * Eingabe / Ausgabe:
 *   dout = Feld, in dem die eingelesenen Daten gespeichert werden; dout zeigt 
 *          bei Eintritt in die Funktion noch auf keinen Speicherbereich, sondern 
 *          wird auf d gesetzt
 *
 *   nout = Adresse einer Integervariablen, die die Anzahl der gelesenen Elemente speichert.
 *
 * Eingabe:
 *   filename = Dateiname
 *   binary = Schalter, ob die Datei Binärdaten enthält, oder eine Spalte mit Zahlen im Textformat
 *
 ***********************************************************************/
int read_dataset ( double ** const dout, unsigned int * const nout , char * const filename, int const binary ) 
{

  FILE * fp = fopen ( filename, "r" );
  if ( fp == NULL ) return ( 1 );

  unsigned int n = 0;  /* Zähler für die Anzahl der Elemente, wird am ende *nout zugewiesen */
  double * d = NULL;   /* Feld für die zu lesenden Zahlen; wird am Ende *dout zugewiesen    */

  /* Einlesen der Meta-Daten ( Daten zur Beschreibung des Datensatzes )
   * das ist hier nur die Anzahl der Elemete, die nach Vereinbarung an erster Stelle stehen soll
   */
  if ( binary ) 
  {
    /* Im Fall einer Binärdatei benutzen wir fread, siehe man fread
     *
     * Prüfe auch gleich, ob die Anzahl der gelesenen Argument gleich 1 ist,
     * sonst Abbruch
     */
    if ( fread ( &n, sizeof ( unsigned int ), 1, fp ) !=  1 ) return( 2 );

  } else {
    /* Im Fall einer Textdatei benutzen wir fscanf, siehe man fscanf 
     *
     * Prüfe wiederum, dass wirklich ein Element gelesen wurde, sonst Abbruch
     */
    if ( fscanf ( fp, "%u\n", &n ) != 1 ) return ( 2 );

  }
  /* Falls Null gesesen wurde, Abbruch, dann gibt es nichts einzulesen
   */
  if ( n == 0 ) 
  {
    fprintf ( stderr, "[read_dataset] no data\n" );
    return ( 4 );

  } else {
    /* Kontrollausgabe des gelesenen n-Wertes
     */
    fprintf ( stdout, "# [read_dataset] length of dataset = %d\n", n );

  }

  /* Wir reservieren ein Feld im Speicher für n doubles
   */
  d = (double*) malloc ( n * sizeof(double) );
  /* Wir prüfen, dass der von malloc zurückgegebene Zeiger nicht NULL ist;
   * NULL heisst Fehler, also Abbruch mit Fehler-Code
   */
  if ( d == NULL ) {
    fprintf ( stderr, "[read_dataset] Error from malloc %s %d\n", __FILE__, __LINE__ );
    return ( 5 );
  }
  /* Daten einlesen;
   *
   * wir prüfen jeweils, dass beim Lesebefehl die richtige Anzahl
   * der Elemente gelesen wurde 
   */ 
  if ( binary ) {
    /* Lesen im Binaerformat mit fread */
    if ( fread ( d, sizeof ( double ), n , fp ) !=  n ) return( 3 );

  } else {

    /* Lesen im ASCII-Format mit fscanf, zeilenweise mit fscanf
     *
     * hier benutzen wir Zeiger-Arithmetik, und übergeben
     * and fscanf die Adressen, an die gespeichert werden soll
     * mit d = d+0, d+1, d+2, ...
     */
    for ( unsigned int i = 0; i < n; i++ ) 
    {
      if ( fscanf ( fp, "%lf\n", d+i ) != 1 )  return( 3 );
    }

  }  /* end of if binary */

  /* Schliesse Dateizeiger */
  fclose ( fp );

  /* Zuweisung der Rückgabewerte; 
   *
   * der SPEICHERINHALT, auf den die Zeiger dout und nout zeigen, wird manipuliert (NICHT
   * die Pointer selbst) 
   */
  *nout = n;
  *dout = d;

  /* Rückker aus der Funktion mit Rückgabewert 0 = kein Fehler */
  return ( 0 );

}  /* end of read_dataset */

/***********************************************************************
 * Kontrollfunktion: zeige den Inhalt des Feldes data in stdout
 *
 * Eingabe: 
 *   data: Pointer of Speicherbereich vom Typ double; wird nicht
 *          verändert, also const
 *   n: Anzal der double-Elemente 
 *
 * Ausgabe:
 *   direkt nach Standard-Ausgabe
 ***********************************************************************/
int show_dataset ( double * const data, unsigned int const n ) 
{
  
  if ( data == NULL || n == 0 ) return ( 1 );

  fprintf( stdout, "\n\n# [show_dataset]\n" );
  for ( unsigned int i = 0; i < n; i++ ) 
  {
    fprintf( stdout, "%6d %25.16e\n", i, data[i] );
  }

  return ( 0 );
}  /* end of show_dataset */

/***********************************************************************
 * Die Funktion cum_sqr_sum führt die kumulative Quadratsumme aus.
 *
 * dout[k] = din[0]^2 + ... + din[k]^2
 *
 * interativ als
 * dout[k] = dout[k-1] + din[k] * din[k];
 *
 * Eingabe:
 *   din = Eingangswerte
 *   n = Anzahl der Elemente in din
 *
 * Ausgabe:
 *   dout = Werte der Funktion
 *
 * Kann auch "in-place" genutzt werden, also dout = din; dann wird dout 
 * mit den neu berechneten Werten überschrieben
 *
 ***********************************************************************/
int cum_sqr_sum ( double * const dout, double * const din, unsigned int n )
{
  if ( dout == NULL || din == NULL || n == 0 ) return ( 1 );

  dout[0] = din[0] * din[0];
  for ( unsigned int i = 1; i < n; i++ ) {
    dout[i] = dout[i-1] + din[i] * din[i];
  }

  return ( 0 );
} /* end of cum_sqr_sum */


/***********************************************************************
 * MAIN PROGRAM
 ***********************************************************************/

int main(int argc, char **argv) {

  unsigned int ndata = 0;
  int exitstatus;
  double * data = NULL;
  FILE * fout = NULL;

  /***********************************************************************
   * Wir behandeln dataset1: Einlesen, kumulative
   *   Quadratsumme ausrechenenen und Ergebnis in einer Datei abspeichern
   ***********************************************************************/

  /* dataset1 einlesen 
   *
   * Wir übergeben die ADRESSE von data mit "&"-Operator and die Funktion
   * read_dataset; innerhalb der Funktion wird dann Speicher reserviert
   * und and data zugewiesen.
   */
  exitstatus =read_dataset ( &data, &ndata, "dataset1", 0 );
  if ( exitstatus != 0 ) {
    fprintf( stderr, "[main] Error from read_dataset, status was %d %s %d\n", exitstatus, __FILE__, __LINE__);
    exit( 3 );
  }

  /* Zur Kontrolle: data in stdout anzeigen 
   */
  show_dataset( data, ndata );

  /* kumulative Quadratsumme in data2 
   */
  double * data2 = (double*)malloc ( ndata * sizeof(double) );

  /* Prüfe, dass in data2 tatsächlich Speicher reserviert wurde
   */
  if (data2 == NULL ) exit( 5 );

  /* Datei oeffnen zum Schreiben, d.h. den FILE-Zeiger FILE * fout setzen
   */
  fout = fopen ( "dataset1.cum_sqr_sum", "w" );
  if ( fout == NULL) exit ( 6 );

  /* Funktionsaufruf für kumulative Quadratsumme 
   */
  cum_sqr_sum ( data2, data, ndata );
  
  /* data2 nach fout schreiben 
   */
  for( unsigned int i = 0; i < ndata; i++ ) {
    fprintf ( fout, "%e\n", data2[i] );
  }
  /* Datei schliessen 
   */
  fclose ( fout );

  /* dynamisch allokierten heap Speicher wieder freigeben; (pruefe hier noch mal, dass Zeiger nicht NULL sind ) 
   *
   * data selbst war in read_dataset reserviert worden; 
   * data2 haben wir im Hauptprogramm selbst mit malloc angelegt.
   */
  if ( data  != NULL ) free ( data  );
  if ( data2 != NULL ) free ( data2 );

  /* Der Sauberkeit halber setzen wir data wieder auf NULL
   */
  data = NULL;


  /***********************************************************************
   * Wir behandeln dataset2: Einlesen ALS BINÄRDATEI, den Sinus
   * auf den gelesenen Daten ausrechnen und einer Datei abspeichern
   ***********************************************************************/

  /* dataset2 einlesen
   */
  exitstatus =read_dataset ( &data, &ndata, "dataset2", 1 );
  if ( exitstatus != 0 ) {
    fprintf( stderr, "[main] Error from read_dataset, status was %d %s %d\n", exitstatus, __FILE__, __LINE__);
    exit( 4 );
  }

  /* Zur Kontrolle die eingelesenen Daten ausgeben nach Standard-Ausgabe
   */
  show_dataset( data, ndata );

  /* Die Sinus-Funktion anwenden und in eine Datei schreiben
   */
  fout = fopen ( "dataset2.sin", "w" );
  if ( fout == NULL) exit ( 7 );

  /* Schleife über Feldelemente, wende Funktion sin() aus der Mathe-Bibliothek math an */
  for( unsigned int i = 0; i < ndata; i++ ) {
    fprintf ( fout, "%25.16e %25.16e\n", data[i], sin( data[i] ) );
  }
  fclose ( fout );

  /* heap-Speicher freigeben
   */
  if ( data != NULL ) free ( data );

  return ( 0 );
}
