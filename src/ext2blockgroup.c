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
        readSize=session->blockSize;
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

void getBlockGroupBuffer(int sess, int blockGroupNum, void* buffer)
{
    int loc=sessions[sess].blockSize;
    sessions[sess].readFunction(loc/DISK_SECTOR_SIZE,buffer,sectorsForByteCount(sessions[sess].blockSize));
}

void writeBlockGroupBuffer(int sess, int blockGroupNum, void* buffer)
{
    int loc=sessions[sess].blockSize;
    sessions[sess].writeFunction(loc/DISK_SECTOR_SIZE,buffer,sectorsForByteCount(sessions[sess].blockSize));
}

void readBlockGroup(int sess, int blockGroupNum, struct ext2_group_desc* gd)
{
    char buffer[sessions[sess].blockSize];
    
    getBlockGroupBuffer(sess, blockGroupNum,buffer);
    int offset=0;
    offset=blockGroupNum*sizeof(struct ext2_group_desc);
    memcpy(gd,buffer+offset,sizeof(struct ext2_group_desc));
    getBitmaps(sess, blockGroupNum, gd);
}

int writeBlockGroup(int sess, int blockGroupNum, struct ext2_group_desc* gd)
{
    char buffer[sessions[sess].blockSize];

    getBlockGroupBuffer(sess, blockGroupNum,buffer);
    int offset=0;
    offset=blockGroupNum*sizeof(struct ext2_group_desc);
    memcpy(buffer+offset,gd,sizeof(struct ext2_group_desc));
    writeBlockGroupBuffer(sess,blockGroupNum,buffer);
}

void updateBGBitmap(sFile* file, int blockGroupNum, int block, bool used)
{
    struct ext2_group_desc blockGroup;

    //Get the descriptor for the block group that the node is in
    readBlockGroup(file->sess, blockGroupNum, &blockGroup);
    if (used)
        bitmapSet(sessions[file->sess].blockBitmap.blockGroupBitmap,block);
    else
        bitmapUnSet(sessions[file->sess].blockBitmap.blockGroupBitmap,block);
    
    writeBGBitmap(file->sess,blockGroupNum,&blockGroup);
}