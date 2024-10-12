# Custom Memory Allocation in C

This program implements a custom memory allocation system in C, mimicking the behavior of the standard `malloc` and `free` functions. It maintains a linked list of free memory blocks, allowing for dynamic memory allocation and deallocation with additional features such as block reuse and memory integrity checks.

## Features

- **Dynamic Memory Allocation**: Allocates memory of specified sizes using the `malloc_3is` function.
- **Memory Freeing**: Frees allocated memory blocks with the `free_3is` function, and maintains a list of free blocks for reuse.
- **Block Merging**: Merges adjacent free blocks to reduce fragmentation.
- **Magic Number Check**: Ensures memory integrity with a magic number that verifies the integrity of allocated blocks.
- **Memory Sorting**: Sorts free memory blocks by address for efficient management.
- **Cutting Memory**: Allows for cutting memory blocks of specified sizes.

## Data Structure

### Header Structure

The `HEADER` struct represents a memory block's header, which includes:

- `ptr_next`: Pointer to the next free block.
- `block_size`: Size of the allocated block.
- `magic_number`: A constant value used to verify the integrity of the memory block.

```c
typedef struct HEADER_TAG
{
    struct HEADER_TAG *ptr_next; /* Pointer to the next free block */
    size_t block_size;           /* Size of the memory block in bytes */
    long magic_number;           /* Magic number for integrity check */
} HEADER;
```