#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t cpid = fork();
    if (cpid == 0) {
        execlp("gcc", "gcc", "-Wall", "-Wextra", "-Wpedantic", "-Wmissing-prototypes", "src/minsk.c",
               "-o", "minsk", NULL);
    }
    int ret;
    waitpid(cpid, &ret, 0);
    return ret;
}
