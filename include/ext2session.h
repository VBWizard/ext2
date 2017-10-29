/* 
 * File:   ext2session.h
 * Author: yogi
 *
 * Created on October 19, 2017, 7:29 AM
 */

#ifndef EXT2SESSION_H
#define	EXT2SESSION_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "ext2.h"
#include "ext2blockgroup.h"
    
    typedef struct tExt2Session
    {
        int sessionNbr;
        int (*readFunction)(int, void*, int);
        int (*writeFunction)(int, void*, int);
        struct ext2_super_block superBlock;
        int device;
        int blockSize, inodes_per_block, inode_table_blocks;
        struct ext2_inode rootDirInode;
        sBlockGroupBlockBitmap blockBitmap;
        sBlockGroupInodeBitmap inodeBitmap;
        
    } sExt2Session;

    //Array of session pointers
    __attribute__((visibility("default"))) sExt2Session* sessions;

    int ext2initsession(void* readFunction, void* writeFunction);
    int initSession(int sess);

#ifdef	__cplusplus
}
#endif

#endif	/* EXT2SESSION_H */

