# Memory-Mapped File Reverser

This program demonstrates how to use memory-mapped files in C to read and manipulate the contents of a file. Specifically, it opens a file, maps it to memory, reverses its contents, and saves the changes back to the file.

## Functionality Overview

### 1. **File Opening**
   - The program opens a file named `test.txt` for reading and writing using the `open()` system call with the flags `O_RDONLY | O_RDWR`:
     ```c
     int file = open("test.txt", O_RDONLY | O_RDWR);
     ```
   - If the file cannot be opened, it outputs an error message and exits the program:
     ```c
     if (file == -1) {
         perror("Error opening file");
         return 1;
     }
     ```

### 2. **Getting File Statistics**
   - The program retrieves the size of the file using the `stat()` system call, which stores the file's metadata in a `struct stat`:
     ```c
     struct stat fileStat;
     int excStat = stat("test.txt", &fileStat);
     ```
   - If `stat()` fails, an error message is displayed, and the program exits:
     ```c
     if (excStat == -1) {
         perror("Error getting file statistics");
         return 1;
     }
     ```

### 3. **Memory Mapping**
   - The program maps the file into memory using `mmap()`, allowing it to access the file's contents as if it were an array in memory:
     ```c
     char *mmaped = (char *)mmap(NULL, fileStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
     ```
   - If the memory mapping fails, an error is printed and the program exits:
     ```c
     if (mmaped == MAP_FAILED) {
         perror("Error mapping file to memory");
         return 1;
     }
     ```

### 4. **Reversing File Content**
   - Once the file is mapped into memory, the program reverses the content by swapping the characters at the beginning and end of the file, progressing towards the center:
     ```c
     for (int i = 0; i < fileStat.st_size / 2; i++) {
         char temp = mmaped[i];
         mmaped[i] = mmaped[fileStat.st_size - i - 1];
         mmaped[fileStat.st_size - i - 1] = temp;
     }
     ```

### 5. **Cleanup and Exit**
   - After reversing the contents, the program unmaps the memory using `munmap()`, closes the file, and exits successfully:
     ```c
     munmap(mmaped, fileStat.st_size);
     close(file);
     return 0;
     ```

## Error Handling
- The program checks for errors at every step:
  - **File opening**: Uses `open()` and checks if the file descriptor is valid.
  - **File statistics retrieval**: Uses `stat()` to ensure file metadata is accessible.
  - **Memory mapping**: Uses `mmap()` and ensures mapping is successful before proceeding.
  
  If any error occurs, an appropriate error message is printed using `perror()`, and the program exits with a non-zero status.

## System Calls and Libraries Used
- `open()` - Opens a file for reading and writing.
- `stat()` - Retrieves metadata of the file, including file size.
- `mmap()` - Maps the file into the virtual memory space for direct access.
- `munmap()` - Unmaps the memory-mapped file once operations are completed.
- `close()` - Closes the file descriptor.

## Example Output

When executed, the program outputs the size of the file and manipulates the contents. For example:
