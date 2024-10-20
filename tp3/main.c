#define FUSE_USE_VERSION 26
#define MAGIC 2272147200

#include <fuse_lowlevel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "./ressources/tosfs.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

// Contain list of all files in directory
struct dirbuf
{
	char *p;
	size_t size;
};

// Made it global to access memory from all functions
void *data_mmapped;

// Add a file to dirbuff
static void dirbuf_add(fuse_req_t req, struct dirbuf *b, const char *name, fuse_ino_t ino)
{
	struct stat stbuf;
	size_t oldsize = b->size;
	b->size += fuse_add_direntry(req, NULL, 0, name, NULL, 0);
	b->p = (char *)realloc(b->p, b->size);
	memset(&stbuf, 0, sizeof(stbuf));
	stbuf.st_ino = ino;
	fuse_add_direntry(req, b->p + oldsize, b->size - oldsize, name, &stbuf, b->size);
}

#define min(x, y) ((x) < (y) ? (x) : (y))

static int reply_buf_limited(fuse_req_t req, const char *buf, size_t bufsize, off_t off, size_t maxsize)
{
	if (off < bufsize)
		return fuse_reply_buf(req, buf + off, min(bufsize - off, maxsize));
	else
		return fuse_reply_buf(req, NULL, 0);
}

// Fill stat infos on file
static int file_system_stat(fuse_ino_t ino, struct stat *stbuf)
{
	stbuf->st_ino = ino;
	struct tosfs_superblock *superBlock = data_mmapped;
	// Check if inode exists
	if (ino <= superBlock->inodes)
	{
		struct tosfs_inode *inode = data_mmapped + superBlock->block_size + ino * sizeof(struct tosfs_inode);
		stbuf->st_mode = inode->mode;
		stbuf->st_nlink = inode->nlink;
		stbuf->st_uid = inode->uid;
		stbuf->st_gid = inode->gid;
		stbuf->st_size = inode->size;
		stbuf->st_blksize = superBlock->block_size;
		stbuf->st_blocks = superBlock->blocks;
		stbuf->st_atime = MAGIC;
	}
	else
	{
		return -1;
	}
	return 0;
}

// to get file attributes (metadata)
static void file_system_getattr(fuse_req_t req, fuse_ino_t ino,
								struct fuse_file_info *fi)
{
	struct stat stbuf;
	(void)fi;
	memset(&stbuf, 0, sizeof(stbuf));
	if (file_system_stat(ino, &stbuf) == -1)
		fuse_reply_err(req, ENOENT);
	else
		fuse_reply_attr(req, &stbuf, 1.0);
}

static void file_system_lookup(fuse_req_t req, fuse_ino_t parent, const char *name)
{
	struct fuse_entry_param e;
	char name_check = 0;
	struct tosfs_superblock *superBlock = data_mmapped;

	struct tosfs_dentry *dentries = data_mmapped + 2 * superBlock->block_size;
	void *end = dentries + 3 * superBlock->block_size;
	memset(&e, 0, sizeof(e));
	// Search inode value for file. Return an error if two files have the same name
	while ((void *)dentries < end)
	{
		if (strcmp(name, dentries->name) == 0)
		{
			name_check += 1;
			e.ino = dentries->inode;
		}
		dentries++;
	}

	if (parent != 1 || name_check != 1)
		fuse_reply_err(req, ENOENT);
	else
	{
		e.attr_timeout = 1.0;
		e.entry_timeout = 1.0;
		file_system_stat(e.ino, &e.attr);
		fuse_reply_entry(req, &e);
	}
}

