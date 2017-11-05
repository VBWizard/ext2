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

#define BLOCK_IS_IN_BUFFER(block) (block<globalBlockBufferStartBlock?false:block>globalBlockBufferStartBlock+READ_BUFFER_BLOCK_COUNT?false:true)
    
#include "ext2.h"
void readDiskBlock(int sess, void* buffer, int block);
void populateReadBuffer(int sess, int block, int blockSize);

    char* blockBuffer;

#ifdef	__cplusplus
}
#endif

#endif	/* EXT2READ_H */

