#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    printf("hello world\n"); // no additional arguments
    printf("hello %s\n", "world"); // one additional argument to satisfy the %s
    printf("meaining of life is: %d\n", 42); // one additional argument to satisfy %d
    printf("%d + %d = %d\n", 2, 2, 2+2); //three additional arguments to satisfy 3 %d's
    return 0;
}