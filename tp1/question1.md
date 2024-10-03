# Process Memory Map Viewer

This program demonstrates various aspects of memory management in C, including stack, heap, data, and BSS segment manipulations, along with memory mapping and process creation using `fork()`. It displays the memory layout of the current process using `pmap`.

## Functionality Overview

### 1. **Global Variables and Segments**
   - **Global variables**:
     - `data`: A constant integer stored in the **data segment**.
     - `bss`: An uninitialized variable stored in the **BSS segment**.
     - `str`: A pointer to a string stored in the **data segment**.
     ```c
     const int data = 2;
     int bss;
     char *str = "Hello World!";
     ```

### 2. **Printing Memory Addresses**
   - The program prints the memory addresses of the global variables (`data`, `bss`, `str`), a heap-allocated variable, a stack variable, and two functions (`main_function` and `libC_function`):
     ```c
     printf("Address of data: %p\n", &data);
     printf("Address of bss: %p\n", &bss);
     printf("Address of str: %p\n", &str);
     ```

### 3. **Dynamic Memory Allocation (Heap)**
   - Memory is dynamically allocated on the **heap** using `malloc()` and freed using `free()`:
     ```c
     int *heap = (int *)malloc(sizeof(int));
     heap[0] = 3;
     printf("Address of heap: %p\n", &heap);
     free(heap);
     ```

### 4. **Printing Function Addresses**
   - The memory addresses of the functions `main_function` and `libC_function` are printed. These functions are stored in the **text/code segment**:
     ```c
     printf("Address of main_function: %p\n", (void *)&main_function);
     printf("Address of libC_function: %p\n", (void *)&libC_function);
     ```

### 5. **Memory Mapping with `mmap()`**
   - The program creates a memory-mapped region using `mmap()` in the child process:
     ```c
     char *mmaped = (char *)mmap(NULL, sizeof(char *), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
     if (mmaped == MAP_FAILED) {
         perror("mmap");
         return 1;
     }
     printf("Address of mmaped: %p\n", (void *)&mmaped);
     ```

### 6. **Process Creation and `pmap`**
   - The program creates a child process using `fork()`. The child process replaces itself with the `pmap` command, displaying the memory map of the current process:
     ```c
     pid_t child = fork();
     if (child == 0) {
         int exec_status = execlp("pmap", "pmap", "-X", pid, NULL);
         if (exec_status == -1) {
             perror("execlp");
             return 1;
         }
     }
     ```
   - The parent process waits for the child to complete using `wait()`.

### 7. **Using `pmap`**
   - The child process uses `execlp()` to execute the `pmap` command, which outputs the memory layout of the current process:
     ```bash
     pmap -X <pid>
     ```

## Error Handling
- The program checks for errors at key points:
  - **File operations**: `mmap()` ensures successful memory mapping.
  - **Process creation**: `fork()` is checked for errors.
  - **Execution replacement**: `execlp()` is used to replace the process image, and its success is checked.
  - If an error occurs, `perror()` is used to display an appropriate error message.

## System Calls and Libraries Used
- `malloc()` - Allocates memory dynamically on the heap.
- `free()` - Frees dynamically allocated memory.
- `mmap()` - Maps memory to the process's address space.
- `fork()` - Creates a new process (child).
- `execlp()` - Replaces the process image with a new program.
- `wait()` - Waits for the child process to terminate.

## Example Output

The program prints memory addresses of various variables and regions:
