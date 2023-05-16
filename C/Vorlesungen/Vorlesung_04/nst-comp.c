/***********************************************************************
 *
 * Kompilieren mit
 *
 *   gcc -Wall -pedantic nst-comp.c -o nst-comp -lm
 *
 *     für single/double precision
 *
 * Ausführen mit
 *   ./nst-comp
 *
 ***********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct {
 double u;
 double v;
} double2;

/* Typdefinition; Pointer auf Funtion y = f( x , z ),
 */
typedef double2 (*func_type ) ( double const, double const );


/***********************************************************************
 * Funktion, für die das Extremum bestimmt werden soll,
 * vom Typ func_type
 ***********************************************************************/
double2 func ( double const x, double const y )
{
  double2 f = { x*x-y, 2.*x };

  return ( f );

}  /* end of func */


/***********************************************************************/
/***********************************************************************/

/***********************************************************************
 * Sekantenverfahren
 * Startintervall [x0, x1]; Genauigkeit eps, benutze Funktion f
 ***********************************************************************/
double sekant ( double x0, double x1, func_type f, double const eps, double const z )
{

  double2 a = f( x0, z );
  double2 b = f( x1, z );

  if ( fabs( a.u - b.u ) < eps )
  {
    fprintf(stderr, "[sekant] Problem, Differenz der Startordinaten zu klein \n");
    return ( NAN );
  }

  int iter = 0;

  while ( fabs ( ( x0 - x1 ) / (x0 + x1 ) * 2. ) > eps )
  {
    double const x = ( x0 * b.u - x1 * a.u ) / ( b.u - a.u );

    iter++;
  
    fprintf ( stdout, "# [sekant] diff %4d %25.16e %25.16e\n", iter, fabs( x1 - x ), fabs( x1 - x0 ) );

    a = b;

    b = f( x, z );

    x0 = x1;

    x1 = x;

  }

  fprintf ( stdout, "# [sekant] Anzahl iterationen = %4d\n", iter );

  return ( x1 );

} /* end of sekant */

/***********************************************************************/
/***********************************************************************/

/***********************************************************************
 * Newton-Raphson-Verfahren
 * Startpunkt x0; Genauigkeit eps, benutze Funktion f
 ***********************************************************************/
double newton_raphson ( double x0, func_type f, double const eps, double const z )
{

  double2 y = f( x0, z );

  if ( fabs( y.v ) < eps )
  {
    fprintf(stderr, "[newton_raphson] Problem, Ableitung  zu klein \n");
    return ( NAN );
  }

  int iter = 0;
  double reldif = 2. * eps;

  while ( reldif > eps )
  {
    double const x = x0 - y.u / y.v;

    iter++;
  
    fprintf ( stdout, "# [newton_raphson] diff %4d %25.16e\n", iter, fabs( x0 - x ) );

    y = f( x, z );

    reldif = fabs ( (x0 - x ) / x );

    x0 = x;

  }

  fprintf ( stdout, "# [newton_raphson] Anzahl iterationen = %4d\n", iter );

  return ( x0 );

} /* end of newton_raphson */

/***********************************************************************
 * MAIN PROGRAM
 ***********************************************************************/

int main(int argc, char **argv)
{

  double z = 0.; /* Defaultwert */

  /* Eingabewert z aus der Kommandozeile */
  z = atof ( argv[1] );

  /* Startintervall für das Sekantenverfahren */
  double const x0 = 1.;
  double const x1 = 6.;
  double const eps = 1.e-12;  /* Zielgenauigkeit für Sekantenverfahren */

  /* Schätzwert für die Nullstelle mit Sekanten-Verfahren */
  double const xnst = sekant ( x0, x1, func, eps, z );

  /* sekant kann NAN = not a number zurückgeben;
   * NAN hat die Eigenschaft, dass x != x wahr ist
   * (im Gegensatz zu jedem Gleitkommatyp
   */
  if ( xnst != xnst )
  {
    fprintf(stderr, "[nst-comp] Fehler von sekant \n" );
    return ( 1 );
  }

  /* Schätzwert für die Nullstelle mit Newton-Raphson */
  double const ynst = newton_raphson ( x0, func, eps, z );
  
  if ( ynst != ynst )
  {
    fprintf(stderr, "[nst-comp] Fehler von sekant \n" );
    return ( 2 );
  }


  /* Ausgabe von xnst */
  fprintf ( stdout, "# [nst-comp] sqrt ( %25.1e )   %25.16e  %25.16e   eps = %e \n", z, xnst, ynst, eps );

  return ( 0 );
}
