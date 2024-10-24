#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "resources/segdef.h"

int shmid, semid;
char *buf;

int initialization(){
    shmid = shmget(cle, segsize, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        return 1;
    }
    printf("Shared memory ID: %d\n", shmid);
    semid = semget(cle, 3, IPC_CREAT | 0666);
    if (semid == -1)
    {
        perror("semget");
        return 1;
    }
    buf = shmat(shmid, 0, 0);
    if (buf == (char *)-1)
    {
        perror("shmat");
        return 1;
    }
    init_rand();
    return 0;
}

int main()
{
    initialization();
    long array[maxval], sum = 0;

    for (int i = 0; i < maxval; i++)
    {
        array[i] = getrand();
        sum += array[i];
    }
    sum = sum / maxval;
    

    acq_sem(semid, seg_dispo);
    segment *seg=(segment *)buf;
    seg->pid = getpid();
    seg->req = 1;
    seg->result = sum;
    for (int i = 0; i < maxval; i++)
    {
        seg->tab[i] = array[i];
    }
    lib_sem(semid, seg_init);
    return 0;
}