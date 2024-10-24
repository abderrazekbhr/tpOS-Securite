#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include "resources/segdef.h"

int shmid, semid;
segment *buf;

void initialize()
{

    if ((semid = semget(cle, 3, IPC_CREAT | 0666)) == -1)
    {
        perror("Erreur lors de la récupération de semid");
        exit(1);
    }
    if ((shmid = shmget(cle, segsize, 0666)) == -1)
    {
        perror("Erreur lors de la récupération de shmid");
        exit(1);
    }

    printf("ID de la mémoire partagée : %d\n", shmid);

    if ((buf = (segment *)shmat(shmid, NULL, 0)) == (void *)-1)
    {
        perror("Erreur lors de l'attachement du segment de mémoire partagée");
        exit(1);
    }

    init_rand();
}

int main()
{
    initialize();

    long array[maxval], sum = 0;

    for (int i = 0; i < maxval; i++)
    {
        array[i] = getrand();
        sum += array[i];
    }

    long local_avg = sum / maxval;

    acq_sem(semid, seg_dispo);
    segment *seg = buf;
    seg->pid = getpid();
    seg->req = 1;
    seg->result = local_avg;

    wait_sem(semid, res_ok);
    
    printf("Requête %d : résultat prêt\n", seg->req);
    // Vérification du résultat calculé par un autre processus
    if (seg->result == local_avg)
    {
        printf("Requête %d : succès, moyenne correcte (%ld)\n", seg->req, seg->result);
    }
    else
    {
        printf("Requête %d : échec, moyenne incorrecte (%ld au lieu de %ld)\n", seg->req, seg->result, local_avg);
    }

    // Libérer les sémaphores pour indiquer la fin du traitement
    lib_sem(semid, seg_init);
    lib_sem(semid, seg_dispo);

    // Détacher le segment de mémoire partagée
    if (shmdt(buf) == -1)
    {
        perror("Erreur lors du détachement de la mémoire partagée");
        exit(1);
    }

    return 0;
}
