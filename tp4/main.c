#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include "resources/segdef.h" // Contient les définitions des constantes et la structure SEGMENT

// Déclaration du segment mémoire
segment *shmseg;
int shmid, semid;

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

    // Attacher le segment de mémoire partagée
    shmseg = (segment *)shmat(shmid, NULL, 0);
    if (shmseg == (void *)-1) {
        perror("Erreur shmat");
        exit(1);
    }

    // Initialiser le générateur de nombres aléatoires
    init_rand();
}

void process_requests(int num_requests) {
    for (int i = 0; i < num_requests; i++) {
        // 1. Acquérir le sémaphore seg_dispo pour accéder à la mémoire partagée
        acq_sem(semid, seg_dispo);

        // 2. Initialiser le segment : PID, numéro de requête, tableau de valeurs aléatoires
        shmseg->pid = getpid();
        shmseg->req = i + 1;
        long local_sum = 0;

        for (int j = 0; j < maxval; j++) {
            shmseg->tab[j] = getrand();  // Générer un nombre aléatoire
            local_sum += shmseg->tab[j];
        }
        long local_result = local_sum / maxval;
        shmseg->result = 0;

        // 3. Signaler au serveur que le segment est initialisé
        lib_sem(semid, seg_init);

        // 4. Attendre que le serveur signale que le résultat est prêt
        acq_sem(semid, res_ok);

        // 5. Comparer les résultats (local et serveur)
        printf("Requête %d - PID %d\n", shmseg->req, shmseg->pid);
        printf("Résultat local: %ld, Résultat serveur: %ld\n", local_result, shmseg->result);

        // 6. Libérer les sémaphores seg_init et seg_dispo
        lib_sem(semid, seg_init);
        lib_sem(semid, seg_dispo);
    }
<<<<<<< HEAD:tp4/main.c

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
=======
}
void cleanup() {
    // Détacher la mémoire partagée
    if (shmdt(shmseg) == -1) {
        perror("Erreur shmdt");
>>>>>>> 3649ef15dd251df6207f97bf15ab07d1ace0aab8:tp4/other.c
        exit(1);
    }
}
int main() {
    int num_requests = 10;  // Nombre de requêtes à effectuer

    // 1. Initialisation
    init();

    // 2. Traiter les requêtes
    process_requests(num_requests);

    // 3. Nettoyer et détacher la mémoire
    cleanup();

    return 0;
}
