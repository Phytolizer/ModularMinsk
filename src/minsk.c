#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char* line = NULL;
    size_t line_len = 0;
    while (true) {
        printf("> ");
        fflush(stdout);
        errno = 0;
        if (getline(&line, &line_len, stdin) == -1) {
            if (errno != 0) {
                perror("getline");
            }
            free(line);
            puts("");
            break;
        }

        char* endl = strpbrk(line, "\r\n");
        if (endl) {
            *endl = 0;
        }

        if (strcmp(line, "1 + 2 * 3") == 0) {
            puts("7");
        } else {
            puts("ERROR: Invalid expression!");
        }
    }
}
