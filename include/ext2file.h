/* 
 * File:   ext2file.h
 * Author: yogi
 *
 * Created on October 29, 2017, 2:45 PM
 */

#ifndef EXT2FILE_H
#define	EXT2FILE_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdbool.h>
#include "ext2inode.h"
    
    typedef struct tFile
    {
        int filePtr;
        int sess;
        int base,offset, eofOffset;
        struct ext2_inode inode;
        char symlinkPath[1024];
        int inodeNumber;
        char* name;
        char path[1024];
        int lastBlockFreeSize;
        unsigned int* blockList;
        unsigned int usedBlockListCount, totalBlockListCount;
        bool blockList12Used, blockList13Used, blockList14Used;
    } sFile;

    //Array of file handle pointers
    __attribute__((visibility("default"))) sFile* fileHandles;
    int updateFileInode(sFile* file);
    int updateFileBlockList(sFile* file, int block, bool added);
    int findMaxBlockNumInBlockList(sFile* file);
    int write_inode(sFile* file);

#ifdef	__cplusplus
}
#endif

#endif	/* EXT2FILE_H */

