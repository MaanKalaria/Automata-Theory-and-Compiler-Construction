#include<stdio.h>
void main(){
    FILE *f1;
    f1 = fopen("file.txt","w+");
    fprintf(f1,"Hello, World!");
    fclose(f1);
}