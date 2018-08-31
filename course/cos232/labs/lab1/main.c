#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    printf("hello world\n"); // no additional arguments
    printf("hello %s\n", "world"); // one additional argument to satisfy the %s
    printf("meaining of life is: %d\n", 42); // one additional argument to satisfy %d
    printf("%d + %d = %d\n", 2, 2, 2+2); //three additional arguments to satisfy 3 %d's

    printf("Size of char: %d\n", (int) sizeof(char));
    printf("Size of short: %d\n", (int) sizeof(short));
    printf("Size of int: %d\n", (int) sizeof(int));
    printf("Size of long: %d\n", (int) sizeof(long));

    char x = 0x54;
    printf("%c\n", x);
    printf("%d\n", x);
    printf("%x\n", x);

    char x2 = 'T';
    printf("%c\n", x2);
    printf("%d\n", x2);
    printf("%x\n", x2);

    char x3 = 84;
    printf("%c\n", x3);
    printf("%d\n", x3);
    printf("%x\n", x3);

    char y[5];
    y[0] = 0x54;
    y[1] = 0x41;
    y[2] = 0x43;
    y[3] = 0x4f;
    y[4] = 0x53; //null-terminated string
    printf("String: %s\n", y);
    printf("Null: %x\n", y[4]);

    char * y2 = "TACO";
    printf("String: %s\n", y2);
    printf("Null: %x\n", y2[4]);

    int * poof_youre_a_32bit_int = (int *) y;
    printf("As an int: %d\n", *poof_youre_a_32bit_int);
    printf("In hex: %x\n", *poof_youre_a_32bit_int);
    return 0;
}