#include "ext2.h"
#include "ext2blockgroup.h"
#include "ext2inode.h"
#include "ext2utility.h"
#include "ext2session.h"

void calculateInodeLocation(int sess, int inode_no, int* inode_sector, int* inode_sector_offset)
{
    int inode_table_location=0, inode_location=0, lTemp=0;
    int blockGroupNumber=(inode_no-1)/sessions[sess].superBlock.s_inodes_per_group;
    struct ext2_group_desc blockGroup;

    //Get the descriptor for the block group that the node is in
    getBlockGroupDescriptor(sess, blockGroupNumber, &blockGroup);
    
    //Calculate the inode table location within the block group
    inode_table_location=blockGroup.bg_inode_table*sessions[sess].blockSize;
    //inode_table_location=blockGroupNumber*sessions[sess].superBlock.s_blocks_per_group*sessions[sess].blockSize+4096;
    //inode_table_location+=(blockGroup.bg_inode_table-1)*sessions[sess].blockSize;
    
    //Calculate the inode location within the inode table
//    inode_location=inode_table_location+( ((inode_no-1) % sessions[sess].superBlock.s_blocks_per_group)
//            *sessions[sess].superBlock.s_inode_size);
    
    lTemp=(inode_no-1)%sessions[sess].superBlock.s_inodes_per_group;
    inode_location=inode_table_location + (lTemp*sessions[sess].superBlock.s_inode_size);

    
    //Calculate the sector number on the disk where the inode sector resides
    *inode_sector=inode_location/DISK_SECTOR_SIZE;
    
    //Calculate the offset within the inode sector where the inode resides
    *inode_sector_offset=inode_location%DISK_SECTOR_SIZE;
    
}

int write_inode(sFile* file)
{
    int inode_sector, inode_sector_offset;
    char sectorBuffer[512];
    
    calculateInodeLocation(file->sess,file->inodeNumber,&inode_sector,&inode_sector_offset);
    read_inode_sector(file->sess,file->inodeNumber,sectorBuffer);
    memcpy(sectorBuffer+inode_sector_offset,&file->inode,sizeof(struct ext2_inode));
    sessions[file->sess].writeFunction(inode_sector,sectorBuffer,1);
}

//Read the sector containg an inode into a sector buffer
//Returns the offset of the inode within the buffer, and the buffer
int read_inode_sector(int sess, int inode_no, char* inode_sector_buffer)
{
    int inode_sector, inode_sector_offset;
    
    calculateInodeLocation(sess,inode_no,&inode_sector,&inode_sector_offset);
    
    //Read the inode sector
    sessions[sess].readFunction(inode_sector,inode_sector_buffer,1);
    
    return inode_sector_offset;
    
}

int read_inode(int sess, int inode_no, struct ext2_inode *inode)
{
    char sector_buffer[DISK_SECTOR_SIZE];
    int inode_sector_offset=0;
    
    inode_sector_offset=read_inode_sector(sess, inode_no, sector_buffer);
    //Copy the inode from the sector buffer to the inode structure
    memcpy(inode,sector_buffer+inode_sector_offset,sizeof(struct ext2_inode));
}

int updateInode(sFile* file)
{
    write_inode(file);
    
}

