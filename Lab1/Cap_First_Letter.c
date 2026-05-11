#include <stdio.h>
#include <ctype.h>
void main() {
    FILE *in, *out;
    char ch;
    int start = 1;
    in = fopen("file.txt", "r");
    out = fopen("output.txt", "w");
    if (in == NULL || out == NULL) {
        printf("File is empty.\n");
        return 1;
    }
    while ((ch = fgetc(in)) != EOF) {
        if (start == 1 && ch != ' ' && ch != '\n' && ch != '\t') {
            ch = toupper(ch);
            start = 0;
        }
        if (ch == ' ' || ch == '\n' || ch == '\t') {
            start = 1;
        }
        fputc(ch, out);
    }
    fclose(in);
    fclose(out);
    printf("Done!\n");
}