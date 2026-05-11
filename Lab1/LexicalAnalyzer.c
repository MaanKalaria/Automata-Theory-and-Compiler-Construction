#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const char *keywords[] = {
    "auto","break","case","char","const","continue","default","do","double",
    "else","enum","extern","float","for","goto","if","inline","int","long",
    "register","restrict","return","short","signed","sizeof","static","struct",
    "switch","typedef","union","unsigned","void","volatile","while","_Bool",
    "_Complex","_Imaginary"
};
const int num_keywords = sizeof(keywords)/sizeof(keywords[0]);

int isKeyword(const char *str) {
    for (int i = 0; i < num_keywords; ++i)
        if (strcmp(str, keywords[i]) == 0) return 1;
    return 0;
}

void print_token(const char *type, const char *lexeme) {
    printf("%s : %s\n", type, lexeme);
}

int main(int argc, char **argv) {
    FILE *fp = NULL;
    if (argc >= 2) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            fprintf(stderr, "Could not open file: %s\n", argv[1]);
            return 1;
        }
    } else {
        fp = stdin;
        printf("Reading from stdin. Press Ctrl+Z (Windows) or Ctrl+D (Unix) to finish.\n");
    }

    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (isspace(c)) continue;

        // Identifiers or keywords (start with letter or _)
        if (isalpha(c) || c == '_') {
            char buf[256];
            int idx = 0;
            buf[idx++] = (char)c;
            while ((c = fgetc(fp)) != EOF && (isalnum(c) || c == '_')) {
                if (idx < (int)sizeof(buf)-1) buf[idx++] = (char)c;
            }
            buf[idx] = '\0';
            if (c != EOF) ungetc(c, fp);
            if (isKeyword(buf)) print_token("KEYWORD", buf);
            else print_token("IDENTIFIER", buf);
            continue;
        }

        // Numbers (integer and floating)
        if (isdigit(c)) {
            char buf[256];
            int idx = 0;
            buf[idx++] = (char)c;
            int is_float = 0;
            while ((c = fgetc(fp)) != EOF) {
                if (isdigit(c)) {
                    if (idx < (int)sizeof(buf)-1) buf[idx++] = (char)c;
                } else if (c == '.' && !is_float) {
                    is_float = 1;
                    if (idx < (int)sizeof(buf)-1) buf[idx++] = (char)c;
                } else {
                    break;
                }
            }
            buf[idx] = '\0';
            if (c != EOF) ungetc(c, fp);
            if (is_float) print_token("FLOAT_CONST", buf);
            else print_token("INT_CONST", buf);
            continue;
        }

        // Strings
        if (c == '"') {
            char buf[1024];
            int idx = 0;
            buf[idx++] = '"';
            int prev = 0;
            while ((c = fgetc(fp)) != EOF) {
                buf[idx++] = (char)c;
                if (c == '"' && !prev) break;
                if (c == '\\' && !prev) prev = 1; else prev = 0;
                if (idx >= (int)sizeof(buf)-1) break;
            }
            buf[idx] = '\0';
            print_token("STRING_LITERAL", buf);
            continue;
        }

        // Character literal
        if (c == '\'') {
            char buf[16];
            int idx = 0;
            buf[idx++] = '\'';
            while ((c = fgetc(fp)) != EOF) {
                buf[idx++] = (char)c;
                if (c == '\'' ) break;
                if (idx >= (int)sizeof(buf)-1) break;
            }
            buf[idx] = '\0';
            print_token("CHAR_LITERAL", buf);
            continue;
        }

        // Comments or division operator
        if (c == '/') {
            int d = fgetc(fp);
            if (d == '/') {
                // single-line comment
                char buf[1024];
                int idx = 0;
                buf[idx++] = '/'; buf[idx++] = '/';
                while ((c = fgetc(fp)) != EOF && c != '\n') {
                    if (idx < (int)sizeof(buf)-1) buf[idx++] = (char)c;
                }
                buf[idx] = '\0';
                print_token("COMMENT", buf);
                continue;
            } else if (d == '*') {
                // block comment
                char buf[4096];
                int idx = 0;
                buf[idx++] = '/'; buf[idx++] = '*';
                int prev = 0;
                while ((c = fgetc(fp)) != EOF) {
                    if (idx < (int)sizeof(buf)-1) buf[idx++] = (char)c;
                    if (prev == '*' && c == '/') break;
                    prev = c;
                    if (idx >= (int)sizeof(buf)-2) break;
                }
                buf[idx] = '\0';
                print_token("COMMENT", buf);
                continue;
            } else {
                if (d != EOF) ungetc(d, fp);
                char s[2] = {'/', '\0'};
                print_token("OPERATOR", s);
                continue;
            }
        }

        // Two-character operators
        int next = fgetc(fp);
        if (next != EOF) {
            char two[3] = {(char)c, (char)next, '\0'};
            int matched = 1;
            if (strcmp(two, "==") == 0 || strcmp(two, "!=") == 0 || strcmp(two, "<=") == 0 || strcmp(two, ">=") == 0 ||
                strcmp(two, "++") == 0 || strcmp(two, "--") == 0 || strcmp(two, "+=") == 0 || strcmp(two, "-=") == 0 ||
                strcmp(two, "*=") == 0 || strcmp(two, "/=") == 0 || strcmp(two, "&&") == 0 || strcmp(two, "||") == 0 ||
                strcmp(two, "<<") == 0 || strcmp(two, ">>") == 0) {
                print_token("OPERATOR", two);
            } else {
                matched = 0;
            }
            if (!matched) {
                ungetc(next, fp);
                // single-character tokens: operators or separators
                char s[4] = {(char)c, '\0', '\0', '\0'};
                switch (c) {
                    case '+': case '-': case '*': case '%': case '=': case '<': case '>': case '!': case '&': case '|': case '^': case '~':
                        s[0] = (char)c;
                        print_token("OPERATOR", s);
                        break;
                    case '(': case ')': case '{': case '}': case '[': case ']': case ';': case ',': case ':': case '?':
                        s[0] = (char)c;
                        print_token("SEPARATOR", s);
                        break;
                    default:
                        s[0] = (char)c;
                        print_token("UNKNOWN", s);
                        break;
                }
            }
            continue;
        } else {
            // EOF after single char
            char s[2] = {(char)c, '\0'};
            print_token("UNKNOWN", s);
            continue;
        }
    }

    if (fp != stdin) fclose(fp);
    return 0;
}
