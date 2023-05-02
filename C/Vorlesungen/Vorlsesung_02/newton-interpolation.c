/***********************************************************************
 * Newtonsche Polynominterpolation
 *
 * kompilieren  mit
 *
 *   gcc -Wall -pedantic newton-interpolation.c  -o newton-interpolation -lm
 *
 * aufrufen mit
 *   ./newton-interpolation nxy
 *
 *  nxy ist die Anzahl der Stützstellen in diesem Beispiel-Code
 *
 ***********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#if 0
/***********************************************************************
 * Runge-Funktion
 ***********************************************************************/
static double runge ( double const x )
{
  return ( 1. / ( 1. + x*x ) );

}  /* end of runge */
#endif

/***********************************************************************
 * Eine zu interpolierende Funktion zum Testen
 ***********************************************************************/
static double func ( double const x )
{
#if 0 
  double const p =   1.7
                   + 4.3  * x
                   - 1.2  * x*x
                   + 0.3  * x*x*x
                   - 0.075 * x*x*x*x
                   - 0.0032 * x*x*x*x*x
                   + 0.00023 * x*x*x*x*x*x;
#endif

  double const p = exp(x);

  return ( p );

}  /* end of func */


/***********************************************************************
 * Horner-Schema zur Auswertung eines Interpolationspolynoms
 * in der Newton-Darstellung
 *
 * Auswertung des Polynoms "von innen nach aussen", beginnend mit der
 * höchsten Potenz
 *
 * p(z) = [ ... [ f_n * ( x - x_{n-1} )  + f_{n-1} ] * ( z - x_{n-2} ) + ... ] ( z - x_0 ) + f_0
 *
 ***********************************************************************/
static double horner ( double const z, double * const x , double * const f, unsigned int const n )
{
  double p = 0.;  /* Startwert is 0 */

  for ( unsigned int i = n; i > 0; i-- )
  {
    /* addiere zu p den i-ten Koeffizienten und multipliziere
     * die Summe mit ( z - x_i ) 
     */
    p = ( p + f[i] ) * ( z - x[i-1] );
  }
  p += f[0];
  
  return ( p );

}  /* end of horner */


/***********************************************************************
 * MAIN PROGRAM
 ***********************************************************************/
int main(int argc, char **argv) 
{


  if ( argc < 2 )
  {
    fprintf( stderr, "[newton-interpolation] Fehler, keine Eingabe nxy \n" );
    exit (1);
  }
  /* Das ist die Anzahl der Stützstellen
   */
  int nxy = atoi ( argv[1] );
  if ( nxy <= 0 ) {
    fprintf ( stderr, "[newton-interpolation] Fehler, nxy muss positiv sein\n" );
    exit(4);
  }

  /* Zur Kontrolle geben wir die Eingabe nach Standardausgabe aus
   */
  fprintf ( stdout, "# [newton-interpolation] Eingabe   nxy = %2d\n", nxy );


  int const max_deg = nxy - 1;
  
  /* Parameter für die Auswertung des Interpolationspolynoms 
   * zmin <= z <= zmax mit max_nz Punkten
   */
  int const max_nz = 200;

  double const zmin = 0.;
  double const zmax = 5.;

  double * pcoeff = (double *) malloc ( nxy * sizeof(double) );  /* Koeffizienten des Polynoms */

  double * fdd = (double *) malloc ( nxy * sizeof(double) );
  double * x   = (double *) malloc ( nxy * sizeof(double) );

  if ( x == NULL || fdd == NULL || pcoeff == NULL )
  {
    fprintf ( stderr, "[newton-interpolation] Error from malloc %s %d\n", __FILE__, __LINE__ );
    exit(2);
  }

 
  for ( int i = 0; i < nxy; i++ )
  {
    x[i] = (double)i;
  }


  /***********************************************************************
   * Konstruktion der f_... nach Newton mit dividierten
   * Differenzen
   *
   * x_0 | y_0 = f_0
   *     |              f_01
   * x_1 | y_1 = f_1           f_012
   *     |                           \
   *     |              f_12           f_0123
   *     |                   \       /      
   * x_2 | y_2 = f_2           f_123
   *     |            \      /
   *     |              f_23
   *     |            /
   * x_3 | y_3 = f_3 
   * .   |
   * .   |
   * .   |
   *
   * Die jeweils unterste, von links nach rechts steigende Schrägzeile halten wir in fdd;
   * in pcoeff speichern wir schrittweise die Koeffizienten des Polynoms
   * in der obersten, von links nachts fallenden Schrägzeile ab
   ***********************************************************************/

  /* initialisiere fdd und pcoeff zu Null in allen Bytes
   */
  memset ( fdd,    0, nxy * sizeof ( double ) );
  memset ( pcoeff, 0, nxy * sizeof ( double ) );

  for ( unsigned int grad = 0; grad <= max_deg; grad++ )
  {

    fdd[grad] = func ( x[grad] );

    for ( int k = grad -1; k >= 0; k-- )
    {
      fdd[k] = ( fdd[k+1] - fdd[k] ) / ( x[grad] - x[k] );
    }

    pcoeff[grad] = fdd[0];


    /* Auswertung des Interpolationspolynoms mit Grad grad 
     *
     * Die Wertetabelle schreiben wir in eine separate Datei
     * für jeden Polynomgrad
     */

    /* Erstelle einen Dateinamen
     */
    char filename[100];
    sprintf( filename, "newton.n%d.tab", grad );

    /* Öffne die Datei filename zum Schreiben
     */
    FILE * fp = fopen ( filename, "w" );

    /* Schreibe die Koeffizienten des Polynoms mit Grad grad 
     * in die Datei 
     */
    fprintf ( fp, "# n = %d coefficients\n", grad+1 );
    for ( int k = 0; k <= grad; k++ )
    {
      fprintf ( fp, "# k %4d  %25.16e\n", k, pcoeff[k] );
    }
    /* Schreibe eine Wertetabelle des Interpolationspolynoḿs und
     * der Funktion func in eine Datei
     */
    for ( int l = 0; l <= max_nz; l++ )
    {
      double const z = zmin + ( zmax - zmin )/(double)max_nz * (double)l;
      double const y = horner ( z, x, pcoeff, grad );
      fprintf( fp, "%6d %25.16e  %25.16e %25.16e\n" , l, z, y, func ( z ) );
    }

    fclose ( fp );
  }  /* end of loop on grad */

  free ( x      ) ;
  free ( fdd    );
  free ( pcoeff );

  return ( 0 );
}

