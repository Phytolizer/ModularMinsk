#include <sys/wait.h>
#include <unistd.h>

#define SOURCES "src/minsk.c", "src/analysis/syntax/kind.c"

int main(void) {
    pid_t cpid = fork();
    if (cpid == 0) {
        execlp("gcc", "gcc", "-Wall", "-Wextra", "-Wpedantic", "-Wmissing-prototypes", "-I",
               "include", SOURCES, "-o", "minsk", NULL);
    }
    int ret;
    waitpid(cpid, &ret, 0);
    return ret;
}
