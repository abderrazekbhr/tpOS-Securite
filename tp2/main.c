

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE_LONG sizeof(long)
#define SIZE_HEADER sizeof(HEADER)
#define MAGIC_NUMBER 0x0123456789ABCDEFL
#define MAX_SIZE 1000
typedef struct HEADER_TAG
{
    struct HEADER_TAG *ptr_next; /* pointe sur le prochain bloc libre */
    size_t block_size;           /* taille du memory bloc en octets*/
    long magic_number;           /* 0x0123456789ABCDEFL */
} HEADER;

HEADER *free_head = NULL;

void *malloc_3is(size_t memory_size)
{
    HEADER *current = NULL;
    size_t total_size = SIZE_HEADER + memory_size + sizeof(MAGIC_NUMBER);

    void *new_block = sbrk(total_size);
    if (new_block == (void *)-1)
    {
        return NULL;
    }
    current = (HEADER *)new_block;
    current->block_size = memory_size;
    current->ptr_next = NULL;
    current->magic_number = MAGIC_NUMBER;
    int offset = SIZE_HEADER + current->block_size;
    long *magic_number = (long *)(current + offset);
    *magic_number = MAGIC_NUMBER;
    return new_block;
}

void display_free_memory_block()
{
    printf("------------------------ Start display free memory block ------------------------\n");
    HEADER *temp = free_head;
    while (temp != NULL)
    {
        printf("address : %p | size = %ld \n", (void *)temp, temp->block_size);
        temp = temp->ptr_next;
    }
    printf("------------------------ End display free memory block ------------------------\n");
}

long generate_random_magic_number()
{
    return rand();
}

void sort_free_list_by_address()
{
    // If the list is empty or has only one element, return immediately.
    if (!free_head || !free_head->ptr_next)
        return;

    HEADER *sorted = NULL; // This will hold the sorted list.

    while (free_head)
    {
        HEADER *current = free_head; // Get the current node.
        free_head = free_head->ptr_next; // Move to the next node.

        // Insert current into the sorted list.
        if (!sorted || current < sorted) // If sorted is empty or current goes before sorted.
        {
            current->ptr_next = sorted; // Insert at the beginning.
            sorted = current; // Update sorted head.
        }
        else
        {
            // Find the correct position in the sorted list.
            HEADER *temp = sorted;

            // Traverse the sorted list to find the insertion point.
            while (temp->ptr_next && temp->ptr_next < current) 
            {
                temp = temp->ptr_next; // Move forward in the sorted list.
            }

            // Insert current at the found position.
            current->ptr_next = temp->ptr_next; // Point current to the next node in sorted.
            temp->ptr_next = current; // Link temp to current.
        }
    }

    free_head = sorted; // Update free_head to point to the sorted list.
}


void merge_adjacent_blocks()
{
    if (!free_head)
    {
        return;
    }

    HEADER *current_block = free_head;

    while (current_block && current_block->ptr_next)
    {
        HEADER *next_block = current_block->ptr_next;

        if ((char *)current_block + SIZE_HEADER + current_block->block_size + MAGIC_NUMBER == (char *)next_block)
        {
            current_block->block_size += next_block->block_size + SIZE_HEADER + MAGIC_NUMBER;
            current_block->ptr_next = next_block->ptr_next;
        }
        else
        {
            current_block = next_block;
        }
    }
}

HEADER *reuse_memory_block(size_t memory_size)
{
    HEADER *temp = free_head;
    if (temp == NULL)
    {
        return NULL;
    }
    else if (temp->block_size == memory_size)
    {
        free_head = temp->ptr_next;
        return temp;
    }
    while (temp->ptr_next != NULL)
    {
        if (temp->ptr_next->block_size == memory_size)
        {
            HEADER *temp2 = temp->ptr_next;
            temp->ptr_next = temp2->ptr_next;
            return temp2;
        }
        temp = temp->ptr_next;
    }
    return NULL;
}

