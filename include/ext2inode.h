/* 
 * File:   ext2inode.h
 * Author: yogi
 *
 * Created on October 16, 2017, 2:55 AM
 */

#ifndef EXT2INODE_H
#define	EXT2INODE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "ext2.h"

    int read_inode_sector(int sess, int inode_no, char* inode_sector_buffer);
    int read_inode(int sess, int inode_no, struct ext2_inode *inode);
    int updateInode(sFile* file);
    void calculateInodeLocation(int sess, int inode_no, int* inode_sector, int* inode_sector_offset);
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* EXT2INODE_H */

