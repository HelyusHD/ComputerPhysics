/***********************************************************************
 *
 * compile with
 *
 *   gcc -Wall -pedantic romberg.c -o romberg -lm
 *
 * run with
 *   ./romberg
 *
 ***********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>  /* needed for memcpy */
#include <math.h>

/* Vergleiche Kommentar in trapez.c, gleiches Spiel hier
 * 
 */
typedef double (*func_type ) ( double const x, void * p );

/***********************************************************************
 * elementarer Trapze-Schritt
 *
 * Eingabe:
 *   f = Zeiger auf Funktion, die Integriert werden soll
 *   x1, x2 = Stützstellen = Beginn und Ende des Integrationsintervalls
 *   p = Zeiger auf Speicherbereich mit Parametern für die
 *       Auswertung von f
 *
 * Ausgabe:
 *   Rückgabe des Trapez-Integrals
 *
 ***********************************************************************/
double trapez ( func_type f, double const x1, double const x2, void * p ) 
{

  return( ( f(x1, p) + f(x2, p) ) * 0.5 * ( x1 - x2 ) );

} /* end of trapez */

/***********************************************************************
 * Trapez-Integration
 *
 * fuer Funktion f zwischen xa und xe mit Schrittweite h;
 *
 * Eingabe:
 *   f  = Integrand
 *   xa = untere Intervallgrenze
 *   xb = obere Intervallgrenze
 *   p  = Parameterliste für die Auswertung des Integranden
 *
 * Ausgabe:
 *   Rückgabewert = Trapezintegral
 ***********************************************************************/
double trapez_integration ( func_type f, double const xa, double const xe, double const h, void * p )
{

  /* Anzahl der Zwischenschritte */
  int const nstep = ( xe - xa ) / h;

  /* Beitrag der Randwerte */
  double tsum = ( f ( xa, p ) + f( xe, p ) ) * 0.5;

  /* Beitraege der Zwischenwerte */
  for ( int i = 1; i < nstep ; i++ ) {
    tsum += f( xa + i * h, p );
  }
  /* mal Schrittweite */
  tsum *= h;

  /* Rückgabe von tsum */
  return( tsum );

} /* end of trapez_integration */

/***********************************************************************
 * Trapez-Integration für die iterative Verfeinerung,
 * 
 * d.h. es werden Zwischenstellen hinzugefügt
 *
 * Eingabe:
 *   f = Integrand
 *   xa, xe = Intervallgrenzen für die Integration
 *   h = Schrittweite
 *   p = Zeiger auf Parameterspeicherbereich
 *
 * Ausgabe:
 *   Rückgabe der Summe von den Zwischenpunkten
 *
 ***********************************************************************/
double trapez_integration_restarted ( func_type f, double const xa, double const xe, double const h, void * p )
{

  /* Anzahl der Zwischenschritte
   */
  int const nstep = ( xe - xa ) / h;

  /* fprintf( stdout, "# [trapez_integration_restarted] nstep = %4d\n", nstep ); */

  /* Initialisierung
   */
  double tsum = 0.;
  double x    = xa;

  /* Beitraege der Zwischenwerte 
   */
  for ( int i = 0; i <= nstep ; i++ )
  {
    tsum += f( x, p );
    x    += h;
  }
  /* mal Schrittweite */
  tsum *= h;

  return( tsum );

} /* end of trapez_integration_restarted */


/***********************************************************************
 * Neville-Schema fuer Polynom-Interpolation,
 *
 * Eingabe:
 *   y = Sützordinaten, hier die Trapezintegrale
 *   x = Stützwerte, hier die Liste der Schrittweiten
 *   m = Anzahl der Schrittweiten = Länge von y = Länge von x
 *
 * Ausgabe:
 *   Rückgabewert y[0], die Extrapolation bei 0
 *
 * in-place, d.h. ersetze Eintraege in y selbst
 *
 * Der Algorithmus ist analog dem in der Interpolation;
 * der Unterschied ist, dass wir hier immer konkret
 * für z = 0 auswerten
 ***********************************************************************/
double neville ( double * const y, double const * x , unsigned int const m )
{
  unsigned int k = 1;
  
  /* Spalten-weise über Spalten k */
  while ( k <= m ) 
  {
    /* Zeilen-weise */
    for ( unsigned int i = 0; i <= m-k; i++ ) 
    {
      double const xr = x[i] / x[k+i];
      y[i] = y[i+1] + ( y[i+1] - y[i] ) / ( xr * xr - 1 );
    }
    k++;
  }

  /* y[0] enhält am Ende T_mm(0) */
  return ( y[0] );

} /* end of neville */

/***********************************************************************
 * Bsp. Integrand fuer Gamma-Funktion
 *
 * exp ( -t ) t^( z - 1 ), z reell
 ***********************************************************************/
double Gamma_integrand ( double x, void * p  ) 
{
  double const z = ((double*)p)[0];

  return( exp  ( -x ) * pow  ( x, z - 1. ));
}  /* end of Gamma_integrand */

/***********************************************************************
 * Hauptprogramm
 ***********************************************************************/
