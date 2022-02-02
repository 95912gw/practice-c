#include <stdio.h>
#include <string.h>

int main(void) {
    char str[] = "   you need to    get some sleep.   ";
    char delim[] = " ";
    char *token;

    token = strtok(str, delim);
    while (token != NULL) {
        printf("next token is: %s: %d\n", token, strlen(token));
        token = strtok(NULL, delim);    }
}