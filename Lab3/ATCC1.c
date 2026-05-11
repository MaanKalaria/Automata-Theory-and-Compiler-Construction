#include <stdio.h>
#include <stdlib.h>
int main()
{
    FILE *f, *f1;
    f = fopen("ex.txt", "r");
    f1 = fopen("cp.txt", "w");
    char c, p;

    while ((c = fgetc(f)) != EOF)
    {
        if (c == '/'){
            if (c == '/')
            {
                do
                {
                    c = fgetc(f1);
                } while (c != EOF && c != '\n');
                printf("\n Single line comment is ignored.");
            }       
        }
        else{
            c = fputc(c,f1);
        }
    }
}