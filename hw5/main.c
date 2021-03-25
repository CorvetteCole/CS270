#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int childStatus;
    if (fork() == 0) {
        char *args[2] = {"/bin/ls", "/usr/local/bin"};
        execve(args[0], args, NULL);
    } else {
        wait(&childStatus);
        printf("Child's exit status: %d\n", childStatus);
    }
    return 0;
}

