#include <stdio.h>
int main() {
    FILE *file1, *file2;
    char ch;
    file1 = fopen("file.txt", "a");   
    file2 = fopen("file1.txt", "r");   
    if (file1 == NULL || file2 == NULL) {
        printf("File is empty.\n");
        return 1;
    }
    while ((ch = fgetc(file2)) != EOF) {
        fputc(ch, file1);   
    }
    printf("Appended successfully.\n");
    fclose(file1);
    fclose(file2);
    return 0;
}
