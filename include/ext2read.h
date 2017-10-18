/* 
 * File:   ext2read.h
 * Author: yogi
 *
 * Created on October 16, 2017, 7:16 PM
 */

#ifndef EXT2READ_H
#define	EXT2READ_H

#ifdef	__cplusplus
extern "C" {
#endif

#define BLOCK_IS_IN_BUFFER(block) (block<readBufferStartBlock?false:block>readBufferStartBlock+READ_BUFFER_BLOCK_COUNT?false:true)
    
#include "ext2.h"
int ext2Read (int FILE, char* buffer, int bytes);
void readFileBlock(sFile* file, char* buffer, int block);
void readDiskBlock(int sess, char* buffer, int block);

    char* blockBuffer;

#ifdef	__cplusplus
}
#endif

#endif	/* EXT2READ_H */

