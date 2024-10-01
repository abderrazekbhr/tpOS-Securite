#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main()
{
    struct stat fileStat;
    // Open the file "test.txt" for reading and writing
    int file = open("test.txt", O_RDONLY | O_RDWR);
    // Check if file opened successfully
    if (file == -1)
    {
        perror("Error opening file");
        return 1;
    }

    // Get the file stat
    int excStat = stat("test.txt", &fileStat);
    // Check if stat was successful
    if (excStat == -1)
    {
        perror("Error getting file statistics");
        return 1;
    }

    // Print the size of the file
    printf("Size of file: %ld bytes\n", fileStat.st_size);

    // Map the file into memory
    char *mmaped = (char *)mmap(NULL, fileStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
    // Check if mmap was successful
    if (mmaped == MAP_FAILED)
    {
        perror("Error mapping file to memory");
        return 1;
    }

    // Reverse the contents of the mapped memory
    for (int i = 0; i < fileStat.st_size / 2; i++)
    {
        char temp = mmaped[i];
        mmaped[i] = mmaped[fileStat.st_size - i - 1];
        mmaped[fileStat.st_size - i - 1] = temp;
    }

    // Unmap the memory and close the file
    munmap(mmaped, fileStat.st_size);
    // Close the file
    close(file);
    // Successful execution
    return 0;
}
