#include <stdio.h>

void main() {
    FILE *fp;
    int ch,chars = 0, spaces = 0, tabs = 0, lines = 0;
    fp = fopen("sample.txt", "w+");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return 1;
    }
    fprintf(fp, "Hello world!\nThis\tis a line with a tab.\nAnd  two  spaces.\n");
    fclose(fp);
    fp = fopen("sample.txt", "r");
    if (fp == NULL) {
        printf("Error opening file for reading.\n");
        return 1;
    }
    while ((ch = fgetc(fp)) != EOF) {
        chars++;                 
        if (ch == ' ')
            spaces++;
        else if (ch == '\t')
            tabs++;            
        else if (ch == '\n')
            lines++;             
    }
    fclose(fp);
    printf("Total characters : %d\n", chars);
    printf("Spaces : %d\n", spaces);
    printf("Tabs : %d\n", tabs);
    printf("Lines : %d\n", lines);
}