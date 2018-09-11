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

    signed int start;
    fseek (infile, 10, SEEK_SET);
    fread (&start, 4,1, infile);
    printf("starting position: %d\n", start);

    signed int width;
    fseek (infile ,18, SEEK_SET);
    fread (&width, 4, 1, infile);
    printf("width: %d\n", width);

    signed int height;
    fseek (infile, 22, SEEK_SET);
    fread (&height, 4,1, infile);
    printf("height: %d\n", height);

}
