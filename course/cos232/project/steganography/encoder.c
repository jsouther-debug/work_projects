#include <stdio.h>

int main(int argc, char **argv) {
    char* inputf = argv[1];
    char* outputf = argv[2];
    char* message = argv[3];
    FILE* infile = fopen (inputf, "r");
    FILE* outfile = fopen (outputf, "w");
    printf ("input file: %s\n", inputf);
    printf ("output file: %s\n", outputf);
    printf ("message: %s\n", message);

}