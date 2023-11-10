#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    pid_t p1;
    pid_t p2;

    p1 = fork();
    p2 = fork();

    if (p1 == 0) {
        printf("I am a Child and my ID is: %d\n", getpid());
    }
    else if (p2 == 0) {
        printf("I am a Child and my ID is: %d\n", getpid());
    } else {
        wait(NULL);
        printf("I am Parent and my ID is: %d\n", getpid());
    }

    return 0;
}