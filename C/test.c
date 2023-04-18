// how to upload the file to my uni account
// scp C:\Users\ing-b\Documents\VisualStudioCodeProgramms\LUA\CC\test.c s6anthul@cip-pool.physik.uni-bonn.de:~/Documents/



//just a test

//compile a c program
// gcc -Wall -pedantic name.c -o FileName

//execute a file
// ./FileName

//#include <stdio.h> // #include imports lib
double square(double zahl){
    return zahl*zahl; // ; is needed to define end of command, in this case the return command
}

int main(int argc, char **argv){ // char = element from string
    printf("hello world\n");
    double a=0.5;
    printf("Das Quadrat von %f ist %f\n", a, square(a)); // %f is a placeholder and the first cronological %f is replaced with the first following argument
    return 0;
}