int main(int argc, char **argv) 
{

  /* Integrand und Parameter fuer Integration von Gamma_integrand
   *
   * brauchen nur fuer Parameter 1 <= p <= 2 integrieren, sonst
   * p Gamma ( p ) = Gamma ( p + 1 ) ausnutzen
   *
   */ 
  func_type f = Gamma_integrand;

  double p[1] = { 1.62782384 };  /* Argument z fuer Gamma ( z ) */
  double xa = 0.;                /* untere Intervallgrenze */
  double xe = 100.;              /* obere Intervallgrenze  */

  double epsabs = 1.e-08;        /* geforderete max. absolute Aenderung */
  double epsrel = 1.e-06;        /* geforderete max. relative Aenderung */

  unsigned int nstep_min = 1;    /* minimale Anzahl Schritte */
  unsigned int nstep_max = 40;   /* maximal Anzahl Schritte */

  /* Startwerte */
  unsigned int nstep = nstep_min;
  double diffrel = 2*epsrel;
  double diffabs = 2*epsabs;

  double * y_old = (double*) malloc( ( nstep + 1 ) * sizeof( double ) );
  double * x_old = (double*) malloc( ( nstep + 1 ) * sizeof( double ) );

  /* Beginn mit der maximalen Schrittweite
   *   = Länge des Integrationsintervalls
   */
  double h = xe - xa;

  /* Berechne Trapezwerte fuer h_0,..,h_nstep
   *
   * mit Romberg-Folge h_{i+1} = h_{i} / 2
   */
    
  /* Beginn der Iteration;
   * Trapez-Integral mit zwei Stützpunkten
   */
  y_old[0] = trapez_integration ( f, xa, xe, h, (void*)p );
  x_old[0] = h;

  /* Schleife über die Anzahl der Zwischenschritte
   *   = Schrittweiten
   */
  for ( unsigned int i = 1; i <= nstep; i++ ) 
  {
 
    double const xan = xa + h / 2.;  /* neue untere Intervallgrenze */
    double const xen = xe - h / 2.;  /* neue obere Intervallgrenze  
                                          */
    y_old[i] = 0.5 * ( y_old[i-1] + trapez_integration_restarted ( f, xan, xen, h, p ) );

    h *= 0.5;

    x_old[i] = h;

    fprintf ( stdout, "# [main] i = %3d   h = %e   T = %25.16e\n", i, x_old[i], y_old[i] );
  }

  /* erster Romberg-Integrationswert mit nstep = nstep_min */
  double val_old = neville ( y_old, x_old , nstep );
  fprintf ( stdout, "# [main] Romberg nstep %2u integral value = %25.16e\n", nstep, val_old );

  /* Iteration über Schrittzahl, 2. Teil;
   *
   * jetzt geht es um die Kontrolle der Konvergenz
   *
   * Stopp-Kriterien gibt es drei: 
   *   1) nstep soll immer kleiner / gleich nstep_max sein
   *   2) die relative Änderung des Integralwertes soll < epsrel sein
   *   3) die absolute Änderung des Integralwertes soll < epsabs sein
   *
   * Ist eines dieser Kriterien erreicht, wird die Iteration
   * abgebrochen, und der aktuelle Wert als Schätzwert genommen.
   *
   */
  while ( nstep <= nstep_max && ( diffrel > epsrel || diffabs > epsabs ) ) 
  {

    nstep++;

    /* Wir reservieren neuen Speicher für x und y, der um 1 double 
     * grösser ist, als der alte, weil wir eine neue Schrittweite
     * hinzufügen;
     *
     * dazu benutzen wir realloc: realloc erweitert oder sucht einen neuen Block
     * und erhält den Speicherinhalt des alten Blocks
     */
    y_old = (double*) realloc( y_old, ( nstep + 1 ) * sizeof( double ) );
    x_old = (double*) realloc( x_old, ( nstep + 1 ) * sizeof( double ) );

    /* neue Intervallgrenzen */
    double const xan = xa + h / 2.;
    double const xen = xe - h / 2.;
    
    /* Verfeinerung des Integralwertes
     * durch Hinzufügen der Zwischenstellen
     */
    y_old[nstep] = 0.5 * ( y_old[nstep-1] + trapez_integration_restarted ( f, xan, xen, h, (void*)p ) );

    /* neuer Wert der Schrittweite = 1 / 2 der alten Schrittweite,
     * also Verwendung der Romberg-Folge
     */
    h *= 0.5;
    x_old[nstep] = h;
  
    /* Extrpolation h -> 0 mit den vorhandenen
     * Schrittweiten als Stützstellen
     */
    double val = neville ( y_old, x_old, nstep );

    /* Berechnung der neuen absoluten und
     * relativen Änderung
     */
    diffabs = fabs( val_old - val );
    diffrel = diffabs / fabs( val_old + val ) * 2.;

    /* Aktualisiere den Integralwert in
     * val_old für die nächste Iteration
     */
    val_old = val;

    /* Ausgabe des extrapolierten Wertes
     */
    fprintf ( stdout, "# [main] Romberg nstep %2u integral value = %25.16e    %e %e\n", nstep, val, diffabs, diffrel );

  }  /* end of nstep, eps iteration */

  /* Speicher freigeben
   */
  free ( x_old );
  free ( y_old );

  return ( 0 );
}
