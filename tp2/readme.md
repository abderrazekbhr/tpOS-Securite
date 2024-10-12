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
## Functions

### Memory Management Functions

- **`void *malloc_3is(size_t memory_size)`**
  - Allocates a block of memory of the specified size.
  - Returns a pointer to the allocated memory or `NULL` if allocation fails.

- **`void free_3is(HEADER *ptr)`**
  - Frees a previously allocated memory block and adds it back to the free list.

- **`HEADER *reuse_memory_block(size_t memory_size)`**
  - Reuses a free memory block of the specified size if available.

- **`void *cut_memory_by_size(size_t memory_size)`**
  - Cuts a memory block to fit the specified size and returns a pointer to the new block.

### Utility Functions

- **`void display_free_memory_block()`**
  - Displays the current state of the free memory blocks.

- **`int check_debo(HEADER *head)`**
  - Checks if a memory block's integrity is valid using the magic number.

- **`void merge_adjacent_blocks()`**
  - Merges adjacent free blocks to reduce fragmentation.

- **`void sort_free_list_by_address()`**
  - Sorts the list of free blocks by their memory addresses.

### Random Magic Number Generator

- **`long generate_random_magic_number()`**
  - Generates a random magic number, though not used in the current implementation.
