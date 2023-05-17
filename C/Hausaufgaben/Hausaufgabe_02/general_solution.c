#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#define _USE_MATH_DEFINES
#include <math.h>


typedef double (*func_type) (double const x, void* p);                   // Definition eines neuen Datentyps = Zeiger auf Fuktionen

double trapez(func_type f, double const x1, double const x2, void* p)    // Erstes Trapez mit zwei Stützstellen
{
    return((f(x1, p) + f(x2, p)) * 0.5 * (x1 - x2));                     // Trapez-Regel
} 

double intermediate_steps(func_type f, double const xa, double const xe, double const h, void* p)      // Berechnung der Zwischenstellen
{
	int const n = (xe - xa) / h;  // Anzahl der Zwischenschritte

	double sum = 0.;
	double x = xa;

	for (int i = 0; i <= n; i++)   // Aufsummieren der Zwischenwerte
	{
		sum += f(x, p);
		x += h;
	}
	sum *= h;

	return(sum);
}

double neville(double* const y, double const* x, unsigned int const m)    // Neville-Schema für Extrapolation bei 0 
{
	unsigned int k = 1;

	while (k <= m)
	{
		for (unsigned int i = 0; i <= m - k; i++)
		{
			double const xr = x[i] / x[k + i];
			y[i] = y[i + 1] + (y[i + 1] - y[i]) / (xr * xr - 1);
		}
		k++;
	}
	return (y[0]);   // Rückgabewert ist
}

double potential(double x, void* p)     // Funktion für das Potential mit Integrationsvariable x, sowie Variablen a und z, die variiert werden können
{
	double const z = ((double*)p)[0];
    double const a = ((double*)p)[1];

	return(exp(-(x*x)/(a*a)) / (sqrt(M_PI)*a*sqrt(x*x + z*z)));
}  
 
double efield(double x, void* p)
{
    double const z = ((double*)p)[0];
    double const a = ((double*)p)[1];

    return (exp(-(x * x) / (a * a))*z) / (sqrt(M_PI) * a * pow(x * x + z * z,1.5));
}

double potentialH3(double x, void* p)
{
    double const z = ((double*)p)[0];
    double const a = ((double*)p)[1];

    return (exp(-(x * x) / (4. * 4.)) / (sqrt(M_PI) * 4. * sqrt(x * x + z * z))) + (4*exp(-(x * x) / (a * a)) / (sqrt(M_PI) * a * sqrt(x * x + (z-12.) * (z-12.))));
}
 
double integral(func_type f, void* p)          // Funktion zur Berechnung des Integrals
{
    double xa = -100.;     // Integrationsgrenzen
    double xe = 100.;

    double epsrel = 1.e-08;  // vorgegebene relative Genauigkeit

       // minimale und maximale Anzahl an Schritten
    unsigned int n_max = 40;

    unsigned int n = 1;
    double diffrel = 2 * epsrel;
    double diffabs = 0.;


    double* y_old = (double*)malloc((n + 1) * sizeof(double));
    double* x_old = (double*)malloc((n + 1) * sizeof(double));


    double h = xe - xa;

    y_old[0] = trapez(f, xa, xe, (void*)p);   // Start mit dem elementaren Trapez 
    x_old[0] = h;

    for (unsigned int i = 1; i <= n; i++)     // Schleife zum Einfügen der Zwischenschritte
    {

        double const xan = xa + h / 2.;       // neue Intervallgrenzen
        double const xen = xe - h / 2.;

        y_old[i] = 0.5 * (y_old[i - 1] + intermediate_steps(f, xan, xen, h, p));

        h *= 0.5;

        x_old[i] = h;
    }

    double val_old = neville(y_old, x_old, n);   // erster Wert der Integration durch Extrapolation für h -> 0

    double val = 0.;                           
    while (n <= n_max && diffrel > epsrel)  // Abbruchkriterien: maximale Schrittzahl wird überschritten oder gewünschte relative Genauigkeit wird erreicht
    {
        n++;

        y_old = (double*)realloc(y_old, (n + 1) * sizeof(double));       // Reservierung neuen Speichers um ein Double für die Extrapolation
        x_old = (double*)realloc(x_old, (n + 1) * sizeof(double));

        double const xan = xa + h / 2.;
        double const xen = xe - h / 2.;

        y_old[n] = 0.5 * (y_old[n - 1] + intermediate_steps(f, xan, xen, h, (void*)p)); // neue Zwischenstellen
 
        h *= 0.5;       // neue Schrittweite (Romberg-Folge h/2^m)
        x_old[n] = h;
     
        val = neville(y_old, x_old, n); // Extrapolation

        diffabs = fabs(val_old - val);      // absolute Änderung
        diffrel = diffabs / fabs(val_old + val) * 2.; // relative Änderung

        val_old = val;   // Aktualisiere
    }  

    free(x_old);  // Freigeben des Speichers
    free(y_old);

    return (val); // Ausgabe des Integralwerts
}


