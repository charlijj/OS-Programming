#include<sys/shm.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <stdlib.h>

#include"shared_mem.h"

int main () {

    void *shared_mem;
    int shmid;

    shmid = shmget((key_t) KEY, 2 * BUFF_SIZE, 0666);
    shared_mem = shmat(shmid, NULL, 0);
    printf("\n\n\nReceiver --------------------------------\n\nThis is the sent message:\n\n%s", (char *)shared_mem);

    return 0;
}