void *cut_memory_by_size(size_t memory_size)
{
    HEADER *temp = free_head;
    HEADER *min_block = reuse_memory_block(memory_size);
    HEADER *prev = NULL;
    HEADER *prev_min = NULL;
    size_t min_size = MAX_SIZE;
    if (min_block != NULL)
    {
        return min_block;
    }
    min_block = reuse_memory_block(memory_size);

    // Find the smallest block that is large enough
    while (temp != NULL)
    {
        printf("------------------\n");
        if (temp->block_size >= memory_size + SIZE_HEADER + SIZE_LONG && temp->block_size < min_size)
        {
            min_block = temp;
            prev_min = prev;
            min_size = temp->block_size;
        }
        prev = temp;
        temp = temp->ptr_next;
    }
    printf("%p\n", min_block);
    // If no suitable block found, return NULL
    if (min_block == NULL)
    {
        return NULL;
    }

    // Create a new block for the remaining free memory
    HEADER *new_block = (HEADER *)(min_block + SIZE_HEADER + memory_size);

    long *magic_number = (long *)(new_block);
    *magic_number = MAGIC_NUMBER;

    // Correctly calculate the remaining block size
    new_block += SIZE_LONG;
    new_block->block_size = min_block->block_size - memory_size;

    new_block->ptr_next = min_block->ptr_next;
    new_block->magic_number = MAGIC_NUMBER;

    // Update the min_block to reflect the size of the allocated memory
    min_block->block_size = memory_size;

    // Update the free list to replace min_block with new_block
    if (prev_min == NULL)
    {
        free_head = new_block;
    }
    else
    {
        prev_min->ptr_next = new_block;
    }

    // Return the allocated memory (right after the HEADER)
    return min_block;
}

int check_debo(HEADER *head)
{
    if (head == NULL)
    {
        return 0;
    }
    int offset = SIZE_HEADER + head->block_size;
    long *magic_number = (long *)(head + offset);
    printf("start magic number=%ld\n", head->magic_number);
    printf("end magic number=%ld\n", *magic_number);
    if (head->magic_number != *magic_number)
    {
        return 0;
    }
    return 1;
}

void free_3is(HEADER *ptr)
{
    sort_free_list_by_address();
    merge_adjacent_blocks();
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

int main()
{
    // Validating malloc_3is function
    printf("****************************************** Validating malloc_3is function ******************************************\n");
    const size_t SIZE = 30;

    HEADER *memory_alloc0 = malloc_3is(SIZE * 3);
    HEADER *end = sbrk(0);
    printf("init 0: %p\n", memory_alloc0);
    printf("end : %p\n", end);

    HEADER *memory_alloc1 = malloc_3is(SIZE);
    end = sbrk(0);
    printf("init 1: %p\n", memory_alloc1);
    printf("end : %p\n", end);

    HEADER *memory_alloc2 = malloc_3is(SIZE - 10);
    end = sbrk(0);
    printf("init 2: %p\n", memory_alloc2);
    printf("end : %p\n", end);

    HEADER *memory_alloc3 = malloc_3is(SIZE + 5);
    end = sbrk(0);
    printf("init 3: %p\n", memory_alloc3);
    printf("end : %p\n", end);

    // Validating free_3is function
    printf("****************************************** Validating free_3is function ******************************************\n");
    free_3is(memory_alloc0);
    free_3is(memory_alloc2);
    display_free_memory_block();

    // Validating check_debo function
    printf("****************************************** Validating check_debo function ******************************************\n");
    printf("//////////////// First use case : ////////////////\n");
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
    printf("//////////////// Second use case : ////////////////\n");
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
    HEADER *memory_alloc4 = reuse_memory_block(SIZE);
    if (memory_alloc4 != NULL)
    {
        printf("Reuse memory with 30 as size block: %p | size = %ld\n", memory_alloc4, memory_alloc4->block_size);
    }
    else
    {
        printf("No block of 30 found for reuse.\n");
    }
    display_free_memory_block();

    free_3is(memory_alloc2);

    // Validating cut memory by size function
    printf("****************************************** Validating cut memory by size function ******************************************\n");
    HEADER *memory_alloc5 = cut_memory_by_size(12);
    printf("%p\n", memory_alloc5);
    display_free_memory_block();

    check_debo(memory_alloc5);
    printf("%p\n", memory_alloc5);
    if (memory_alloc5 != NULL)
    {
        printf("Cut memory by size with 12 as size block: %p | size = %ld\n", memory_alloc5, memory_alloc5->block_size);
        free_3is(memory_alloc5);
        display_free_memory_block();
    }
    else
    {
        printf("No block of 12 found for cut.\n");
    }

    return 0;
}
