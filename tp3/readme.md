# FUSE-Based File System

This project implements a simple file system using the FUSE (Filesystem in Userspace) library. The file system is designed to work with a custom binary file format and allows for basic file operations such as reading, writing, creating files, and directory listing.

## Table of Contents

- [Features](#features)
- [How It Works](#how-it-works)
- [File System Structure](#file-system-structure)
- [Usage](#usage)

## Features

- Create, read, and write files in a custom file system.
- List files in the root directory.
- Retrieve file attributes (metadata).
- Handle errors for invalid operations.

## How It Works

The code defines a FUSE file system that interacts with a memory-mapped file containing the file system structure. The file system structure consists of a superblock, inodes, directory entries (dentries), and data blocks.

1. **Memory Mapping**: The file system uses `mmap` to map the binary file into memory, allowing direct access to its contents.

2. **File Operations**: The main operations include:
   - **`lookup`**: Find a file by name.
   - **`getattr`**: Retrieve file attributes.
   - **`readdir`**: List files in a directory.
   - **`read`**: Read data from a file.
   - **`create`**: Create a new file.
   - **`write`**: Write data to a file.

3. **Data Structures**: The file system uses custom structures (`tosfs_superblock`, `tosfs_inode`, `tosfs_dentry`) to represent the superblock, inodes, and directory entries.

## File System Structure

- **Superblock**: Contains metadata about the file system, such as magic number, block size, and inode count.
- **Inodes**: Each inode contains information about a file, including its size, permissions, and block number.
- **Directory Entries**: Each entry maps a filename to an inode, allowing for directory traversal.

## Usage

To run the file system:

1. Compile the code using a C compiler:

   ```bash
    gcc -Wall main.c `pkg-config fuse --cflags --libs` -o main
    ```

2. Run the compiled binary with a mount point:

   ```bash
    ./main /tmp/futosfs -d ressources/test_tosfs_files 
   ```
