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
#include "ext2methods.h"
    
#define READ_BUFFER_BLOCK_COUNT 256
#define FILE_HANDLE_MAX_COUNT 1024
#define SESSION_MAX_COUNT 100
#define DISK_SECTOR_SIZE 512
#define BASE_OFFSET 4096  /* location of the super-block in the first group */
#define BLOCK_OFFSET_FIRSTGROUP(block) (BASE_OFFSET + (block-1)*block_size)
#define BLOCK_OFFSET(block,block_size) (BASE_OFFSET + (block-1)*block_size)
    
    //Pointers to caller's malloc and free functions
    void* (*mallocF)(int);
    void (*freeF)(void*);
    
    
    //Boolean identifying whether the library has been initialized
    bool libInitialized;
    
    void ext2init();
    void ext2deinit();

    //Pointer to the sequential block buffer
    __attribute__((visibility("default"))) char *globalBlockBuffer;
    
    //Identifies the starting block # in the sequential block buffer
    __attribute__((visibility("default"))) int globalBlockBufferStartBlock;
    
    
    
#include "ext2Errors.h"

#ifdef	__cplusplus
}
#endif

#endif	/* EXT2_H */

