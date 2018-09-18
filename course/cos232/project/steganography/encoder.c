#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    char* inputf = argv[1];
    char* outputf = argv[2];
    char* message = argv[3];
    FILE* infile = fopen (inputf, "r");
    FILE* outfile = fopen (outputf, "w");
    printf ("input file: %s\n", inputf);
    printf ("output file: %s\n", outputf);
    printf ("message: %s\n", message);

    int start;
    fseek (infile, 10, SEEK_SET);
    fread (&start, 4,1, infile);
    printf("starting position: %d\n", start);

    int width;
    fseek (infile ,18, SEEK_SET);
    fread (&width, 4, 1, infile);
    printf("width: %d\n", width);

    int height;
    fseek (infile, 22, SEEK_SET);
    fread (&height, 4,1, infile);
    printf("height: %d\n", height);


    fseek(infile, start, SEEK_SET);
    int pix;
    int ct;
    for (int i=0; i < width * abs(height); i++){
        fread(outfile, 4, 1, infile);
        pix = getc(infile);
        printf("%d\n", pix);
        if(i%4!=3&&ct<strlen(message)) {
            printf("%c\n", message[ct]);
            ct++;
        }
    }
}
