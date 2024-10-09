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

HEADER *free_head = NULL;

void *malloc_3is(size_t memory_size)
{
    HEADER *current = NULL;
    size_t total_size = sizeof(HEADER) + memory_size + sizeof(MAGIC_NUMBER);

    void *new_block = sbrk(total_size);
    if (new_block == (void *)-1)
    {
        return NULL;
    }
    current = (HEADER *)new_block;
    current->bloc_size = memory_size;
    current->ptr_next = NULL;
    current->magic_number = MAGIC_NUMBER;
    int offset = sizeof(HEADER) + current->bloc_size;
    long *magic_number = (long *)(current + offset);
    *magic_number = MAGIC_NUMBER;
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
    printf("------------------------ Start display free memory block ------------------------\n");
    HEADER *temp = free_head;
    while (temp != NULL)
    {
        printf("address : %p\n", (void *)temp);
        temp = temp->ptr_next;
    }
    printf("------------------------ End display free memory block ------------------------\n");
}
void *reuseMemoryBlock(size_t memory_size)
{
    HEADER *temp = free_head;
    if (temp == NULL)
    {
        return NULL;
    }
    else if (temp->bloc_size == memory_size)
    {
        free_head = temp->ptr_next;
        return temp;
    }
    while (temp->ptr_next != NULL)
    {
        if (temp->ptr_next->bloc_size == memory_size)
        {
            HEADER *temp2 = temp->ptr_next;
            temp->ptr_next = temp2->ptr_next;
            return temp2;
        }
        temp = temp->ptr_next;
    }
    return NULL;
}

int check_debo(HEADER *head)
{
    int offset = sizeof(HEADER) + head->bloc_size;
    long *magic_number = (long *)(head + offset);
    printf("start magic number=%ld\n", head->magic_number);
    printf("end magic number=%ld\n", *magic_number);
    if (head->magic_number != *magic_number)
    {
        return 0;
    }
    return 1;
}

int main()
{
    // Validating malloc_is3 function
    printf("****************************************** Validating malloc_is3 function ******************************************\n");
    size_t size = 30;
    printf("magic_number : %ld\n", sizeof(MAGIC_NUMBER));
    printf("header : %ld\n", sizeof(HEADER));
    HEADER *memory_alloc0 = malloc_3is(size);
    HEADER *end = sbrk(0);
    printf("init 0: %p\n", memory_alloc0);
    printf("end : %p\n", end);
    HEADER *memory_alloc1 = malloc_3is(size - 5);
    end = sbrk(0);
    printf("init 1: %p\n", memory_alloc1);
    printf("end : %p\n", end);
    HEADER *memory_alloc2 = malloc_3is(size - 10);
    end = sbrk(0);
    printf("init 2: %p\n", memory_alloc2);
    printf("end : %p\n", end);
    HEADER *memory_alloc3 = malloc_3is(size + 5);
    end = sbrk(0);
    printf("init 3: %p\n", memory_alloc3);
    printf("end : %p\n", end);

    // Validating free_3is function
    printf("****************************************** Validating free_3is function ******************************************\n");
    free_3is(memory_alloc0);
    free_3is(memory_alloc2);
    displayFreeMemoryBlock();

    // Validating check_debo function
    printf("****************************************** Validating check_debo function ******************************************\n");
    printf("//////////////// First usecase : ////////////////\n");
    int check_debo_test_1 = check_debo(memory_alloc3);
    if (check_debo_test_1)
    {
        printf("check debo is valid\n");
    }
    else
    {
        printf("check debo is not valid\n");
    }

    memory_alloc1->magic_number = 0x123456789;
    printf("//////////////// Second usecase : ////////////////\n");
    int check_debo_test_2 = check_debo(memory_alloc1);

    if (check_debo_test_2)
    {
        printf("check debo is valid\n");
    }
    else
    {
        printf("check debo is not valid\n");
    }

    // Validating reuseMemoryBlock function fitted with block size
    printf("****************************************** Validating reuseMemoryBlock function ******************************************\n");
    HEADER *memory_alloc4 = reuseMemoryBlock(size);
    printf("Reuse memory with 30 as size block: %p | size=%ld\n", memory_alloc4, memory_alloc4->bloc_size);
    displayFreeMemoryBlock();

    return 0;
}