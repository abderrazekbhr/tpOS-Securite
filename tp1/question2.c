#include<stdio.h>
#include<stdlib.h>
#include <sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
int main(){
    struct stat fileStat;
    int file=open("test.txt",O_RDONLY);
    if(file==-1){
        perror("Error");
        exit(1);
    }
    int excStat=stat("test.txt",&fileStat);
    if(excStat==-1){
        perror("stat");
        exit(1);
    }
    char *mmaped = (char *)mmap(NULL, excStat, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
    if(mmaped==MAP_FAILED){
        perror("mmap");
        exit(1);
    }
    printf("addresse de mmaped: %p\n", (void *)&mmaped);
    return 0;
}
