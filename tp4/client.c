#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "resources/segdef.h"

int shmid, semid_dispo, semid_init, semid_res;
char *buf;

// Step 1
int initialization(){
    // Step 1.1
    shmid = shmget(cle, segsize, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        return 1;
    }
    printf("Shared memory ID: %d\n", shmid);
    semid_dispo = semget(cle, seg_dispo, IPC_CREAT | 0666);
    if (semid_dispo == -1)
    {
        perror("semget");
        return 1;
    }
    semid_init = semget(cle, seg_init, IPC_CREAT | 0666);
    if (semid_init == -1)
    {
        perror("semget");
        return 1;
    }
    semid_res = semget(cle, res_ok, IPC_CREAT | 0666);
    if (semid_res == -1)
    {
        perror("semget");
        return 1;
    }
    // Step 1.2
    buf = shmat(shmid, 0, 0);
    if (buf == (char *)-1)
    {
        perror("shmat");
        return 1;
    }
    // Step 1.3
    init_rand();
    return 0;
}
void process_requests(int num_requests) {
    for (int i = 0; i < num_requests; i++){
        acq_sem(semid_dispo, seg_dispo);
        segment *seg=(segment *)buf;
        seg->pid = getpid();
        seg->req = i + 1;
        long local_sum = 0;
        for (int j = 0; j < maxval; j++)
        {
            seg->tab[j] = getrand();
            local_sum += seg->tab[j];
        }
        long local_result = local_sum / maxval;
        seg->result = 0;
        acq_sem(semid_init, seg_init);
        wait_sem(semid_res, res_ok);
        lib_sem(semid_init, seg_init);
        lib_sem(semid_dispo, seg_dispo);

        printf("Requête %d - PID %d\n", seg->req, seg->pid);
        //printf("Résultat local: %ld, Résultat serveur: %ld\n", local_result, seg->result);
        if(local_result == seg->result){
            printf("Les résultats sont identiques\n");
        }
        else{
            printf("Les résultats sont différents\n");
        }
    }
}
void cleanup() {
    if (shmdt(buf) == -1) {
        perror("Erreur shmdt");
        exit(1);
    }
}

int main()
{
    initialization();
    process_requests(5);
    cleanup();
    return 0;
}