#include "ext2block.h"
#include "ext2session.h"
#include "ext2read.h"
#include "ext2utility.h"

//Get the list of blocks for a file 
int setFileBlockList(sFile *file)
{
    int blockListSize=0;
    int curr=0,currSaveBegin=0,currSaveEnd=0,currTempListPtr=0;
    unsigned int* lvl2BlockList;
    unsigned int* lvl3BlockList;
    int blockSize=sessions[file->sess].blockSize;
    
    file->blockList12Used=file->blockList13Used=file->blockList14Used=false;
    
    //Figure out how big the block list needs to be
    for (int cnt=0;cnt<15;cnt++)
    {
        if (file->inode.i_block[cnt]>0)
        {
            //First 12 pointer in the inode block list are block #s with data
            if (cnt<12)
                blockListSize++;
            //The 13th block number is an indirect block.  It points at an array of block IDs
            else if (cnt==12)
            {
                blockListSize+=blockSize;
                file->blockList12Used=true;
            }
            //The 14th entry in this array is the block number of the first doubly-indirect block; which is a block containing an array of 
            //indirect block IDs, with each of those indirect blocks containing an array of blocks containing the data. In a 1KiB block size, 
            //there would be 256 indirect blocks per doubly-indirect block, with 256 direct blocks per indirect block for a total of 65536 blocks 
            //per doubly-indirect block. 
            else if (cnt==13)
            {
                blockListSize+=blockSize*blockSize;
                file->blockList13Used=true;
            }
            //Fifteenth pointer in the inode block list is a triple indirect block
            else if (cnt==14)
            {
                blockListSize+=blockSize*blockSize*blockSize;
                file->blockList14Used=true;
            }
        }
        else
            break;
    }
    file->totalBlockListCount=blockListSize;
    //Allocate the space for the list
    file->blockList=mallocF(blockListSize*sizeof(int));
    memset(file->blockList,0,blockListSize*sizeof(int));
    //Populate it!
    for (int cnt=0;cnt<15;cnt++)
    {
        if (file->inode.i_block[cnt]==0)
            break;
        if (cnt<12)
            file->blockList[curr++]=file->inode.i_block[cnt];
        else if (cnt==12)
        {
            //The 13th block number is an indirect block.  It points at an array of block IDs
            readDiskBlock(file->sess,
                    &file->blockList[curr],
                    file->inode.i_block[cnt]);
            //Find the first unused entry
            while (file->blockList[curr]!=0)
                curr++;
            currSaveBegin=curr;
        }
        else if (cnt==13)
        {
            lvl2BlockList=mallocF(blockSize);
            lvl3BlockList=mallocF(blockSize);
            //The 14th entry in this array is the block number of the first doubly-indirect block; which is a block containing an array of 
            //indirect block IDs, with each of those indirect blocks containing an array of blocks containing the data. In a 1KiB block size, 
            //there would be 256 indirect blocks per doubly-indirect block, with 256 direct blocks per indirect block for a total of 65536 blocks 
            //per doubly-indirect block. 
            
            //Get the list of doubly-indirect blocks
            readDiskBlock(file->sess,   //block to read
                    lvl2BlockList,               //session
                    file->inode.i_block[cnt]); //buffer address

            //For each level 2 block list, retrieve the indirect blocks list
            for (int lvl2=0;lvl2<1024;lvl2++)
            {
                if (lvl2BlockList[lvl2]==0)
                    break;
                
                readDiskBlock(file->sess,   //block to read
                        &file->blockList[curr],               //session
                        lvl2BlockList[lvl2]); //buffer address
                
                //bring curr back to the end
                while (file->blockList[curr]!=0)
                    curr++;
            }
            while (file->blockList[curr]!=0)
                curr++;
       
/*            printf("\n");
            for (int cnt=0;cnt<curr;cnt++)
            {
                printf("%08x ",(unsigned int)file->blockList[cnt]);
            if ((cnt-1)%20==0)
                printf("\n");
            }
*/            
            freeF(lvl2BlockList);
            freeF(lvl3BlockList);
        }
        else if (cnt==14)
        {
            int a=cnt;
            int b=1;
            b-=1;
            a=a/b;          //divide by zero error signifies that we don't yet do 
        }
    }
    //if block pointers above 11 have been used then we've processed indirect blocks and forwarded curr to the first unused block list index every time
    //so back off by 1 to get back to the last used block list index and that is our count of blocks
    if (curr>11)
        curr-=1;
    file->usedBlockListCount=curr;
}

int allocateBlock(sFile* file)
{
    int blockGroupNumber=file->inodeNumber/sessions[file->sess].superBlock.s_inodes_per_group;
    struct ext2_group_desc blockGroup;

    //Get the descriptor for the block group that the node is in
    getBlockGroupDescriptor(file->sess, blockGroupNumber, &blockGroup);

/*    //Find the maximum block used for the file's inode
    int maxBlockUsed=arrayMaxValue(file->blockList,file->usedBlockListCount);
    
    //Localize the max block used to the current block group
    maxBlockUsed-=(BLOCK_SIZE(file->sess)*blockGroupNumber);
    
    //Get the bit offset of the int to start at in the bitmap
    int bitmapPtr=maxBlockUsed/(sizeof(int)*8);
    //Add the offset of the bit to start at in the bitmap
    bitmapPtr+=maxBlockUsed%(sizeof(int)*8);
  */  
    
    //Find the first unused block in the block group
    int blockListFreeBlock=bitmapFFZ(sessions[file->sess].blockBitmap.blockGroupBitmap,sessions[file->sess].blockSize);
    int groupBlock=blockListFreeBlock+(blockGroupNumber*sessions[file->sess].superBlock.s_blocks_per_group);

    updateBGBitmap(file, blockGroupNumber, groupBlock, true);
    
    return groupBlock;
}
