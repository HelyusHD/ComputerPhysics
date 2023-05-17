/***********************************************************************
 *
 * Kompilieren mit
 *
 *   gcc -DPRECISION_SIZE=<4/8>  -Wall -pedantic ableitung.c -o ableitung -lm
 *
 *     für single/double precision
 *
 * Ausführen mit
 *   ./ableitung
 *
 ***********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Makro zur Umwandlung eines Makros in eine Zeichenkette,
 * "stringification" 
 */
#define str(_a) #_a
#define xstr(_a) str(_a)

/* Wenn PRECISION_SIZE nicht definiert ist, benutze Defaultwert 8 
 * */
#ifndef PRECISION_SIZE
#define PRECISION_SIZE 8
#endif

/* Floating point-Datentyp und Ausgabeformat in Abhängigkeit
 * von PRECISION_SIZE
 */
#if   PRECISION_SIZE ==  4
#  define PRECISION_TYPE float
#  define FORMAT_STR "%25.16e"
#elif PRECISION_SIZE ==  8
#  define PRECISION_TYPE double
#  define FORMAT_STR "%25.16e"
#else
#  error "Unbekannt floating point Byte-Größe"
#endif

/* Typdefinition; Pointer auf Funtion y = f( x ),
 * wobei x und y mit Makro PRECISION_TYPE deklariert werden
 */
typedef PRECISION_TYPE (*func_type ) ( PRECISION_TYPE const x);

/***********************************************************************
 * 1. symmetrische Ableitung
 ***********************************************************************/
PRECISION_TYPE deriv_sym_1 ( func_type f, PRECISION_TYPE const x, PRECISION_TYPE const h )
{
  return( ( f(x+h) - f(x-h) ) / (2.*h) );
} /* end of deriv_sym_1 */

/***********************************************************************
 * 2. symmetrische Ableitung
 ***********************************************************************/
PRECISION_TYPE deriv_sym_2 ( func_type f, PRECISION_TYPE const x, PRECISION_TYPE const h )
{
  return( ( f(x+h) + f(x-h) - 2 * f(x) ) / (h*h) );
} /* end of deriv_sym_2 */

/***********************************************************************
 * 3. symmetrische Ableitung
 ***********************************************************************/
PRECISION_TYPE deriv_sym_3 ( func_type f, PRECISION_TYPE const x, PRECISION_TYPE const h )
{
  return( ( ( f(x+2*h) - f(x-2*h) ) - 2 * ( f(x+h) - f(x-h) ) )  / (2*h*h*h) );
} /* end of deriv_sym_3 */

/***********************************************************************
 * Funktion, für die Ableitungen berechnet werden sollen
 ***********************************************************************/
PRECISION_TYPE func ( PRECISION_TYPE const x)
{
  /* Funktionen aus math.h in Abhängigkeit des Typs */
#if   PRECISION_SIZE ==  4
  return ( logf(x) );
#elif PRECISION_SIZE ==  8
  return ( log (x) );
#endif
}  /* end of func */


/***********************************************************************
 * MAIN PROGRAM
 ***********************************************************************/

int main(int argc, char **argv)
{

  /* 1./2./3. Ableitung der Funktion func bei x0 = 3 */
  PRECISION_TYPE x0 = 3.;
  PRECISION_TYPE h0 = 1.;
  unsigned int n_step = 1000;
  PRECISION_TYPE dec_step = 0.95;

  /* Ausgabedatei für die Iteration über Schrittweite h */
  char filename[400];
  sprintf(filename, "deriv_sym.%s", xstr(PRECISION_TYPE) ) ;  /* cool, stringification des Datentyps */
  FILE * ofs = fopen( filename, "w" );

  /* Iteration über die Schrittweite h */
  PRECISION_TYPE h = h0;

  for ( unsigned int i = 0; i <= n_step; i++ ) 
  {
    /* Auswertung der Funktion func und der 1., 2. und 3. Ableitung */
    PRECISION_TYPE const fderiv[4] = {
      func(x0),
      deriv_sym_1 ( func, x0, h ),
      deriv_sym_2 ( func, x0, h ),
      deriv_sym_3 ( func, x0, h ) 
    };

    /* Ausgabe in Datei */
    fprintf ( ofs, FORMAT_STR"    "FORMAT_STR"    "FORMAT_STR" "FORMAT_STR" "FORMAT_STR" "FORMAT_STR"\n", x0, h, 
       fderiv[0], fderiv[1], fderiv[2], fderiv[3] );

    /* Verringerung der Schrittweite um vorgegebenen Faktor */
    h *= dec_step;
  }

  /* Datei schließen */
  fclose ( ofs );

  return ( 0 );
}
