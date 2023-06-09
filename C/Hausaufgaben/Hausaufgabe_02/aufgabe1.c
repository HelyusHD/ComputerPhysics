/***********************************************************************
 *
 * compile with
 *
 *   gcc -Wall -pedantic aufgabe1_copy.c -o aufgabe1_copy -lm
 *
 * run with
 *   ./aufgabe1_copy
 *
 ***********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


typedef double (*func_type ) ( double const x, void * p );
// we define a typ, which contains the information of our function 

double trapez_integration ( func_type f, double const xa, double const xe, double const h, void * p )
{

  int const nstep = ( xe - xa ) / h;// count of steps

  double tsum = ( f ( xa, p ) + f( xe, p ) ) * 0.5;// calculate the borders


  for ( int i = 1; i < nstep ; i++ ) {
    tsum += f( xa + i * h, p );
  } // this loop calculate the tsum for the points in between

  tsum *= h;

  return( tsum );

} // here we integrate by the trapez method, i want to use this for the outer support points

double trapez_integration_restarted ( func_type f, double const xa, double const xe, double const h, void * p )
{

  int const nstep = ( xe - xa ) / h;// count of steps

  double tsum = 0.; // here i initiate tsum
  double x    = xa;

  for ( int i = 0; i <= nstep ; i++ )
  {
    tsum += f( x, p );
    x    += h;
  }
  
  tsum *= h;

  return( tsum );

} // function to calculate the integration for the support points in between 

double neville ( double * const y, double const * x , unsigned int const m )
{
  unsigned int k = 1;
  
  while ( k <= m ) 
  {
    for ( unsigned int i = 0; i <= m-k; i++ ) 
    {
      double const xr = x[i] / x[k+i];
      y[i] = y[i+1] + ( y[i+1] - y[i] ) / ( xr * xr - 1 );
    }
    k++;
  }

  return ( y[0] );

} // we use neville for h=0, as the "exact" function

/***********************************************************************
 * here i define my function for the romberg integral
 * 
 * alpha * q/a * exp ( -x^2/a^2 )
 ***********************************************************************/

double Ladungsdichte (double x, void * p)
{
  double const faktor = ((double*)p)[0]; // a wird auf p[0] gespeichert etc
  double const alpha = ((double*)p)[1];
  double const q = ((double*)p)[2];
  double const a = ((double*)p)[3];
  double const z = ((double*)p)[4];
  return( faktor * alpha * q/a * exp (-pow(x,2)/pow(a,2))/sqrt(pow(x,2)+pow(z,2)));
}

//call integral(Ladungsdichte, {1.0 , sqrt(M_PI) , 1.0, .5 , 0.1}, "potential.txt", 200., 2.) // interval_lenght == distance from 0

int integral(func_type f, double p[], char *filename_write, double interval_steps, double interval_lenght){

  FILE * fp = fopen ( filename_write, "a" );

  //fprintf(fp, "%f\n", p[3]); //only used for debuging

  for(int j = -interval_steps ; j <= interval_steps ; j++){ // for loop to vary over z

  //fprintf(stderr,"%d",interval_steps);

      p[4] = interval_lenght / interval_steps*j;


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

    double h = xe - xa;

    y_old[0] = trapez_integration ( f, xa, xe, h, (void*)p );
    x_old[0] = h;

    for ( unsigned int i = 1; i <= nstep; i++ ) 
    {
    
      double const xan = xa + h / 2.;  /* neue untere Intervallgrenze */
      double const xen = xe - h / 2.;  /* neue obere Intervallgrenze  
                                            */
      y_old[i] = 0.5 * ( y_old[i-1] + trapez_integration_restarted ( f, xan, xen, h, p ) );

      h *= 0.5;

      x_old[i] = h;
    }

      /* erster Romberg-Integrationswert mit nstep = nstep_min */
      double val_old = neville ( y_old, x_old , nstep );

      while ( nstep <= nstep_max && ( diffrel > epsrel || diffabs > epsabs ) ) 
      {

        nstep++;

        y_old = (double*) realloc( y_old, ( nstep + 1 ) * sizeof( double ) );
        x_old = (double*) realloc( x_old, ( nstep + 1 ) * sizeof( double ) );
        // we use realloc to write every newstep into our arrays


        double const xan = xa + h / 2.;
        double const xen = xe - h / 2.;
        // here we define our new integral

        y_old[nstep] = 0.5 * ( y_old[nstep-1] + trapez_integration_restarted ( f, xan, xen, h, (void*)p ) );
        // we add points in between our old points


        h *= 0.5;
        x_old[nstep] = h;
        // we shorten our stepwidth

        double val = neville ( y_old, x_old, nstep );
        // we use neville to extrapolate our step width to h approach 0


        diffabs = fabs( val_old - val );
        diffrel = diffabs / fabs( val_old + val ) * 2.;
        // we calculate our error

        val_old = val;
        //write integral value to new variable

      }
      //printf("%f\n", val_old);
      fprintf(fp, "%f  %f\n",p[4] , val_old);
      
    } 
    fclose(fp); 
  
  /* Speicher freigeben
  * free ( x_old );
  * free ( y_old );
  */
  return 0;
}

// aproximates the derivitive at a point using the difference quotient
double derivative(func_type f, double z, const double thicknes)
{
    double del = 0.25; // init detha z
    double drivtv = 0.; // init
    double relative_error = 1.;

    while (relative_error > 1.e-08)
    {
        if ( (z - del) == 0. || z + del == 12.)
        {
            del *= 0.5;
        }
        else
        {
            double drivtv_old = drivtv;

            //double p1[2] = { z - del,thicknes };
            double p_1[] = {1.0 , sqrt(M_PI) , 1.0, thicknes , 0.1};
            //double p2[2] = { z + del,thicknes };
            double p_2[] = {1.0 , sqrt(M_PI) , 1.0, thicknes , 0.1};

            drivtv = -(integral(f, p_2) - integral(f, p_1)) / (2 * del); // this is the classical difference quotient
            del *= 0.5;
            relative_error = fabs(drivtv_old - drivtv);
            fprintf(stderr,"%f\n",relative_error);
        }
    }
    return drivtv;
}





/***********************************************************************
 * Hauptprogramm
 ***********************************************************************/
int main(int argc, char **argv)
{
  // create potential values on the z axis for 3 different "a" parameters

  //              const alpha        charge  a     _    
  double p_1[] = {1.0 , sqrt(M_PI) , 1.0,    0.5 , 0.1};
  double p_2[] = {1.0 , sqrt(M_PI) , 1.0,    1.0 , 0.1};
  double p_3[] = {1.0 , sqrt(M_PI) , 1.0,    1.5 , 0.1};
  double p_4[] = {1.0 , sqrt(M_PI) , 1.0,    2.0 , 0.1};
  integral(Ladungsdichte, p_1, "potential_1.txt", 200., 2.);
  integral(Ladungsdichte, p_2, "potential_2.txt", 200., 2.);
  integral(Ladungsdichte, p_3, "potential_3.txt", 200., 2.);
  integral(Ladungsdichte, p_4, "potential_4.txt", 200., 2.);
  
  //         function_name  z  thicknes
  derivative(Ladungsdichte, 1, 0.1);
  return 0 ;
}

/*DINGE DIE ICH AM CODE VERBESSERN MÖCHTE

ich möchte meine ausgabe in eine txt datei schreiben, mein problem ich habe immer nur einen einzelnen eintrag

ich möchte mein code für verschiedene a automatisch ausführen lassen

*/
