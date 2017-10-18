#include "ext2read.h"
#include "utility.h"

int getBlockNumForLoc(int sess, int loc, unsigned int* blockList)
{
    int blkNum=loc/sessions[sess].blockSize;
    return blockList[blkNum];
}

void readDiskBlock(int sess, char* buffer, int block)
{
    int offset=0;
    
    if (!BLOCK_IS_IN_BUFFER(block))
    {
        sessions[sess].readFunction(block*(sessions[sess].blockSize/512),
                (char*)readBuffer,
                (READ_BUFFER_BLOCK_COUNT*sessions[sess].blockSize)/512);
                readBufferStartBlock=block;
    }
    offset=(block-readBufferStartBlock)*sessions[sess].blockSize;
    memcpy(buffer,readBuffer+offset,sessions[sess].blockSize);
}

void readFileBlock(sFile* file, char* buffer, int block)
{
    int offset=0;
    
    if (!BLOCK_IS_IN_BUFFER(block))
    {
        sessions[file->sess].readFunction(block*(sessions[file->sess].blockSize/512),
                (char*)readBuffer,
                (READ_BUFFER_BLOCK_COUNT*sessions[file->sess].blockSize)/512);
                readBufferStartBlock=block;
    }
    offset=(block-readBufferStartBlock)*sessions[file->sess].blockSize;
    memcpy(buffer,readBuffer+offset,sessions[file->sess].blockSize);
}


int ext2ReadN(int FILE, char* buffer, int bytes)
{
    int realSize=bytes, readSize=0;
    int firstBlockOffset=0;
    int bytesRead=0, bytesThisRead=0;
    
    //If handle is invalid, return an error
    if (invalidFileHandle(FILE))
        return ERROR_INVALID_FILE_HANDLE;

    //If current offset is past the end of the file, return 0 (as in no characters read)
    if (fileHandles[FILE].offset>fileHandles[FILE].inode.i_size)
        return 0;

    if (readBuffer==NULL)
        readBuffer=mallocF(READ_BUFFER_BLOCK_COUNT*sessions[fileHandles[FILE].sess].blockSize);
    
    blockBuffer=mallocF(sessions[fileHandles[FILE].sess].blockSize);

    //Clear the caller's buffer
    memset(buffer,0,bytes);

    //If request is to read past end of file, adjust the size we will read to only read to the end of the file
    if (fileHandles[FILE].offset+realSize>fileHandles[FILE].inode.i_size)
        realSize=fileHandles[FILE].inode.i_size - fileHandles[FILE].offset;
    else
        realSize=bytes;

    //Set the offset to use when writing from the local buffer to the caller's buffer for the first read
    //based on the offset within the file
    firstBlockOffset=(fileHandles[FILE].base+fileHandles[FILE].offset)%sessions[fileHandles[FILE].sess].blockSize;
    
    readSize=realSize;
    //if ( realSize>DISK_SECTOR_SIZE && ((fileHandles[FILE].base+fileHandles[FILE].offset)%DISK_SECTOR_SIZE)!=0)
    //Read is across sector boundry, so add a sector to the read size
    if (readSize%DISK_SECTOR_SIZE && readSize+firstBlockOffset>DISK_SECTOR_SIZE)
        readSize+=DISK_SECTOR_SIZE;
    
    //Adjust read size to nearest full sector size
    if (readSize%DISK_SECTOR_SIZE!=0)
        readSize+=DISK_SECTOR_SIZE-(readSize%DISK_SECTOR_SIZE);
    
    //Loop reading 1 block at a time until we've read the entire size requested
    while (bytesRead < realSize)
    {
        //Read a block from the file
        readFileBlock(&fileHandles[FILE],                           //The file handle
                blockBuffer,                                        //The local block buffer
                getBlockNumForLoc(fileHandles[FILE].sess,           //The block number for the offset in the file
                                    fileHandles[FILE].offset,
                                    fileHandles[FILE].blockList));
        //Calculate the size to write to the caller's buffer
        if (realSize-bytesRead>=sessions[fileHandles[FILE].sess].blockSize)
            bytesThisRead=sessions[fileHandles[FILE].sess].blockSize;
        else
            bytesThisRead=realSize-bytesRead;
        //Copy from the local buffer to the caller's buffer
        memcpy(buffer+bytesRead,blockBuffer+firstBlockOffset,bytesThisRead);
        //Increment the file pointer by the number of bytes this read
        fileHandles[FILE].offset+=bytesThisRead;
        //Increment the total bytes read by the number of bytes this read
        bytesRead+=bytesThisRead;
        //For future reads, read an entire block at a time
        firstBlockOffset=0;
    }
    
    freeF(blockBuffer);
    return bytesRead;
}
int ext2Read (int FILE, char* buffer, int bytes)
{
    static char *iBuffer=NULL;
    int realSize=bytes, readSize=0;
    int startSector=0;
    int iBufferOffset=0;
    
    //If handle is invalid, return an error
    if (invalidFileHandle(FILE))
        return ERROR_INVALID_FILE_HANDLE;
    
    //If current offset is past the end of the file, return 0 (as in no characters read)
    if (fileHandles[FILE].offset>fileHandles[FILE].inode.i_size)
        return 0;

    if (iBuffer==NULL)
        iBuffer=mallocF(1024*1024*1);
    
    if (bytes>1024*1024*1)
        return 0;
    
    //Clear the caller's buffer
    memset(buffer,0,bytes);

    //If request is to read past end of file, adjust the size we will read to only read to the end of the file
    if (fileHandles[FILE].offset+realSize>fileHandles[FILE].inode.i_size)
        realSize=fileHandles[FILE].inode.i_size - fileHandles[FILE].offset;
    else
        realSize=bytes;

    iBufferOffset=(fileHandles[FILE].base+fileHandles[FILE].offset)%DISK_SECTOR_SIZE;
    
    readSize=realSize;
    //if ( realSize>DISK_SECTOR_SIZE && ((fileHandles[FILE].base+fileHandles[FILE].offset)%DISK_SECTOR_SIZE)!=0)
    //Read is across sector boundry, so add a sector to the read size
    if (readSize%DISK_SECTOR_SIZE && readSize+iBufferOffset>DISK_SECTOR_SIZE)
        readSize+=DISK_SECTOR_SIZE;
    
    //Adjust read size to nearest full sector size
    if (readSize%DISK_SECTOR_SIZE!=0)
        readSize+=DISK_SECTOR_SIZE-(readSize%DISK_SECTOR_SIZE);
    
    //allocate our internal buffer, rounding up to sector size
    startSector=fileHandles[FILE].base+fileHandles[FILE].offset;
    startSector/=DISK_SECTOR_SIZE;
    
    //TODO: Address offset!!!
    sessions[fileHandles[FILE].sess].readFunction((fileHandles[FILE].base+fileHandles[FILE].offset)/DISK_SECTOR_SIZE,
            iBuffer, 
            sectorsForByteCount(readSize));
    
    memcpy(buffer,iBuffer+iBufferOffset,realSize);
    
    memset(iBuffer,0,readSize);

    fileHandles[FILE].offset+=realSize;
    
    //freeF(iBuffer);
    return realSize;
}

