/***********************************************************************
 *
 * compile with
 *
 *   gcc -O3 -Wall -pedantic trapez.c -o trapez -lm
 *
 * run with
 *   ./trapez
 *
 ***********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>  /* needed for memcpy */
#include <math.h>

/* Wir definieren einen neuen Datentyp = Zeiger auf Funktion;
 * die Funktion soll folgendermassen beschaffen sein:
 * 
 * Eingabe:
 *   x = double Wert als Funktionsargument; x wird nicht verändert, also const
 *   p = pointer auf einen Speicherbereich, der Parameter 
 *       für die Funktionsauswertung enthält; das ist sehr allgemein
 *       gehalten und der Speicherbereich p kann in Abhängigkeit
 *       von der Funktion ausgelesen werden
 *
 * Rückgabe:
 *   Funktionswert in double
 *
 * Dieser Funktionstyp wird nachher als Integrand verwendet
 */
typedef double (*func_type ) ( double const x, void * p );

/***********************************************************************
 * elementarer Trapze-Schritt
 *
 * Eingabe:
 *   xa = unteres Intervallende
 *   xb = oberes Intervallende
 *   p  = Parameter für die Funktionsauswertung; p wird einfach durch-
 *        geschleift
 ***********************************************************************/
double trapez ( func_type f, double const xa, double const xe, void * p ) 
{
  return( ( f(xe, p) + f(xa, p) ) * 0.5 * ( xe - xa ) );
} /* end of trapez */

/***********************************************************************
 * Trapez-Integration
 *
 * ACHTUNG: "_restarted" heisst: Es werden NUR die Beiträge
 *                               an Zwischenstellen aufaddiert,
 *                               also alle mit Gewicht 1
 *
 * Verwendet äquidistante Stützstellen bei xa, xa+h, ..., xe
 * 
 * Eingabe:
 *   f  = Funktion, die integriert werden soll, vom Typ func_type
 *   xa = unteres Intervallende
 *   xb = oberes Intervallende
 *   p  = Parameter für die Funktionsauswertung; p wird einfach durch-
 *        geschleift
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

} /* end of trapez_integration */

/***********************************************************************
 * Es folgen Beispielfunktionen zum Integrieren
 *
 * Alle diese Funktionen sind vom Typ func_type, d.h.
 *
 * Eingabe:
 *   x = Funktionsargument, an dem auswertet werden soll
 *   p = Zeiger auf Speicherbereich, aus dem Parameter
 *       gelesen werden, die zur Funktionsauswertung notwendig sind
 ***********************************************************************/

/***********************************************************************
 * Bsp. Integrand fuer Gamma-Funktion
 *
 * exp ( -t ) t^( z - 1 ), z reell
 ***********************************************************************/
double Gamma_integrand ( double x, void * p  ) 
{
  /* cast p zu Zeiger auf double, dann wird die erste
   * double-Zahl ausgelesen;
   *
   * ACHTUNG: p muss von der aufrufenden Funktion auch so
   * angelegt worden sein
   */
  double const z = ((double*)p)[0];
  return( exp  ( -x ) * pow  ( x, z - 1. ));
}  /* end of Gamma_integrand */



