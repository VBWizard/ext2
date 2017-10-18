#include "ext2.h"
#include "utility.h"

void getBlockGroupDescriptor(int sess, int blockGroupNum, struct ext2_group_desc* gd)
{
    char buffer[1024];
    
    int loc=sessions[sess].superBlock.s_blocks_per_group * blockGroupNum * sessions[sess].blockSize + sessions[sess].blockSize;
    sessions[sess].readFunction(loc/DISK_SECTOR_SIZE,buffer,sectorsForByteCount(1024));
    memcpy(gd,buffer,sizeof(struct ext2_group_desc));
}

