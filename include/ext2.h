/* 
 * File:   ext2.h
 * Author: yogi
 *
 * Created on October 13, 2017, 6:02 PM
 */

#ifndef EXT2_H
#define	EXT2_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "ext2_fs.h"

#define READ_BUFFER_BLOCK_COUNT 256
#define FILE_HANDLE_MAX_COUNT 1024
#define SESSION_MAX_COUNT 100
#define DISK_SECTOR_SIZE 512
#define BASE_OFFSET 4096  /* location of the super-block in the first group */
#define BLOCK_OFFSET_FIRSTGROUP(block) (BASE_OFFSET + (block-1)*block_size)
#define BLOCK_OFFSET(block,block_size) (BASE_OFFSET + (block-1)*block_size)
    
    typedef struct tExt2Session
    {
        int sessionNbr;
        int (*readFunction)(int, char*, int);
        int (*writeFunction)(int, char*, int, int);
        struct ext2_super_block superBlock;
        int device;
        int blockSize, inodes_per_block, inode_table_blocks;
        struct ext2_inode rootDirInode;
    } sExt2Session;

    typedef struct tFile
    {
        int filePtr;
        int sess;
        int base,offset;
        struct ext2_inode inode;
        char symlinkPath[1024];
        int inodeNumber;
        char* name;
        char path[1024];
        int lastBlockFreeSize;
        unsigned int* blockList;
    } sFile;

int ext2Read (int FILE, char* buffer, int bytes);
int ext2Write (int FILE, char* buffer, int bytes);
int ext2Seek (int FILE, int pos, int whence);

    
    void* (*mallocF)(int);
    void (*freeF)(void*);
    sFile* fileHandles;
    bool libInitialized;
    sExt2Session* sessions;
    sFile* files;
    void libInit();
    void libClose();

    char *readBuffer;
    int readBufferStartBlock;
    
#include "ext2Errors.h"

#ifdef	__cplusplus
}
#endif

#endif	/* EXT2_H */

