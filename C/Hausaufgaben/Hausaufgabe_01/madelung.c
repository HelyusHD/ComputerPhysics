/********************************************************************************
 Programm kompelier Befehl:
 gcc -Wall -pedantic madelung.c -o madelung -lm

 Programm aufruf Befehl:
 madelung 
 ********************************************************************************/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// sums potentials of charges on half the surface of a cube 
double cube(int n){
    double SurfacePotential = 0;
    int sign = 1;

    // face facing in x direction
    int x = n;
    for (int y=-n ; y<=n ; y++){
        for (int z=-n ; z<=n ; z++){ //not full interval so that edges are not dublicated
            if (abs(x+y+z)%2 == 0){sign = -1;}else{sign = 1;} // keeping track of the charge of the ions at a lattice point
            SurfacePotential += (sign / sqrt(x*x + y*y + z*z));
        }
    }
    // face facing in y direction
    int y = n;
    for (int x=-n+1 ; x<=n-1 ; x++){
        for (int z=-n ; z<=n ; z++){
            if (abs(x+y+z)%2 == 0){sign = -1;}else{sign = 1;}
            SurfacePotential += (sign / sqrt(x*x + y*y + z*z));
        }
    }
    // face facing in z direction
    int z = n;
    for (int x=-n+1 ; x<=n-1 ; x++){
        for (int y=-n+1 ; y<=n-1 ; y++){
            if (abs(x+y+z)%2 == 0){sign = -1;}else{sign = 1;}
            SurfacePotential += (sign / sqrt(x*x + y*y + z*z));
        }
    }

//attempt to half the runtime by using symmetri of the crystal

/*
    x = -n;
    for (int y=-n ; y<=n ; y++){
        for (int z=-n ; z<=n ; z++){ //not full interval so that edges are not dublicated
            if (abs(x+y+z)%2 == 0){sign = -1;}else{sign = 1;} // keeping track of the charge of the ions at a lattice point
            SurfacePotential += (sign / sqrt(x*x + y*y + z*z));
        }
    }
    // face facing in y direction
    y = -n;
    for (int x=-n+1 ; x<=n-1 ; x++){
        for (int z=-n ; z<=n ; z++){
            if (abs(x+y+z)%2 == 0){sign = -1;}else{sign = 1;}
            SurfacePotential += (sign / sqrt(x*x + y*y + z*z));
        }
    }
    // face facing in z direction
    z = -n;
    for (int x=-n+1 ; x<=n-1 ; x++){
        for (int y=-n+1 ; y<=n-1 ; y++){
            if (abs(x+y+z)%2 == 0){sign = -1;}else{sign = 1;}
            SurfacePotential += (sign / sqrt(x*x + y*y + z*z));
        }
    }
*/

    // -x,y,z are missing so we only iterated half the cube
    return 2 * SurfacePotential;
}



double square(int n){
    double SurfacePotential = 0;
    int sign = 1;
    int z = 0;

    int x = n;
    for (int y=-n ; y<=n ; y++){
        if (abs(x+y+z)%2 == 0){sign = -1;}else{sign = 1;} // keeping track of the charge of the ions at a lattice point
        SurfacePotential += (sign / sqrt(x*x + y*y + z*z));
    }
    int y = n;
    for (int x=-n+1 ; x<=n-1 ; x++){
            if (abs(x+y+z)%2 == 0){sign = -1;}else{sign = 1;}
            SurfacePotential += (sign / sqrt(x*x + y*y + z*z));
    }
    // -x,y are missing so we only iterated half the cube
    return 2 * SurfacePotential;
}



// no inputs required
int main(int argc, char **argv){ 

    int DecimalPlaces;
    printf("This program will aproximate the Madelung constant of a NaCl cristal\nPlease specify the exact number of decimal places to be calculated:");
    scanf("%d", &DecimalPlaces);

    double MadelungConstNaCl = 1.747564; // not full accuracy 
    double VolumePotential = 0;

    double accuracy = pow(0.1,DecimalPlaces);
    int n = 0;
    while((fabs(MadelungConstNaCl - VolumePotential) >= accuracy) && n < 2000){
        n += 1;
        VolumePotential += cube(n);//sums all the charges on the cubes surfaces
        if(n%100 == 0){fprintf(stderr,"iterated %d cubes\ndifference to litrature value = %f\n",n,fabs(MadelungConstNaCl-VolumePotential));}
    }
    if (n > 2000){fprintf(stderr,"desired accuracy could not be reached");}

    fprintf(stderr,"\n-----------------\nDONE\n-----------------\n");
    fprintf(stderr,"the total potential sum = %f\nthe final cube turned out to be %d wide.\n", VolumePotential, n*2+1);
    fprintf(stderr,"the difference between the calculated and the litrature value is %f\n", fabs(MadelungConstNaCl-VolumePotential));

    fprintf(stderr,"Now we will use a flat cube aka square with the same amount of iterations to calculate the constant.");
    VolumePotential = 0;
    int g;
    for (int m=0 ; m<= n ; m++){
        VolumePotential += square(m);//sums all the charges potential on the squares circumferences
        if(n%100 == 0){fprintf(stderr,"iterated %d cubes\ndifference to litrature value = %f\n",n,fabs(MadelungConstNaCl-VolumePotential));}
        g = m;
    }

    fprintf(stderr,"\n-----------------\nDONE\n-----------------\n");
    fprintf(stderr,"the total potential sum = %f\nthe final square turned out to be %d wide.\n", VolumePotential, g*2+1);
    fprintf(stderr,"the difference between the calculated and the litrature value is %f\n", fabs(MadelungConstNaCl-VolumePotential));

    return 0;
}
