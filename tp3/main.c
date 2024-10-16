#include <stdio.h>
#include "./ressources/tosfs.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    const char *file_path = "./ressources/test_tosfs_files";
    struct tosfs_superblock *superblock = NULL;
    struct tosfs_inode *inode_table = NULL;

    int fd = open(file_path, O_RDWR);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    size_t size = 32 * TOSFS_BLOCK_SIZE;

    void *fsdata = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (fsdata == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }
    superblock = (struct tosfs_superblock *)fsdata;

    // ---------------- start display super blocks info ----------------
    printf("--------------------- Display super blocks info : --------------------- \n");
    printf("Magic number = " PRINTF_BINARY_PATTERN_INT32 "\n", PRINTF_BYTE_TO_BINARY_INT32(superblock->magic));
    printf("Block bitmap = " PRINTF_BINARY_PATTERN_INT8 "\n", PRINTF_BYTE_TO_BINARY_INT8(superblock->block_bitmap));
    printf("Inode bitmap = " PRINTF_BINARY_PATTERN_INT8 "\n", PRINTF_BYTE_TO_BINARY_INT8(superblock->inode_bitmap));
    printf("Block size = %d\n", superblock->block_size);
    printf("Blocks = %d\n", superblock->blocks);
    printf("Inodes = %d\n", superblock->inodes);
    printf("Root inode = %d\n", superblock->root_inode);
    // ---------------- end display super blocks info ----------------

    inode_table = (struct tosfs_inode *)(fsdata + TOSFS_BLOCK_SIZE);

    for (int i = 0; i < superblock->inodes + 1; i++)
    {
        struct tosfs_inode *inode = &inode_table[i];
        // ---------------- start display inode info ----------------
        printf("--------------------- Display inode %d info : --------------------- \n", i);
        printf("Inode = %d\n", inode->inode);
        printf("Block number = " PRINTF_BINARY_PATTERN_INT8 "\n", PRINTF_BYTE_TO_BINARY_INT8(inode->block_no));
        printf("User ID = %d\n", inode->uid);
        printf("Group ID = %d\n", inode->gid);
        printf("Mode = %d\n", inode->mode);
        printf("Permission = " PRINTF_BINARY_PATTERN_INT16 "\n", PRINTF_BYTE_TO_BINARY_INT16(inode->perm));
        printf("Size = %d\n", inode->size);
        printf("Link = %d\n", inode->nlink);
        // ---------------- end display inode info ----------------
    }

    munmap(fsdata, size);
    close(fd);
    return 0;
}