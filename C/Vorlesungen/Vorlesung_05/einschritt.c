/***********************************************************************
 * Kompilieren mit
 *
 *   gcc -O2 -Wall -pedantic -o einschritt einschritt.c -lm
 *
 * Ausführen mit
 *
 *   ./ec
 *
 ***********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/* Funktionstyp für den verallgemeinerten Kraft-Term
 *   f = f(t, y(t)) = d/dt y(t)
 */
typedef double (*func_type )(double const, double const );

/* Funktionstyp für den Integrator 
 */
typedef double (*integrator_type) (double , double const, func_type, int const, double const);


/* Mit verbose steuern wie die Menge an Ausgaben des Programms,
 * insbesondere in Funktion euler_cauchy (Bahnkurven schreiben ja oder nein)
 */
int verbose = 0;

/***********************************************************************
 * Exakte Lösung des Harmonischen Oszillators
 *
 * y(t) = y0 * exp ( -1/2 (t - t0)^2 )
 *
 ***********************************************************************/
double y_exact (double const t, double const t0, double const y0 )
{
   double const z = y0 * exp ( -0.5 * ( t * t - t0 * t0 ) );

  return ( z );

}  /* end of y_exact */


/***********************************************************************
 * Kraft-Term für Differentialgleichung
 *
 ***********************************************************************/
double f_force (double const t, double const y )
{
  double const f = -t * y;

  return ( f );

}  /* end of f_force */

/***********************************************************************
 * Euler-Cauchy-Integration
 *
 * Zeitentwicklung für nstep Schritte mit Schrittweite h
 *
 ***********************************************************************/
double euler_cauchy ( double y, double const ta, func_type force,  int const nstep,  double const h ) 
{
  if ( verbose > 2 )
  {
    fprintf(stdout, "# [euler_cauchy] Schritte = %d\n", nstep );
  }

  double t = ta;

  if ( verbose > 2 ) 
  {
    fprintf(stdout, "ec y   %16.7e  %16.7e  %25.16e\n", h, t, y );
  }

  for ( int i = 0; i < nstep; i++ )
  {
    double const f = force ( t, y );

    /* Iterationsvorschrift für den 
     * Polygonzug nach Euler-Cauchy
     */
    y += h * f;

    t += h;

    if ( verbose > 2 ) 
    {
      fprintf(stdout, "ec y   %16.7e  %16.7e  %25.16e\n", h, t, y );
    }
  }

  return ( y );

}  /* end of euler_cauchy */


/***********************************************************************
 * Heun-Integration
 *
 * Zeitentwicklung für nstep Schritte mit Schrittweite h
 *
 ***********************************************************************/
double heun ( double y, double const ta, func_type force,  int const nstep,  double const h ) 
{

  double const a[2] = { 0.5, 0.5 };
  double const p[2] = { 1.0, 1.0 };


  if ( verbose > 2 )
  {
    fprintf(stdout, "# [heun] Schritte = %d\n", nstep );
  }

  double t = ta;

  if ( verbose > 2 ) 
  {
    fprintf(stdout, "heun y   %16.7e  %16.7e  %25.16e\n", h, t, y );
  }

  for ( int i = 0; i < nstep; i++ )
  {
    /* f0 = f ( t, y(t) ) */
    double const f0 = force ( t, y );

    /* f1 = f ( t + p_0 * h , y + h * p_1 * f ( t, y(t) ) )  */
    double const f1 = force ( t + p[0] * h, y + h * p[1] * f0 );

    /* Iterationsvorschrift nach Heun */
    double const phi = a[0] * f0 + a[1] * f1;

    /* aktualisiere y und t */
    y += h * phi;

    t += h;

    if ( verbose > 2 ) 
    {
      fprintf(stdout, "heun y   %16.7e  %16.7e  %25.16e\n", h, t, y );
    }
  }

  return ( y );

}  /* end of heun */


/***********************************************************************
 * Modifiziertes Euler-Verfahren
 ***********************************************************************/
double mod_euler ( double y, double const ta, func_type force,  int const nstep,  double const h ) 
{

  double const a[2] = { 0.0, 1.0 };
  double const p[2] = { 0.5, 0.5 };


  if ( verbose > 2 )
  {
    fprintf(stdout, "# [mod_euler] Schritte = %d\n", nstep );
  }

  double t = ta;

  if ( verbose > 2 ) 
  {
    fprintf(stdout, "mod_euler y   %16.7e  %16.7e  %25.16e\n", h, t, y );
  }

  for ( int i = 0; i < nstep; i++ )
  {
    /* f0 = f ( t, y(t) ) */
    double const f0 = force ( t, y );

    /* f1 = f ( t + p_0 * h , y + h * p_1 * f ( t, y(t) ) )  */
    double const f1 = force ( t + p[0] * h, y + h * p[1] * f0 );

    /* Iterationsvorschrift mit modfiziertem Euler */
    double const phi = a[0] * f0 + a[1] * f1;

    /* aktualisiere y und t */
    y += h * phi;

    t += h;

    if ( verbose > 2 ) 
    {
      fprintf(stdout, "mod_euler y   %16.7e  %16.7e  %25.16e\n", h, t, y );
    }
  }

  return ( y );

}  /* end of mod_euler */

/***********************************************************************
 * Klassisches Runge-Kutta-Verfahren 3. Ordnung
 ***********************************************************************/
