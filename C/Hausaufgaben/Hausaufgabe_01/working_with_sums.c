// i want to try out some ways to make recursive sums in C

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/**********************************************************
 * Summation over n iterations 
**********************************************************/
double sum(int data[], int length);// this is a superior way to define function, it is more easy to read

/**********************************************************
* MAIN PROGRAMM
**********************************************************/
int main(){
    int test[] = {1,2,3,4,5,6,7,8,9,0}; 
    int a = (int)sum(test,10); // trying to cast sum as an int
    fprintf(stdout, "The sum equals to: %d\n", a);
    return 0;
}

double sum(int data[], int length){ // here i define my function
    double sum_value = 0; // this is a temporary variable for iterating, it has to be initialzed to have a starting point
    for(int i = 0 ; i < length ; i++){
        sum_value = sum_value + data[i];
    }
    return sum_value; // the function return its solution
}