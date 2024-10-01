#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
int main()
{
    struct stat fileStat;
    int file = open("test.txt", O_RDONLY | O_RDWR);
    if (file == -1)
    {
        perror("Error");
        exit(1);
    }
    int excStat = stat("test.txt", &fileStat);
    if (excStat == -1)
    {
        perror("stat");
        exit(1);
    }
    printf("size of file: %ld\n", fileStat.st_size);
    char *mmaped = (char *)mmap(NULL, fileStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
    if (mmaped == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    for (int i = 0; i < fileStat.st_size / 2; i++)
    {
        char temp = mmaped[i];
        mmaped[i] = mmaped[fileStat.st_size - i - 1];
        mmaped[fileStat.st_size - i - 1] = temp;
    }
    munmap(mmaped, fileStat.st_size);
    close(file);
    return 0;
}
