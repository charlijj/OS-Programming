#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {

    pid_t p;
    int file;

    p = fork();
    if (p == 0) {
        // create (or clear if it exists) joinus.txt
        file = open("joinus.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
        close(file);
    }
    else {
        wait(NULL);

        file = open("joinus.txt", O_WRONLY | O_APPEND);

        const int max_input_len = 255;
        char *buffer = (char *)calloc(max_input_len + 1, sizeof(char));

        printf("\n\nEnter input for joinus.txt: \n");
        read(0, buffer, (size_t)max_input_len);

        // write user input to joinus.txt
        write(file, buffer, strlen(buffer));
        close(file);

        printf("\n\nWrote: %s\n", buffer);
        free(buffer);
    }
    return 0;
}