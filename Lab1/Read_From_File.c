#include<stdio.h>
void main(){
    FILE *file1;
    char ch;
    file2 = fopen("file1.txt","r");
    if(file2 == NULL){
        printf("Error opening file.");
        return;
    }
    ch = fgetc(file2);
    while(ch != EOF){
        printf("%c", ch);
        ch = fgetc(file2);
    }
    fclose(file2);
}