static void file_system_readdir(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi)
{
	(void)fi;

	if (ino != 1) // The only directory is root
		fuse_reply_err(req, ENOTDIR);
	else
	{
		struct dirbuf b;
		struct tosfs_superblock *superBlock = data_mmapped;
		struct tosfs_inode *inode = data_mmapped + superBlock->block_size;

		// Superblock -> inodes -> dentries -> files
		struct tosfs_dentry *dentries = data_mmapped + 2 * superBlock->block_size;
		int i;

		memset(&b, 0, sizeof(b));
		// Add all files to buffer
		for (i = 0; i < superBlock->inodes + 1; i++)
		{
			// 1 -> file
			// 2 -> folder
			dirbuf_add(req, &b, dentries->name, (inode->mode == 33188) + 1); // inode->mode == 33188 for folder
			dentries++;
			inode++;
		}
		reply_buf_limited(req, b.p, b.size, off, size);
		free(b.p); // Free memory used for the buffer
	}
}

// Function to read data from the filesystem.
static void file_system_read(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi)
{
	(void)fi;
	struct tosfs_superblock *superBlock = data_mmapped; // Access the superblock.

	// Calculate the memory address for the file based on the inode number.
	char *text = data_mmapped + (ino + 1) * superBlock->block_size;
	// Send the requested data back to the requester.
	reply_buf_limited(req, text, strlen(text), off, size);
}

// Function to create a new file in the filesystem.
static void file_system_create(fuse_req_t req, fuse_ino_t parent, const char *name, mode_t mode, struct fuse_file_info *fi)
{
	(void)fi;
	struct tosfs_superblock *superBlock = data_mmapped; // Access the superblock.

	// Validate the file name and ensure there's space for a new file.
	if (strlen(name) <= 32 && parent == 1 && superBlock->inodes < superBlock->blocks)
	{
		int fileNumber = superBlock->inodes; // Get the current inode count.

		// Update the bitmaps to mark the new inode and block as used.
		superBlock->block_bitmap = (superBlock->block_bitmap << 1) + 1;
		superBlock->inode_bitmap = (superBlock->inode_bitmap << 1) + 2;
		superBlock->inodes++; // Increment the inode count.

		// Populate the inode information.
		struct tosfs_inode *inode = data_mmapped + superBlock->block_size + (fileNumber + 1) * sizeof(struct tosfs_inode);
		inode->inode = fileNumber + 1;
		inode->block_no = fileNumber + 1;
		inode->mode = mode;
		inode->perm = 438; // Default permissions.
		inode->nlink = 1;  // Set the link count.

		// Populate the directory entry information.
		struct tosfs_dentry *entry = data_mmapped + 2 * superBlock->block_size + (fileNumber + 1) * sizeof(struct tosfs_dentry);
		entry->inode = inode->inode;
		strcpy(entry->name, name); // Copy the file name.

		// Prepare and send the response with the new file's details.
		struct fuse_entry_param e;
		e.ino = inode->inode;
		file_system_stat(e.ino, &e.attr); // Retrieve file attributes.
		e.attr_timeout = 1.0;			  // Cache timeout for attributes.
		e.entry_timeout = 1.0;			  // Cache timeout for entries.
		fuse_reply_create(req, &e, fi);	  // Reply with the created file info.
	}
	else if (superBlock->inodes == superBlock->blocks)
	{
		fuse_reply_err(req, ENOSPC); // Disk is full.
	}
	else
	{
		fuse_reply_err(req, E2BIG); // File name too long.
	}
}

// Function to write data to a file in the filesystem.
static void file_system_write(fuse_req_t req, fuse_ino_t ino, const char *buf, size_t size, off_t off, struct fuse_file_info *fi)
{
	(void)fi;
	struct tosfs_superblock *superBlock = data_mmapped; // Access the superblock.

	// Check if the write operation fits within the block size.
	if (off + size < superBlock->block_size)
	{
		struct tosfs_inode *inode = data_mmapped + superBlock->block_size + ino * sizeof(struct tosfs_inode);
		char *writePosition = data_mmapped + (inode->block_no + 1) * superBlock->block_size + off;

		inode->size = off + size;	 // Update the inode size.
		strcpy(writePosition, buf);	 // Write the data to the specified position.
		fuse_reply_write(req, size); // Reply indicating the number of bytes written.
	}
	else
	{
		fuse_reply_err(req, EFBIG); // Write would exceed block size.
	}
}

