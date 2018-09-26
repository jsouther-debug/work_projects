#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
    int shift = 7;
    bool e = true;
    bool d = false;
    int tmp = 0;
    char text;
    int length = 10;
    char *message = malloc(length * sizeof(char));
    int count = 0;
    char c;
    FILE *file;
    int size;

    if (argc == 1) {
        printf("what do you want encrypted?:");
        while ((c = (char) getchar()) != '\n') {
            if (count >= length) {
                message = realloc(message, (length += 10) * sizeof(char));
            }
            message[count++] = c;
            //  printf("%s\n",message);
        }
    } else {
        switch (argc) {
            case 2:
                if (strcmp(argv[1], "-d") == 0) {
                    d = true;
                    e = false;
                }
                break;
            case 3:
                if (strcmp(argv[1], "-d") == 0) {
                    d = true;
                    e = false;
                }
                for (int i = 0; i < strlen(argv[2]); i++) {
                    if (!isdigit(argv[2][i])) {
                        printf("invalid number to shift");
                        return 1;
                    }
                }
                shift = (int) argv[2];
                shift = shift % 26;
                if (d) {
                    shift = shift * -1;
                }
                break;
            case 4:
                if (strcmp(argv[1], "-d") == 0) {
                    d = true;
                    e = false;
                }
                if (strcmp(argv[1], "-d") == 0) {
                    d = true;
                    e = false;
                }
                for (int i = 0; i < strlen(argv[2]); i++) {
                    if (!isdigit(argv[2][i])) {
                        printf("invalid number to shift");
                        exit(1);
                    }
                }
                shift = (int) argv[2];
                shift = shift % 26;
                if (d) {
                    shift = shift * -1;
                }
                if ((file = fopen(argv[3], "r")) == NULL) {
                    printf("invalid file");
                    return 1;
                }
                fseek(file, 0, SEEK_END);
                size = ftell(file);
                fseek(file, 0, SEEK_SET);
                message = realloc(message, size * sizeof(char) + 1);
                for (int i = 0; i < size; i++) {
                    message[i] = (char) fgetc(file);
                }
                break;
            default:
                printf("invalid arguments");
                return 1;
        }

    }


    for (int i = 0; i < strlen(message); i++) {
        if (message[i] == '\0') {
            break;
        }
        //printf("%c\n" ,message[i]);
        if (((int) message[i] > 64 && (int) message[i] < 91)) {
            if ((int) message[i] + shift > 90) {
                tmp = 90 - (int) message[i];
                message[i] = (char) (64 + (shift - tmp));
            } else if ((int) message[i] + shift < 65) {
                tmp = (int) message[i] - 65;
                message[i] = (char) (91 - (-shift - tmp));
            } else {
                message[i] = (char) (message[i] + shift);
            }
        } else if ((int) message[i] > 96 && (int) message[i] < 123) {
            if ((int) message[i] + shift > 122) {
                tmp = 122 - (int) message[i];
                message[i] = (char) (96 + (shift - tmp));
            } else if ((int) message[i] + shift < 97) {
                tmp = (int) message[i] - 97;
                message[i] = (char) (123 - (-shift - tmp));
            } else {
                message[i] = (char) (message[i] + shift);
            }
        }
        printf("%c", message[i]);
    }
    fclose(file);

    return 0;
}