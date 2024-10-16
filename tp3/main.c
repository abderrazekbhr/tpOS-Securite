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
    printf("Magic number = %d\n", superblock->magic);
    printf("Block bitmap = %d\n", superblock->block_bitmap);
    printf("Inode bitmap = %d\n", superblock->inode_bitmap);
    printf("Block size = %d\n", superblock->block_size);
    printf("Blocks = %d\n", superblock->blocks);
    printf("Inodes = %d\n", superblock->inodes);
    printf("Root inode = %d\n", superblock->root_inode);
    // ---------------- end display super blocks info ----------------

    inode_table = (struct tosfs_inode *)(fsdata + TOSFS_BLOCK_SIZE);
    struct tosfs_inode *root_inode = &inode_table[TOSFS_ROOT_INODE];
    // ---------------- start display inode info ----------------
    printf("--------------------- Display inode info : --------------------- \n");
    printf("Inode = %d\n", root_inode->inode);
    printf("Block number = %d\n", root_inode->block_no);
    printf("User ID = %d\n", root_inode->uid);
    printf("Group ID = %d\n", root_inode->gid);
    printf("Mode = %d\n", root_inode->mode);
    printf("Permission = %d\n", root_inode->perm);
    printf("Size = %d\n", root_inode->size);
    printf("Link = %d\n", root_inode->nlink);
    // ---------------- end display inode info ----------------

    munmap(fsdata, size);
    close(fd);
    return 0;
}