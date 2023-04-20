/***********************************************************************
 * Polynominterpolation mit dem Algrithmus von Neville und Aitken
 *
 * kompilieren  mit
 *
 *   gcc -Wall -pedantic -o neville-aitken neville-aitken.c  -lm
 *
 *   "-pedantic" und "-Wall" === Warnungen ausgeben
 *
 *   "-o neville-aitken" === Name der Ausgabedatei ist "neville-aitken"
 *
 *   "-lm" === binde die Bibliothek für mathematische Funktionen ein
 *
 *   Die genaue Beschreibung der Optionen lesen wir in der man-page für den Befehl gcc nach, also
 *   in der bash-Konsole mit  "man gcc"
 *
 *   z.B. aus man gcc:
 *
 *   -pedantic
 *      Issue all the warnings demanded by strict ISO C and ISO C++; reject all programs that use forbidden extensions,
 *      and some other programs that do not follow ISO C and
 *      ISO C++.  For ISO C, follows the version of the ISO C standard specified by any -std option used.
 *
 *   -Wall
 *      This enables all the warnings about constructions that some users consider questionable,
 *      and that are easy to avoid (or modify to prevent the warning), even in conjunction with macros.
 *
 *   -o filename
 *      Place output in file filename. 
 *
 * aufrufen mit
 *   ./neville-aitken z nxy
 *
 *   z   = Auswerungsstelle
 *   nxy = Anzahl der Stützpunkte
 *
 ***********************************************************************/


/***********************************************************************
 * HEADER
 *
 * wir binden header-Dateien ein, in denen elementare Funktionen 
 * definiert sind
 ***********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>  /* speziell für Funktion memset () */
#include <math.h>


/***********************************************************************
 * Wir definieren ein Funktion func, die ein gegebenes Polynom auswerten
 * soll
 * 
 * Eingabe: x in double precision; x wird nicht verändert, also
 * deklarieren wir es mit "const"
 *
 * Ausgabe: P(x) in double precision
 *
 * Diese Funktion wird ausschliesslich innerhalb dieser c-Datei benutzt
 * (den Gültigkeitsbereich bezeichnen wir als "scope" ); daher
 * kann sie als static deklariert werden
 *
 ***********************************************************************/
static double func ( double const x )
{
  /* wir berechnen ein Polynom 4. Grades, mit x als Argument
   * und weisen den Wert der Konstanten p zu */

#if 0
  double const p =   1.7 
                   + 4.3  * x 
                   - 9.2  * x*x 
                   + 0.3  * x*x*x 
                   - 2.75 * x*x*x*x 
                   - 6.32 * x*x*x*x*x
                   + 9.63 * x*x*x*x*x*x
                   - 1.98 * x*x*x*x*x*x*x;
                   /* - 0.25 * x*x*x*x*x*x*x*x; */
#endif

  double const p = exp(x);

  /* p wird von der Funktion zurückgegeben; 
   * p MUSS DENSELBEN TYP haben, wie in der Definition der Funktion, also
   * --> double <-- func ( ... ) und
   * --> double <-- p ...  */
  return ( p );

}  /* end of func */


/***********************************************************************
 * MAIN PROGRAM
 *
 * Das ist die Hauptfunktion, die in jedem C-Programm vorhanden sein muss
 ***********************************************************************/
