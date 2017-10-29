#include "ext2.h"
#include "ext2utility.h"
#include "ext2session.h"
#include "ext2utility.h"
#include "ext2blockgroup.h"
#include "ext2write.h"

void writeBGBitmap(int sess, int blockGroupNum, struct ext2_group_desc* gd)
{
    
    writeDiskBlock(sess, sessions[sess].blockBitmap.blockGroupBitmap,gd->bg_block_bitmap);
}

void getBitmaps(int sess, int blockGroupNum, struct ext2_group_desc* gd)
{
    sExt2Session* session=&sessions[sess];
    int bgLoc=0, bgBitmapLoc=0, inodeBitmapLoc=0, readSize=0;
    
    if (session->blockBitmap.blockGroupNum!=blockGroupNum)
    {
        //Get the block group bitmap
        //NOTE: bg_block_bitmap is 1 based, but we need 0 based, so decrement it by 1
        bgBitmapLoc=(gd->bg_block_bitmap)*sessions[sess].blockSize;
        session->blockBitmap.blockGroupNum=blockGroupNum;
        session->blockBitmap.blockGroupBlockCount=session->superBlock.s_blocks_per_group;
        if (session->blockBitmap.blockGroupBitmap!=NULL)
            freeF(session->blockBitmap.blockGroupBitmap);
        readSize=session->blockBitmap.blockGroupBlockCount/(sizeof(int)*8);
        session->blockBitmap.blockGroupBitmap=mallocF(sessions[sess].blockSize);
        session->readFunction(bgBitmapLoc/DISK_SECTOR_SIZE,sessions[sess].blockBitmap.blockGroupBitmap,sectorsForByteCount(readSize));
        
    }
    if (session->inodeBitmap.blockGroupNum!=blockGroupNum)
    {
        //NOTE: bg_inode_bitmap is 1 based, but we need 0 based, so decrement it by 1
        inodeBitmapLoc=(gd->bg_inode_bitmap)*sessions[sess].blockSize;
        session->inodeBitmap.blockGroupNum=blockGroupNum;
        session->inodeBitmap.blockGroupBlockCount=session->superBlock.s_blocks_per_group;
        if (session->inodeBitmap.iNodeBitmap!=NULL)
            freeF(session->inodeBitmap.iNodeBitmap);
        readSize=session[sess].blockSize;
        session->inodeBitmap.iNodeBitmap=mallocF(sessions[sess].blockSize);
        session->readFunction(inodeBitmapLoc/DISK_SECTOR_SIZE,sessions->inodeBitmap.iNodeBitmap,sectorsForByteCount(readSize));
    }   

}

void getBlockGroupDescriptor(int sess, int blockGroupNum, struct ext2_group_desc* gd)
{
    char buffer[1024];
    
    //int loc=sessions[sess].superBlock.s_blocks_per_group * blockGroupNum * sessions[sess].blockSize;
    int loc=sessions[sess].blockSize;
    sessions[sess].readFunction(loc/DISK_SECTOR_SIZE,buffer,sectorsForByteCount(1024));
    int offset=0;
    int a=sizeof(struct ext2_group_desc);
    offset=blockGroupNum*sizeof(struct ext2_group_desc);
    memcpy(gd,buffer+offset,sizeof(struct ext2_group_desc));
    getBitmaps(sess, blockGroupNum, gd);
}

void updateBGBitmap(sFile* file, int blockGroupNum, int block, bool used)
{
    struct ext2_group_desc blockGroup;

    //Get the descriptor for the block group that the node is in
    getBlockGroupDescriptor(file->sess, blockGroupNum, &blockGroup);
    if (used)
        bitmapSet(sessions[file->sess].blockBitmap.blockGroupBitmap,block);
    else
        bitmapUnSet(sessions[file->sess].blockBitmap.blockGroupBitmap,block);
    
    writeBGBitmap(file->sess,blockGroupNum,&blockGroup);
}