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

// Déclaration de la structure sembuf pour manipuler les sémaphores
struct sembuf sop;

void init() {
    // Récupérer les identifiants de la mémoire partagée et des sémaphores
    semid = semget(cle, 3, 0666); // 3 sémaphores
    if (semid == -1) {
        perror("Erreur semget");
        exit(1);
    }

    shmid = shmget(cle, segsize, 0666);
    if (shmid == -1) {
        perror("Erreur shmget");
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
}
void cleanup() {
    // Détacher la mémoire partagée
    if (shmdt(shmseg) == -1) {
        perror("Erreur shmdt");
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
