/* 
 * File:   extblockgroup.h
 * Author: yogi
 *
 * Created on October 16, 2017, 2:59 AM
 */

#ifndef EXTBLOCKGROUP_H
#define	EXTBLOCKGROUP_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "ext2file.h"
    
    void readBlockGroup(int sess, int blockGroupNum, struct ext2_group_desc* gd);
    void updateBGBitmap(sFile* file, int blockGroupNum, int block, bool used);
    
    typedef struct tsBlockGroupBlockBitmap
    {
        //The disk block group number this structure represents
        int blockGroupNum;
        //Pointer to a block group bitmap
        unsigned int* blockGroupBitmap;
        //The number of blocks in the block group
        int blockGroupBlockCount;
    } sBlockGroupBlockBitmap;
    
    typedef struct tsBlockGroupInodeBitmap
    {
        //The disk block group number this structure represents
        int blockGroupNum;
        //Pointer to an inode bitmap
        unsigned int* iNodeBitmap;
        //Identifies which disk block group the iNodeBitmap is for 
        int iNodeBitmapGroupNumber;
        //The number of blocks in the block group
        int blockGroupBlockCount;
    } sBlockGroupInodeBitmap;

#ifdef	__cplusplus
}
#endif

#endif	/* EXTBLOCKGROUP_H */

