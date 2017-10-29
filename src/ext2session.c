#include "ext2session.h"
#include "ext2utility.h"
#include "ext2inode.h"

int getSuperBlock(int sess)
{
    sessions[sess].readFunction(1024/DISK_SECTOR_SIZE,(
            char*)&sessions[sess].superBlock,
            sectorsForByteCount(sizeof(sessions[sess].superBlock)));
    if (sessions[sess].superBlock.s_magic!=EXT2_SUPER_MAGIC)
        return ERROR_BAD_EXT2_MAGIC;
    else
        return 0;
}

int ext2initsession(void* readFunction, void* writeFunction)
{
    int sess=0;
    int lRetVal=0;
    
    for (int cnt=1;cnt<SESSION_MAX_COUNT;cnt++)
        if (sessions[cnt].sessionNbr==0)
        {
            sessions[cnt].sessionNbr=cnt;
            sessions[cnt].readFunction=readFunction;
            sessions[cnt].writeFunction=writeFunction;
            sess=cnt;
            break;
        }
    if (!sess)
        return ERROR_NO_FREE_SESSIONS;
    lRetVal=initSession(sess);
    if (lRetVal==0)
        return sess;
    else
        return lRetVal;
    
}

int initSession(int sess)
{
    int lRetVal=0;
    struct ext2_group_desc gd;
    struct ext2_inode inode;
    
    lRetVal=getSuperBlock(sess);
    if (lRetVal)
        return lRetVal;
    
    sessions[sess].blockSize=1024 << sessions[sess].superBlock.s_log_block_size;
    sessions[sess].inodes_per_block = sessions[sess].blockSize / sizeof(struct ext2_inode);;
    sessions[sess].inode_table_blocks = sessions[sess].superBlock.s_inodes_per_group / sessions[sess].inodes_per_block;
    sessions[sess].blockBitmap.blockGroupNum=-1;
    sessions[sess].inodeBitmap.blockGroupNum=-1;
    sessions->blockBitmap.blockGroupBitmap=NULL;
    sessions->inodeBitmap.iNodeBitmap=NULL;
    getBlockGroupDescriptor(sess, 0, &gd);
    read_inode(sess,2,&sessions[sess].rootDirInode);
    //read_inode(sessions[sess].device,2,&gd,,4096,sessions[sess].blockSize, sessions[sess].superBlock.s_inode_size);
    return 0;
}