static struct fuse_lowlevel_ops file_oper = {
	.lookup = file_system_lookup,
	.getattr = file_system_getattr,
	.readdir = file_system_readdir,
	.read = file_system_read,
	.create = file_system_create,
	.write = file_system_write,
};

int main(int argc, char **argv)
{
	int fd = open(argv[argc - 1], O_RDWR); // Open dump of file system
	if (fd == -1)
	{
		perror("file");
		return -1;
	}
	if (argc < 3)
	{
		perror("Usage: ./tp3 <mountpoint> <file>");
		return 0;
	}
	if (argc >= 3)
	{
		struct stat *buf = malloc(sizeof(struct stat));

		// Use stat function to get size of memory dump
		stat(argv[argc - 1], buf);
		printf("Size of file: %ld\n", buf->st_size);

		off_t size = buf->st_size;

		data_mmapped = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
		if (data_mmapped == (void *)-1)
		{
			perror("mmap");
			return -1;
		}

		// Print all informations on mapping
		struct tosfs_superblock *superBlock = data_mmapped;
		printf("---------------- Supper block data : ----------------\n");
		printf("Magic number: %#x\n", superBlock->magic);
		printf("Bitmap: %d\n", superBlock->block_bitmap);
		printf("Inode bitmap: %d\n", superBlock->inode_bitmap);
		printf("Block size: %d\n", superBlock->block_size);
		printf("Number of blocks: %d\n", superBlock->blocks);
		printf("Number of inodes: %d\n", superBlock->inodes);
		printf("Inode root: %d\n\n", superBlock->root_inode);
		struct tosfs_inode *inodes = data_mmapped + superBlock->block_size;
		struct tosfs_inode *p = inodes + 1;

		printf("---------------- Inodes data : ----------------\n");
		while (p <= inodes + superBlock->inodes)
		{
			printf("Inode %d :\n", p->inode);
			printf("Block number: %d\n", p->block_no);
			printf("UID: %d\n", p->uid);
			printf("GID: %d\n", p->gid);
			printf("Mode: %d\n", p->mode);
			printf("Permissions: %d\n", p->perm);
			printf("Size: %d\n", p->size);
			printf("NB links: %d\n\n", p->nlink);
			p++;
		}
		struct tosfs_dentry *dentries = data_mmapped + 2 * superBlock->block_size; // + superBlock->inodes * sizeof(struct tosfs_inode);
		struct tosfs_dentry *p2 = dentries;

		printf("---------------- Dentries data : ----------------\n");
		while (p2 < dentries + superBlock->inodes + 1)
		{
			printf("Inode %d ->  ", p2->inode);
			printf("Filename: %s\n", p2->name);
			p2++;
		}

		printf("---------------- Files data : ----------------\n");
		char *text = data_mmapped + 3 * superBlock->block_size;
		printf("first file: %s", text);
		text = text + superBlock->block_size;
		printf("second file: %s", text);
		printf("------------------------------------------------\n");
		struct fuse_args args = FUSE_ARGS_INIT(argc - 1, argv);
		struct fuse_chan *ch;
		char *mountpoint;
		int err = -1;

		if (fuse_parse_cmdline(&args, &mountpoint, NULL, NULL) != -1 &&
			(ch = fuse_mount(mountpoint, &args)) != NULL)
		{
			struct fuse_session *se;
			se = fuse_lowlevel_new(&args, &file_oper,
								   sizeof(file_oper), NULL);
			if (se != NULL)
			{
				if (fuse_set_signal_handlers(se) != -1)
				{
					fuse_session_add_chan(se, ch);
					err = fuse_session_loop(se);
					fuse_remove_signal_handlers(se);
					fuse_session_remove_chan(ch);
				}
				fuse_session_destroy(se);
			}
			fuse_unmount(mountpoint, ch);
		}
		fuse_opt_free_args(&args);

		return err ? 1 : 0;
	}
}