int main(int argc, char **argv) 
{

  /*
   * Das Programm wird im Allgemeinen aufgerufen mit
   *
   * /Pfad/zur/Programmdatei/Programm Option1 Option2 ...
   *
   * argc = Anzahl der Wörter im obigen Programmaufruf, also z.B. argc = 3, wenn Option1 und Option2
   * gegeben werden
   *
   * argv ist ein Array von Zeichenketten, das diese Wörter enthält
   *
   * also z.B. 
   * argv[0] = "/Pfad/zur/Programmdatei/Programm"
   * argv[1] = "Option1"
   * argv[2] = "Option2" 
   *
   * Mit argv[1], argv[2], ... stellt man zusätzliche Eingabedaten bereit.
   */

  /* die Zeichenkette argv[1] soll die Auswertungsstelle z enthalten;
   * diese wird von char* nach double umgewandelt 
   * mit Funktion atof aus stdlib.h, siehe Manpages man atof, man stdlib.h
   *
   * Wir prüfen erst, ob ein Argument in der Kommandozeile gegeben wurde
   */
  if ( argc < 3 )
  {
    fprintf( stderr, "[neville-aitken] Fehler, keine Eingabe für z und nxy \n" );
    exit (1);
  }
  double z = atof ( argv[1] );

  /* Das ist die Anzahl der Stützstellen 
   */
  int nxy = atoi ( argv[2] );
  if ( nxy <= 0 ) {
    fprintf ( stderr, "[neville-aitken] Fehler, nxy muss positiv sein\n" );
    exit(4);
  }

  /* Zur Kontrolle geben wir die Eingabe nach Standardausgabe aus
   */
  fprintf ( stdout, "# [neville-aitken] Eingabe z = %25.16e   nxy = %2d\n", z, nxy );


  /* Arrays für die Stützpunkte (x,y); die Stützpunkte sollen in diesem Beispiel
   * nicht verändert werden, sind als mit Qualifier "const" deklariert.
   */
  double * x = (double *)malloc ( nxy * sizeof(double) );  /* Pointer auf Speicher, der die Stützstellen enthält */

  /* y ist ein Pointer auf Speicher, der die Stützordinaten enhält;
   */
  double * y = (double *)malloc ( nxy * sizeof(double) );

  /* Prüfe, dass x und y nicht NULL sind; falls wahr, dann Abbruch
   */
  if ( x == NULL || y == NULL )  /* benutze logisches Oder, also  || */
  {
    /* fprintf ( stderr, ... ) schreibt in die Standard-Fehlerausgabe;
     * __FILE__ , __LINE__ sind Präprozessor-Makros, die beim Kompilieren 
     * mit Programmname und Zeilennummmer ersetzt werden
     */
    fprintf ( stderr, "[neville-aitken] Fehler von malloc %s %d\n", __FILE__, __LINE__ );

    /* Rückkehr aus dem Programm mit Rückgabewert 2 */
    exit(2);
  }


  for ( int i = 0; i < nxy; i++ )
  {
    x[i] = (double)i + 1.;  /* cast = Umwandlung int nach double der Schleifenvariable i */

    y[i] = func( x[i] );  /* für unseren Test setzen wir y = func ( x ) */
  }

  double * T = (double *) malloc ( nxy * sizeof(double) );  /* Speicher für die Tik, maximal brauchen wir Grad + 1;
                                                               malloc gibt immer einen Pointer vom Typ void* zurück;
                                                               dieser wir explizit nach Typ double* umgewandelt 
                                                               (= cast from void* to double* ) 
                                                             */

  /* Prüfe, ob T der NULL-Pointer sind; falls wahr, 
   * dann wurde kein Speicher reserviert und wir müssen abbrechen
   */
  if ( T == NULL )
  {
    fprintf ( stderr, "[neville-aitken] Error from malloc %s %d\n", __FILE__, __LINE__ );
    exit(3);
  }

  /* Initialisiere T mit 0 in allen Bytes
   */
  memset ( T, 0, nxy * sizeof (double ) );


  /***********************************************************************
   * Konstruktion der T_ik nach Neville-Aitken
   *
   * x_0 | y_0 = T_00
   *     |              T_11
   * x_1 | y_1 = T_10          T_22
   *     |                          \
   *     |              T_21          T_33
   *     |                   \      /      
   * x_2 | y_2 = T_20          T_32
   *     |            \      /
   *     |              T_31
   *     |            /
   * x_3 | y_3 = T_30 
   * .   |
   * .   |
   * .   |
   ***********************************************************************/
  
  /* Schleife über den Grad des Interpolationspolynoms;
   *
   * mit der Anzahl nxy der Stützstellen kann der Grad höchstens nxy - 1 sein
   */  

  for ( int grad = 0; grad < nxy; grad++ ) 
  {

    /* bis hierhin ist 
     *
     * T = { T_{grad-1, grad-1}, T_{grad-1, grad-2}, ..., T_{grad-1,0},  0, ... , 0 }
     */

    /* jetzt fügen wir am Ende das neue y[grad] hinzu und bekommen
     *
     * T = { T_{grad-1, grad-1}, T_{grad-1, grad-2}, ..., T_{grad-1,0}, T_{grad,0}, 0, ..., 0 }
     */
    T[grad] = y[grad];

    /* T[grad] = T_{grad,0} ist bereits das neue Element der neuen Schrägzeile;
     * wir berechnen nun mit der Neville-Rekursion 
     * die verbleibenden neuen Elemente nacheinander
     * T_{grad,1}, T_{grad,2}, ..., T_{grad,grad}
     */
    for ( int k = grad -1; k >= 0; k-- )
    {
      T[k] = ( ( z - x[k] ) * T[k+1] - ( z - x[grad] ) * T[k] ) / ( x[grad] - x[k] );
    } 
    /* jetzt enthält T die Elemente der neuen Schrägzeile
     *
     * T = { T_{grad,grad}, T_{grad,,grad-1}, ..., T_{grad,1}, T_{grad,0}, 0, ..., 0 }
     */



    /* Auswertung des Interpolationspolynoms der Ordnung grad
     *
     * T[0] = T_{grad,grad} ist der Wert des Interpolationspolynoms 
     * mit Grad grad an der Stelle z; das geben wir aus
     *
     * Die Differenz zwischen dem P(z) und func (z) berechnen wir
     * mit Funktion fabs() aus math.h; dazu brauchen wir -lm beim
     * Linken; siehe Manpages man fabs, man math.h
     */
    fprintf( stdout, "Grad %2d   P %25.16e   diff %16.7e\n", grad, T[0], fabs(T[0] - func(z)) );

  }  /* end of loop on grad */

  /* Den Speicher für T wieder freigeben
   */
  free ( T );

  /*Programm verlassen mit Rückgabewert 0, d.h. kein Fehler */
  return ( 0 );

}  /* end of main */

