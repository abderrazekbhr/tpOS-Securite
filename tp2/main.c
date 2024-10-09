#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAGIC_NUMBER 0x0123456789ABCDEFL

typedef struct HEADER_TAG {
    struct HEADER_TAG * ptr_next; /* pointe sur le prochain bloc libre */
    size_t bloc_size; /* taille du memory bloc en octets*/
    long magic_number; /* 0x0123456789ABCDEFL */
} HEADER;

typedef struct FREE_MEMORY_BLOCK{
    struct FREE_MEMORY_BLOCK *ptr_next;
    HEADER *address;
} Free_memory_block;
Free_memory_block *free_head = NULL;

void* malloc_3is(size_t memorySize){
    size_t total_size = sizeof(HEADER) + memorySize +  sizeof(MAGIC_NUMBER);
    void *init = sbrk(0);
    
    void *new_block = sbrk(total_size);
    if (new_block == (void*)-1){
        return NULL;
    }
    void *end = sbrk(0);
    if (init == end){
        return NULL;
    }
    return new_block;
}

void free_3is(void* ptr){
    Free_memory_block *block;
    
    block->ptr_next = NULL;
    block->address = ptr;
    if (free_head == NULL){
        free_head = block;
        return;
    } 
    Free_memory_block *temp = free_head;
    while (temp->ptr_next){
        temp = temp->ptr_next;
    }
    temp->ptr_next = block;
}

void displayFreeMemoryBlock()
{
    Free_memory_block *temp = free_head;
    while (temp != NULL)
    {
        sleep(1);
        printf("address : %p\n", (void *)temp->address);
        temp = temp->ptr_next;
    }
}

int main(){
    // Validating first step
    size_t size = 30;
    printf("magic_number : %ld\n", sizeof(MAGIC_NUMBER));
    printf("header : %ld\n", sizeof(HEADER));
    void* memory_alloc0 = malloc_3is(size);
    void* end = sbrk(0);
    printf("init : %p\n", memory_alloc0);
    printf("end : %p\n", end);
    void *memory_alloc1 = malloc_3is(size-5);
    end = sbrk(0);
    printf("init : %p\n", memory_alloc1);
    printf("end : %p\n", end);
    void *memory_alloc2 = malloc_3is(size-10);
    end = sbrk(0);
    printf("init : %p\n", memory_alloc2);
    printf("end : %p\n", end);
    void *memory_alloc3 = malloc_3is(size+5);
    end = sbrk(0);
    printf("init : %p\n", memory_alloc3);
    printf("end : %p\n", end);

    // Validating second step
    free_3is(memory_alloc0);
    free_3is(memory_alloc2);
    displayFreeMemoryBlock();

    return 0;
}