double rk3 ( double y, double const ta, func_type force,  int const nstep,  double const h ) 
{
  if ( verbose > 2 )
  {
    fprintf(stdout, "# [rk3] Schritte = %d\n", nstep );
  }

  double t = ta;

  if ( verbose > 2 ) 
  {
    fprintf(stdout, "rk3 y   %16.7e  %16.7e  %25.16e\n", h, t, y );
  }

  for ( int i = 0; i < nstep; i++ )
  {

    /* die 3 Hilfsrichtungen 
     */
    double const k1 = force ( t          , y                );

    double const k2 = force ( t + 0.5 * h, y + 0.5 * h * k1 );

    double const k3 = force ( t +       h, y - h * k1 + 2 * h * k2 );

    /* Iterationsvorschrift nach Runge-Kutta 4. Ordnung
     */
    double const phi = ( k1 + 4. * k2 + k3 ) / 6.;

    /* aktualisiere y und t */
    y += h * phi;

    t += h;

    if ( verbose > 2 ) 
    {
      fprintf(stdout, "rk3 y   %16.7e  %16.7e  %25.16e\n", h, t, y );
    }
  }

  return ( y );

}  /* end of rk3 */


/***********************************************************************
 * Klassisches Runge-Kutta-Verfahren 4. Ordnung
 ***********************************************************************/
double rk4 ( double y, double const ta, func_type force,  int const nstep,  double const h ) 
{
  if ( verbose > 2 )
  {
    fprintf(stdout, "# [rk4] Schritte = %d\n", nstep );
  }

  double t = ta;

  if ( verbose > 2 ) 
  {
    fprintf(stdout, "rk4 y   %16.7e  %16.7e  %25.16e\n", h, t, y );
  }

  for ( int i = 0; i < nstep; i++ )
  {

    /* die 4 Hilfsrichtungen 
     */
    double const k1 = force ( t          , y                );

    double const k2 = force ( t + 0.5 * h, y + 0.5 * h * k1 );

    double const k3 = force ( t + 0.5 * h, y + 0.5 * h * k2 );

    double const k4 = force ( t +       h, y +       h * k3 );


    /* Iterationsvorschrift nach Runge-Kutta 4. Ordnung
     */
    double const phi = ( k1 + 2. * k2 + 2. * k3 + k4 ) / 6.;

    /* aktualisiere y und t */
    y += h * phi;

    t += h;

    if ( verbose > 2 ) 
    {
      fprintf(stdout, "rk4 y   %16.7e  %16.7e  %25.16e\n", h, t, y );
    }
  }

  return ( y );

}  /* end of rk4 */



/***********************************************************************
 * MAIN PROGRAM
 ***********************************************************************/

int main ( int argc, char **argv )
{

  /* verallgemeinerter Kraftterm
   *   force_ho   = harmomischer Oszillator oder
   */
  func_type force = f_force;
 
  /* Integrationsfunktion für 
   * die iterative Lösung der gDGL
   */
  integrator_type integrator[5] = { euler_cauchy, heun, mod_euler, rk3, rk4 };

  char integrator_name[5][20] = { "euler_cauchy" , "heun", "mod_euler", "rk3", "rk4" };

  int integrator_num = 5;

  /* Anfangswert für y, also Ort und Geschwindigkeit */
  double const y0 = 5.;

  /* Schleife über Integrations-Funktionen */
  for ( int i = 0; i < integrator_num; i++ )
  {

    /* Startwert für die Schrittweite und Anzahl der
     * Schrittweiten für die Bahnkurven;
     *
     * die Iteration verkleinert h um den Faktor
     * 1/2
     */
    double h = 0.5;

    int nh = 16;

    /***********************************************************************
     * (1) Bahnkurven
     ***********************************************************************/

    /* Anfangs- und Endzeitpunkt */
    double ta = 1., te = 6.;

    verbose = 3;
  
    /* Schleife über Schrittweiten */
    for ( int k = 0; k < nh; k++ )
    {
      int const nstep = abs( (int)( ( te - ta ) / h ) );
 
      double const z = integrator[i] ( y0, ta, force, nstep, h );
      
      fprintf (stdout, "%.20s end %16.7e    %25.16e  \n", integrator_name[i], h, fabs(z - y_exact ( te, ta, y0 ) ) );
   
      /* verkleinere die Schrittweite */
      h *= 0.5;

    }  /* Ende Schleife über Schrittweiten h */


    /***********************************************************************
     * (2) Integrationsfehler-Verhalten mit h,
     *
     *   nur für harmonischen Oszillator, für den wir eine exakte Lösung
     *   haben
     ***********************************************************************/
    h = 0.5;

    nh = 32;
  
    ta = 2.0;

    verbose = 2;
  
    for ( int k = 0; k < nh; k++ )
    {
      /* ein Schritt in t, also te = ta + 1 Schrittweite */
      te = ta + h;

      /* Lösung der Bewegungsgleichung, nur 1 Zeitschritt */
      double const z_ec = integrator[i] ( y0, ta, force, 1, h );
    
      /* Analytische Lösung */
      double const z_ex = y_exact ( te, ta, y0 );
   
      fprintf (stdout, "%.20s diff %16.7e    %25.16e  \n", integrator_name[i], h, fabs(z_ec - z_ex ) );

      h *= 0.7;

    }  /* Ende Schleife über Schrittweiten h */

  }  /* Ender der Schleife über die Integrationsfunktionen */

  return ( 0 );

}
