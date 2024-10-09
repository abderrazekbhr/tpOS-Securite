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
} FREE_MEMORY_BLOCK;

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

}
int main(){
    // Validating first step
    size_t size = 30;
    printf("magic_number : %ld\n", sizeof(MAGIC_NUMBER));
    printf("header : %ld\n", sizeof(HEADER));
    void* memoryAlloc = malloc_3is(size);
    void* end = sbrk(0);
    printf("init : %p\n", memoryAlloc);
    printf("end : %p\n", end);
    return 0;
}