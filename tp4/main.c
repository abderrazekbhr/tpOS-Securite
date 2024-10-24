#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "./ressources/segdef.h"
#include <time.h>
#include <stdlib.h>

int main()
{
    init_rand(); // Initialize random number generator

    // Print a random number for verification
    printf("%ld\n", (long)getrand());

    // Create semaphore with 3 semaphores in the set
    int creat_sem = semget(cle, 3, IPC_CREAT | 0666);
    if (creat_sem == -1)
    {
        perror("Failed to create semaphore");
        exit(1);
    }

    // Wait and acquire semaphore
    wait_sem(creat_sem, seg_dispo);
    acq_sem(creat_sem, seg_dispo);

    // Generate 10 random numbers
    int x[10];
    for (int i = 0; i < 10; i++)
    {
        x[i] = getrand();
    }

    // Get shared memory ID
    int shm_id = shmget(cle, sizeof(struct shmseg), IPC_CREAT | 0666);
    if (shm_id == -1)
    {
        perror("Failed to create shared memory");
        exit(1);
    }

    // Attach shared memory to our process
    struct shmseg *seg = (struct shmseg *)shmat(shm_id, NULL, 0);
    if (seg == (void *)-1)
    {
        perror("Failed to attach shared memory");
        exit(1);
    }

    // Set process ID and request
    seg->pid = getpid();
    seg->req = 1;

    // Calculate the average of the random numbers
    long result = 0;
    for (int i = 0; i < 10; i++)
    {
        result += x[i];
        seg->tab[i] = x[i]; // Store the numbers in shared memory
    }
    result /= 10; // Average

    seg->result = result; // Store result in shared memory


    // Release semaphore
    lib_sem(creat_sem, seg_dispo);

    return 0;
}