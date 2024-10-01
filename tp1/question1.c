#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

// Global variables
const int data = 2;      // 'data' is a constant, which will be stored in the read-only data segment
int bss;                 // 'bss' is an uninitialized global variable, which will be stored in the BSS segment
char *str = "Hello World!"; // 'str' is a pointer to a string, which will be stored in the data segment

// Function prototypes
void main_function()
{
    // This is a dummy function, not doing anything
}

void libC_function()
{
    // This function prints a message to the console
    printf("libC_function\n");
}

int main()
{
    // Print the memory address of the global variable 'data' (stored in the data segment)
    printf("Address of data: %p\n", &data);
    
    // Print the memory address of the global variable 'bss' (stored in the BSS segment)
    printf("Address of bss: %p\n", &bss);
    
    // Print the memory address of the string 'str' (stored in the data segment)
    printf("Address of str: %p\n", &str);

    // Dynamically allocate memory on the heap
    int *heap = (int *)malloc(sizeof(int));
    if (heap == NULL)
    {
        // If allocation fails, print an error message and exit
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }
    
    heap[0] = 3; // Assign a value to the allocated memory

    // Print the address of the dynamically allocated memory (on the heap)
    printf("Address of heap: %p\n", &heap);
    
    free(heap); // Free the allocated memory

    int stack = 1; // Variable stored on the stack

    // Print the address of the stack variable
    printf("Address of stack: %p\n", &stack);

    // Print the address of the function 'main_function' (stored in the text/code segment)
    printf("Address of main_function: %p\n", (void *)&main_function);

    // Print the address of the function 'libC_function' (stored in the text/code segment)
    printf("Address of libC_function: %p\n", (void *)&libC_function);

    // Display memory map of the process using 'pmap'

    char pid[10]; // Array to hold the process ID as a string
    snprintf(pid, sizeof(pid), "%d", getpid()); // Get the process ID and store it as a string

    // Fork a child process
    pid_t child = fork();
    if (child == -1)
    {
        // If fork fails, print an error message and exit
        perror("fork");
        return 1;
    }
    if (child == 0)
    {
        // Child process

        // Use mmap to allocate memory for 4096 bytes (4KB)
        char *mmaped = (char *)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (mmaped == MAP_FAILED)
        {
            // If mmap fails, print an error message and exit
            perror("mmap");
            return 1;
        }

        // Print the address of the mmaped memory region
        printf("Address of mmaped: %p\n", (void *)&mmaped);

        // Print a message indicating the child process is running
        printf("Running in process %s\n", pid);

        // Replace the current process with 'pmap' to display the memory map of the process
        int exec_status = execlp("pmap", "pmap", "-X", pid, NULL);
        if (exec_status == -1)
        {
            // If execlp fails, print an error message and exit
            perror("execlp");
            return 1;
        }

        // Unmap the memory region
        munmap(mmaped, 4096);
    }
    else
    {
        // Parent process waits for the child to complete
        wait(NULL);
    }

    return 0; // Exit the program
}
