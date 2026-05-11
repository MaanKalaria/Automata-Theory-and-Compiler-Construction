/* sample_input.c - small test for lexical analyzer */
#include <stdio.h>

int main() {
    int a = 10;
    float b = 3.14;
    // single-line comment
    printf("Hello %d\n", a);
    /* block comment
       spanning multiple lines */
    if (a > 0 && b < 10.0) a++;
    char ch = '\'c\'';
    return 0;
}
