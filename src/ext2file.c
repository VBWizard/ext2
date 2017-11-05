#include "ext2file.h"
#include "ext2inode.h"

int updateFileInode(sFile* file)
{
    write_inode(file);
    
}

int findFirstUnusedIndexInBlockList(sFile* file)
{
    if (file->totalBlockListCount>file->usedBlockListCount)
        for (int cnt=0;cnt<file->usedBlockListCount;cnt++)
        {
            if (file->blockList[cnt]==0)
                return cnt;
        }
    {
        //Figure out what kind of block list we need to use for the next block
    }
}

int findMaxBlockNumInBlockList(sFile* file)
{
    int max=0;
    
    if (file->totalBlockListCount>file->usedBlockListCount)
        for (int cnt=0;cnt<file->usedBlockListCount;cnt++)
        {
            if (file->blockList[cnt]>max)
                max=file->blockList[cnt];
        }
    if (max==0)
        max=-1;
    return max;
}

int updateFileBlockList(sFile* file, int block, bool added)
{
    int blockToUse;
    if (added)
    {
        blockToUse=findFirstUnusedIndexInBlockList(file);
        //Using individual blocks in the inode
        if (blockToUse<13)
        {
            file->inode.i_block[blockToUse]=block;
            file->usedBlockListCount+=1;
            file->blockList[blockToUse]=block;
        }
    }
    else
    {}
}
