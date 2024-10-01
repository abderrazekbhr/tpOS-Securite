#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
const int data = 2;
int bss;
char *str = "Hello World!";
void main_function()
{
}
void libC_function()
{
    printf("libC_function\n");
}

int main()
{
    printf("addresse de data : %p\n", &data);
    printf("addresse de bss : %p\n", &bss);
    printf("addresse de str : %p\n", &str);

    int *heap = (int *)malloc(sizeof(int));
    if (heap == NULL)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        return 1;
    }
    heap[0] = 3;
    printf("addresse de heap : %p\n", &heap);
    free(heap);
    int stack = 1;

    printf("addresse de stack : %p\n", &stack);
    printf("addresse de main_function : %p\n", (void *)&main_function);
    printf("addresse de libC_function : %p\n", (void *)&libC_function);

    // affichager carte memoir du processus
    char pid[10];

    snprintf(pid, sizeof(pid), "%d", getpid());
    pid_t fils = fork();
    if (fils == -1)
    {
        perror("fork");
        return 1;
    }
    if (fils == 0)
    {
        char *mmaped = (char *)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (mmaped == MAP_FAILED)
        {
            perror("mmap");
            return 1;
        }
        printf("addresse de mmaped: %p\n", (void *)&mmaped);
        printf("on est dans le processus %s\n", pid);

        int exc = execlp("pmap", "pmap", "-X", pid, NULL);
        if (exc == -1)
        {
            perror("execlp");
            return 1;
        }
        munmap(mmaped, 4096);
    }
    else
    {
        wait(NULL);
    }

    return 0;
}