/* 
 * File:   ext2block.h
 * Author: yogi
 *
 * Created on October 19, 2017, 6:39 AM
 */

#ifndef EXT2BLOCK_H
#define	EXT2BLOCK_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "ext2.h"

    int setFileBlockList(sFile *file);
    int allocateBlock(sFile* file);

#define DISK_BLOCK_SIZE(sess) (sessions[sess].blockSize)
    
#ifdef	__cplusplus
}
#endif

#endif	/* EXT2BLOCK_H */

