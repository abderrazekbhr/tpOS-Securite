#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./ressources/segdef.h"
#include <sys/wait.h>
int shmid, semid_dispo, semid_init, semid_res;
char *buf;

// Step 1
int initialization()
{
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

// Step 2
void process_requests(int num_requests)
{
    for (int i = 0; i < num_requests; i++)
    {
        // Step 2.1
        acq_sem(semid_dispo, seg_dispo);
        // Stre 2.2
        segment *seg = (segment *)buf;
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
        // Step 2.3
        acq_sem(semid_init, seg_init);
        // Step 2.4
        wait_sem(semid_res, res_ok);
        // Step 2.5
        long server_result = seg->result;
        
        
        // printf("Requête %d - PID %d\n", seg->req, seg->pid);
        //printf("req: %d => pid: %d | ppid: %d , Résultat local: %ld, Résultat serveur: %ld\n", seg->req, getpid(), getppid(), local_result, seg->result);
        // Step 2.7
        if (local_result == server_result)
        {
            printf("Les résultats sont identiques\n");
        }
        else
        {
            printf("Les résultats sont différents\n");
        }
        lib_sem(semid_init, seg_init);
        // Step 2.6
        lib_sem(semid_dispo, seg_dispo);
    }
}
// Step 3
void cleanup()
{
    if (shmdt(buf) == -1)
    {
        perror("Erreur shmdt");
        exit(1);
    }
}

void calculate(int pid, int nb_process)
{

    for (int i = 0; i < nb_process; i++)
    {
        if (pid == getpid())
        {
            if (fork() == 0)
            {
                process_requests(5);
            }
            wait(NULL);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Usage: %s <nb_process>\n", argv[0]);
        return 1;
    }
    initialization();
    calculate(getpid(), atoi(argv[1]));
    cleanup();
    return 0;
}