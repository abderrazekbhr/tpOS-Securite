#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAGIC_NUMBER 0x0123456789ABCDEFL

typedef struct HEADER_TAG
{
    struct HEADER_TAG *ptr_next; /* pointe sur le prochain bloc libre */
    size_t bloc_size;            /* taille du memory bloc en octets*/
    long magic_number;           /* 0x0123456789ABCDEFL */
} HEADER;

// typedef struct FREE_MEMORY_BLOCK
// {
//     struct FREE_MEMORY_BLOCK *ptr_next;
//     HEADER *address;
// } Free_memory_block;
HEADER *free_head = NULL;

void *malloc_3is(size_t memorySize)
{
    HEADER *current = NULL;
    size_t total_size = sizeof(HEADER) + memorySize + sizeof(MAGIC_NUMBER);

    void *new_block = sbrk(total_size);
    if (new_block == (void *)-1)
    {
        return NULL;
    }
    current = (HEADER *)new_block;
    current->bloc_size = total_size;
    current->ptr_next = NULL;
    current->magic_number = MAGIC_NUMBER;
    return new_block;
}

void free_3is(HEADER *ptr)
{

    if (free_head == NULL)
    {
        free_head = ptr;
        ptr->ptr_next = NULL;
        return;
    }
    HEADER *temp = free_head;
    while (temp->ptr_next)
    {
        temp = temp->ptr_next;
    }
    temp->ptr_next = ptr;
    ptr->ptr_next = NULL;
}

void displayFreeMemoryBlock()
{
    HEADER *temp = free_head;
    while (temp->ptr_next != NULL)
    {
        sleep(1);
        printf("address : %p\n", temp);
        printf("address : %p\n", (void *)temp->ptr_next);
        temp = temp->ptr_next;
    }
}

int check_debo(HEADER *head)
{
    int offset = sizeof(HEADER) + head->bloc_size;
    long *magic_number = (long *)(head + offset);
    printf("%ld|%ld",head->magic_number,*magic_number);
    if (head->magic_number != *magic_number)
    {
        return 0;
    }
    return 1;
}

int main()
{
    // Validating first step
    size_t size = 30;
    printf("magic_number : %ld\n", sizeof(MAGIC_NUMBER));
    printf("header : %ld\n", sizeof(HEADER));
    void *memory_alloc0 = malloc_3is(size);
    void *end = sbrk(0);
    printf("init 0: %p\n", memory_alloc0);
    printf("end : %p\n", end);
    void *memory_alloc1 = malloc_3is(size - 5);
    end = sbrk(0);
    printf("init 1: %p\n", memory_alloc1);
    printf("end : %p\n", end);
    void *memory_alloc2 = malloc_3is(size - 10);
    end = sbrk(0);
    printf("init 2: %p\n", memory_alloc2);
    printf("end : %p\n", end);
    void *memory_alloc3 = malloc_3is(size + 5);
    end = sbrk(0);
    printf("init 3: %p\n", memory_alloc3);
    printf("end : %p\n", end);

    // Validating second step
    free_3is(memory_alloc0);
    free_3is(memory_alloc2);
    displayFreeMemoryBlock();

    // Validating third step
    int test = check_debo(memory_alloc3);
    if (test)
    {
        printf("check debo is valid\n");
    }
    else
    {
        printf("check debo is not valid\n");
    }

    return 0;
}