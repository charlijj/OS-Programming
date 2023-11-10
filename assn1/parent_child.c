#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    pid_t p;
    p = fork();

    if (p == 0) {
        printf("\nI am Child and my ID is: %d\n", getpid());
        printf("\nMy Parent ID is: %d\n", getppid());
    }
    else {
        printf("\nI am Parent and my ID is: %d\n", getpid());
        wait(NULL);
        printf("\nMy Child has ID: %d\n", p);
    }

    return 0;
}