/***********************************************************************
 * MAIN PROGRAM
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
  double p[1] = { 3.5 };       /* Argument z fuer Gamma ( z ) */
  double int_xa = 0.;          /* untere Intervallgrenze */
  double int_xe = 20.;         /* obere Intervallgrenze  */

  double int_epsabs = 1.e-08;  /* geforderete max. absolute Aenderung */
  double int_epsrel = 1.e-06;  /* geforderete max. relative Aenderung */

  unsigned int int_nstep_min = 10;     /* minimale Anzahl Schritte, werden auf jeden Fall durchgefuehrt */
  unsigned int int_nstep_max = 40;     /* maximal Anzahl Schritte; ob diese Anzahl Schritte erreicht wird
                                          ist abhängig von der Konvergenz
                                        */

  /* Startwerte */
  double diffrel = 2 * int_epsrel;
  double diffabs = 2 * int_epsabs;

  double h = int_xe - int_xa;

  unsigned int nstep = 1;
  
  /* Start der Integrations-Iteration mit elementarem Trapez-Schritt;
   *
   * das ist die Integration für nstep = 1,
   * also h = ( int_xe - int_xa ) / nstep = int_xe - int_xa
   */
  double int_val = trapez ( f, int_xa, int_xe, p );  /* nur Beitraege xa, xe */

  /* fprintf ( stdout, "# [main] nstep = %3u h = %25.16e   int_val = %25.16e\n", nstep, int_xe - int_xa, int_val ); */
  nstep++;

  /* Iteration bis hinauf zu nstep_min 
   *
   * in der for-Schleife geben wir keinen Startwert 
   * für die Schleifenvariable nstep, da nstep
   * bei Eintritt in die Schleife bereits den richtige
   * Wert hat;
   *
   * wir geben auch keine Änderung der Schleifenvariable, da
   * nstep innerhalb der Schleife erhöht wird
   */
  for ( ; nstep <= int_nstep_min; ) 
  {
    double const xa = int_xa + h / 2.;  /* neue untere Intervallgrenze */
    double const xe = int_xe - h / 2.;  /* neue obere Intervallgrenze  
                                         */

    /* ACHTUNG: Wir benutzen noch den ALTEN Wert für h, deshalb
     * muss hier mit 0.5 multiplizert werden, um auf den neuen Wert 
     * h_neu = h_alt / 2 zu kommen */
    int_val = 0.5 * ( int_val + trapez_integration_restarted ( f, xa, xe, h, p ) );

    /* Hier wird der neue Wert für h für die nächste Iteration berechnet, h <- h / 2 
     * */
    h *= 0.5;

    /* fprintf ( stdout, "# [main] nstep = %3u   xa = %e xe = %e   h = %25.16e   int_val = %25.16e\n", nstep, h, int_val ); */

    /* Wir erhöhen nstep von Hand um 1
     */
    nstep++;
  
  }  /* Ende der Schleife über nstep bis zum Minimalwert */
  
  /*
   * Iteration bis zu max. nstep_max
   *
   * Stoppkriterum: nstep_max, epsrel, epsabs
   *
   * Schrittweite h wir von der Schleife oben 
   * übernommen und weitergeführt
   */
  while ( nstep <= int_nstep_max && ( diffrel > int_epsrel || diffabs > int_epsabs ) )
  {

    double const xa = int_xa + h / 2.;  /* wie oben, neue Intervallgrenzen */
    double const xe = int_xe - h / 2.;

    /* der neue Trapez-Integralwert 
     */
    double val_new = 0.5 * ( int_val + trapez_integration_restarted ( f, xa, xe, h, p ) );
    h *= 0.5;

    /* Wir berechnen die absolute und relative differenz zum Wert
     * im vorherigen Interationsschritt; für das Abbruchkriterium.
     *
     * In der nächsten Iteration der while-Schleife werden nstep, 
     * diffabs und diffrel geprüft
     */
    diffabs = fabs( int_val - val_new );
    diffrel = diffabs / fabs( int_val + val_new ) * 2.;
    
    /* aktualisiere int_val 
     * auf den neuen Wert
     */
    int_val = val_new;

    /* fprintf ( stdout, "# [main] nstep = %3u h = %25.16e   int_val = %25.16e   eps %e %e\n", nstep, h, int_val, diffabs, diffrel ); */
    
    nstep++;

  }  /* end of nstep, eps iteration */
    
  nstep--;  /* tatsächlicher Wert von nstep, bei dem das Abbruchkriterium erreicht wurde;
               nstep war danach noch einmal erhöht worden
             */

  /* Ausgabe des Resultats
   */
  fprintf ( stdout, "# [main] nstep = %3u h = %25.16e   int_val = %25.16e   eps %e %e\n", nstep, h, int_val, diffabs, diffrel );

  return ( 0 );
}
