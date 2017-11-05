#include "ext2write.h"
#include "ext2utility.h"
#include "ext2inode.h"
#include "ext2read.h"
#include "ext2session.h"
#include "ext2block.h"
#include <time.h>


void writeDiskBlock(int sess, void* buffer, int block)
{
    int blockSize=DISK_BLOCK_SIZE(sess);

    sessions[sess].writeFunction(block*(blockSize/512),buffer,blockSize/512);
}

int mergeWriteWithBufferAndWrite(sFile* file, unsigned char* src, int bytes, int blockOffset, int blockSize)
{
    int offset=0;
    int block=file->blockList[file->offset/DISK_BLOCK_SIZE(file->sess)];
    
    //If the block to write is not in the block buffer, get it there
    if (!BLOCK_IS_IN_BUFFER(block))
    {
        populateReadBuffer(file->sess, block, blockSize);
    }
    offset=((block-globalBlockBufferStartBlock)*blockSize)+blockOffset;
    memcpy(globalBlockBuffer+offset,src,bytes);
    writeDiskBlock(file->sess, globalBlockBuffer+((block-globalBlockBufferStartBlock)*blockSize), block);

}

int writeNewBlock(sFile* file, void* buffer, int writeSize)
{
    int block=allocateBlock(file);
    unsigned char localBuffer[sessions[file->sess].blockSize];
    
    //Zero out the local buffer and then write the caller's buffer to it (incase caller's buffer is smaller than the write size)
    memset(localBuffer,0,DISK_BLOCK_SIZE(file->sess));
    memcpy(localBuffer,buffer,writeSize);
    //need a new block
    writeDiskBlock(file->sess, localBuffer, block);
    //TODO: Update the inode's block list
    return block;

}

int write (sFile* file, char* buffer, int bytes)
{
    int alignWriteSize=0, newBytes=0, writeSize=0, bytesWritten=0, callerBufferPtr=0;

        //Identify any bytes that are past the current end of the file
    if (file->offset+bytes>file->inode.i_size)
    {
        newBytes=file->inode.i_size-file->offset+bytes;
    }

    alignWriteSize=file->offset%sessions[file->sess].blockSize;
    
    if (alignWriteSize)    //Non-aligned write (not divisible by blockSize)
        writeSize=sessions[file->sess].blockSize-alignWriteSize; //get remaining bytes to become aligned again

    //But if we are not writing to the end of the file, then don't write alignWriteSize
    if (writeSize>bytes) 
        writeSize=bytes;
    else if (writeSize==0)
        writeSize=sessions[file->sess].blockSize;
    callerBufferPtr=0;
    
    while (bytesWritten<bytes)
    {
        if (file->blockList[file->offset/DISK_BLOCK_SIZE(file->sess)]!=0)
            mergeWriteWithBufferAndWrite(file,
                    buffer+callerBufferPtr,
                    writeSize,
                    (file->offset%sessions[file->sess].blockSize),//offset within the block to write
                    sessions[file->sess].blockSize);
        else
        {
            int newBlock=writeNewBlock(file, buffer+callerBufferPtr,writeSize);
            updateFileBlockList(file,newBlock,true);
            sessions[file->sess].superBlock.s_free_blocks_count--;
        }
        //Increment the bytes written
        bytesWritten+=writeSize;
        //Increment the file's pointer
        file->offset+=writeSize;
        //Increment the pointer to the caller's buffer for the next write
        callerBufferPtr+=writeSize;
        
        //If a block or more remains to be written set writeSize to block size, otherwise set writeSize to bytes left to write
        if (bytes-bytesWritten>=sessions[file->sess].blockSize)
            writeSize=sessions[file->sess].blockSize;
        else
            writeSize=bytes-bytesWritten;
    }

    if (newBytes)
        file->inode.i_size+=newBytes;
    
    //TODO: Update last updated time stamp on file
    //file->inode.i_mtime=time;
    updateFileInode(file);
    writeSuperBlock(file->sess);

}

int ext2write(int FILE, char* buffer, int bytes)
{
    
    //Verify handle
    if (invalidFileHandle(FILE))
        return ERROR_INVALID_FILE_HANDLE;
    
    write(&fileHandles[FILE], buffer, bytes);
}

/*int ext2Write (int FILE, char* buffer, int bytes)
{
    int newBytes=0;
    
    if (invalidFileHandle(FILE))
        return ERROR_INVALID_FILE_HANDLE;
    
    //Update the file size if the write will take us past the current end of the file
    if (fileHandles[FILE].offset+bytes>fileHandles[FILE].inode.i_size)
    {
        newBytes=fileHandles[FILE].inode.i_size-fileHandles[FILE].offset+bytes;
        //fileHandles[FILE].inode.i_size+=newBytes;
    }
    
    //If we are writing to the end of the file and there's enough room in the last assigned block for this file then just write to that block
    if (newBytes>0)
    {
        if (fileHandles[FILE].lastBlockFreeSize>newBytes)
        {
            //New bytes and there is enough space in the last block for the bytes being added
            mergeWriteWithBufferAndWrite(FILE,buffer,bytes,fileHandles[FILE].offset,sessions[fileHandles[FILE].sess].blockSize);
            fileHandles[FILE].lastBlockFreeSize-=newBytes;
            fileHandles[FILE].inode.i_size+=newBytes;
            updateInode(FILE,&fileHandles[FILE]);
        }
        else
        {
            //New bytes and there is NOT enough space in the last block for the bytes being added, so add a block
        }
        //No new bytes, writing to an existing block
    }
    
    
    //handle the rest of the cases here
}
*/