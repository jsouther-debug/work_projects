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

    int lom = strlen(message)*8;
    int pct = 1;
    int nb = 8;
    int mb=8;

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

    rewind(infile);

    for(int i = 0; i < 54; i++) {
        char head = getc(infile);
        putc(head, outfile);
    }


    fseek(infile, start, SEEK_SET);
    int pix;
    int ct;
    int done = 0;
    for (int i=0; i < width * abs(height) * 4; i++) {
        //fread(outfile, 4, 1, infile);
        pix = getc(infile);
        //printf("%d\n", pix);
        if (done == 0) {
            if (lom != 0) {
                if (pct < 4) {
                    if (mb == 0) {
                        ct++;
                        mb = 8;
                    }
                    if ((message[ct] & 1) == 1) {
                        if (pix % 2 == 0) {
                            pix = pix + 1;
                        }
                    }
                    if ((message[ct] & 1) == 0) {
                        if (pix % 2 == 1) {
                            pix = pix - 1;
                        }
                    }
                    message[ct] = message[ct] >> 1;
                    lom--;
                    mb--;
                    pct++;
                    putc(pix, outfile);
                } else {
                    putc(pix, outfile);
                    pct = 1;
                }
            } else {
                if (nb != 0) {
                    if (pct < 4) {
                        if (pix % 2 == 1) {
                            pix--;
                        }
                        pct++;
                        nb--;
                        putc(pix, outfile);
                    }
                    else {
                        putc(pix, outfile);
                        pct=1;
                    }
                }
                else{
                    done=1;
                    putc(pix, outfile);
                }
            }

        }
        else{
            putc(pix, outfile);
        }
    }
    fclose(outfile);

}
