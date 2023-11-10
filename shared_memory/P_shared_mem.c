#include<sys/shm.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <stdlib.h>

#include"shared_mem.h"

int main () {

    void *shared_mem;
    int shmid;
    char *buff = (char *)calloc(BUFF_SIZE+1, sizeof(char));

    shmid = shmget((key_t) KEY, 2 * BUFF_SIZE, 0666 | IPC_CREAT);
    shared_mem = shmat(shmid, NULL, 0);
    printf("\n\nSender --------------------------------\n\nEnter message to send: \n");
    read(0, buff, BUFF_SIZE);
    strcpy(shared_mem, buff);
    free(buff);
    return 0;
}
