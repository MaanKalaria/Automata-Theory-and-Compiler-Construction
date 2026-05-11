// Write a program to implement lexical analyzer.
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAX 100
char input[MAX];
int i = 0;

void skip_whitespace()
{
    while (isspace(input[i]))
    {
        i++;
    }
}

void identifier()
{
    char id[MAX];
    int j = 0;
    while (isalnum(input[i]) || input[i] == '_')
    {
        id[j++] = input[i++];
    }
    id[j] = '\0';
    printf("<IDENTIFIER, \"%s\">\n", id);
}

void number()
{
    char num[MAX];
    int j = 0;
    while (isdigit(input[i]))
    {
        num[j++] = input[i++];
    }
    num[j] = '\0';
    printf("<NUMBER, \"%s\">\n", num);
}

void special_symbol()
{
    char sym = input[i++];
    printf("<SPECIAL_SYMBOL, \"%c\">\n", sym);
}

void main()
{
    printf("Enter the input string:\n");
    fgets(input, MAX, stdin);
    while (input[i] != '\0')
    {
        skip_whitespace();
        if (isalpha(input[i]) || input[i] == '_')
        {
            identifier();
        }
        else if (isdigit(input[i]))
        {
            number();
        }
        else if (ispunct(input[i]))
        {
            special_symbol();
        }
        else
        {
            i++;
        }
 
    }
}