double derivative(func_type f, double z, const double a)
{
    double h = 0.5;
    double val = 0.;
    double epsrel = 1.;
    int n = 0;

    while (epsrel > 1.e-08)
    {
        if ( (z - h) == 0. || z + h == 12.)
        {
            h *= 0.5;
        }
        else
        {
            double val_old = val;
            double p1[2] = { z - h,a };
            double p2[2] = { z + h,a };
            val = -(integral(f, p2) - integral(f, p1)) / (2 * h);
            h *= 0.5;
            epsrel = fabs(val_old - val);
            ++n;
        }
    }
    return val;
}


void file(char* filename, double x1, double const x2, double const step, double const a,func_type f)
{
    FILE* fp = NULL;
    fp = fopen(filename, "w");
    for (x1; x1 <= x2;)
    {
        double p[2] = { x1,a};
        fprintf(fp, "%f %f\n", integral(f,p), x1);
        x1 += step;
    }
    fclose(fp);
}

void e_file(char* filename, double x1, double const x2, double const step, double const a, func_type f)
{
    FILE* fp = NULL;
    fp = fopen(filename, "w");
    for (x1; x1 <= x2;)
    {
        fprintf(fp, "%f %f\n", derivative(f,x1,a), x1);
        x1 += step;
    }
    fclose(fp);
}



double sekant(double x1, double x2, double eps, const double v)
{
    double a = derivative(potentialH3,x1,v);
    double b = derivative(potentialH3,x2,v);

    int iter = 0;

    if (fabs(a - b) < eps)
    {
        fprintf(stderr, "error\n");
    }

    while (fabs(x1 - x2) > eps)
    {
        double x = (x1 * b - x2 * a) / (b - a);
        ++iter;

        a = b;
        b = derivative(potentialH3,x,v);
        x1 = x2;
        x2 = x;
    }
    fprintf(stdout, "# [sekant] Anzahl iterationen = %4d\n", iter);
    return (x2);
}

int main(int argc, char** argv)
{
    file("potential1.txt", 0.1, 3., 0.01, 0.1, potential);
    file("potential2.txt", 0.05, 3., 0.01, 1., potential);
    file("potential3.txt", 0.05, 3., 0.01, 2., potential);
    file("potential4.txt", 0.05, 3., 0.01, 4., potential);


    e_file("method11.txt", 0.1, 3., 0.1, 1., potential); // Methode über Differenzenquotien
    file("method2.txt", 0.1, 3., 0.1, 1., efield);    // erst Ableitung, dann numerische Integration

    e_file("efieldH3.txt", 0.1, 11., 0.1, 1., potentialH3);
    file("potentialH3.txt", 0.05, 11., 0.1, 1., potentialH3);
   
 
    fprintf(stdout, "%f\n", derivative(potential,2.5,1.));


    file("potential5.txt", 0.1, 11.9, 0.05, 1., potentialH3);

    fprintf(stdout, "%f\n", sekant(0.05, 11.95, 1.e-08, 4.));

    /*FILE* f1 = NULL;
    f1 = fopen("sekant.txt", "w");
    for (double i = 0.4; i <= 4.;)
    {
        fprintf(f1, "%f %f\n", sekant(0.05, 11.95, 1.e-12, i), i);
        i += 0.3;
    }
    fclose(f1);*/

    return (0);
}