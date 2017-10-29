/* 
 * File:   ext2write.h
 * Author: yogi
 *
 * Created on October 18, 2017, 7:43 AM
 */

#ifndef EXT2WRITE_H
#define	EXT2WRITE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "ext2.h"

    void writeDiskBlock(int sess, void* buffer, int block);

#ifdef	__cplusplus
}
#endif

#endif	/* EXT2WRITE_H */

