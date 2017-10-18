#include "ext2.h"
#include <fcntl.h>
#include <linux/fs.h>
#include "ext2inode.h"
#include "ext2dir.h"
#include "ext2blockgroup.h"
#include "utility.h"
#include "ext2read.h"


//Parameters:
//  readFunction: Function pointer to a function which reads from the disk
// Returns a (positive) session number or a (negative) error number
//  writeFunction: Function pointer to a function which writes to the disk
//NOTE: Device name/id doesn't have to be passed because read & write functions should read/write against an open device



/*
static int read_inode(int fd,        // the floppy disk file descriptor
     int                           inode_no,  // the inode number to read 
     const struct ext2_group_desc *group,     // the block group to which the inode belongs
     struct ext2_inode            *inode,     // where to put the inode
     unsigned int                 bg_offset,
     int block_size,
     int inode_size)     
{
    int inode_table_location=0;
    inode_table_location=bg_offset + (group->bg_inode_table-1)*block_size;
    
    inode_table_location+=((inode_no-1)*inode_size);
    int pos=inode_table_location;
    lseek(fd, pos, SEEK_SET);
    read(fd, inode, sizeof(struct ext2_inode));
    return pos;
}
*/

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

int internalInit(int sess)
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
    getBlockGroupDescriptor(sess, 0, &gd);
    read_inode(sess,2,&sessions[sess].rootDirInode);
    //read_inode(sessions[sess].device,2,&gd,,4096,sessions[sess].blockSize, sessions[sess].superBlock.s_inode_size);
    return 0;
}

int initInstance(void* readFunction, void* writeFunction)
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
    lRetVal=internalInit(sess);
    if (lRetVal==0)
        return sess;
    else
        return lRetVal;
    
}

int setBlockList(sFile *file)
{
    int blockListSize=0;
    int curr=0,currSaveBegin=0,currSaveEnd=0,currTempListPtr=0;
    unsigned int* lvl2BlockList;
    unsigned int* lvl3BlockList;

    //Figure out how big the block list needs to be
    for (int cnt=0;cnt<15;cnt++)
    {
        if (file->inode.i_block[cnt]>0)
        {
            if (cnt<12)
                blockListSize++;
            else if (cnt==12)
                blockListSize+=1024;
            else if (cnt==13)
                blockListSize+=1024*1024;
            else if (cnt==14)
                blockListSize+=1024*1024*1024;
        }
        else
            break;
    }
    //Allocate the space for the list
    file->blockList=mallocF(blockListSize*4);
    memset(file->blockList,0,blockListSize*4);
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
            lvl2BlockList=mallocF(4096);
            lvl3BlockList=mallocF(4096);
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
                
/*                for (int lvl3=0;lvl3<1024;lvl3++)
                {
                    if (lvl3BlockList[lvl3]==0)
                        break;
                    readBlock(file,               //session
                            lvl3BlockList[lvl3],   //block to read
                            &file->blockList[curr]); //buffer address
                while (file->blockList[curr]!=0)
                    curr++;
                }
*/                //bring curr back to the end
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
}

int getFile(sFile *file, char* fileName)
{
    char* token;
    int lInode=0;
    struct ext2_group_desc gd;
    int fileType=0;
    struct ext2_inode dirInode=sessions[file->sess].rootDirInode;
    
    char* name=mallocF(strlen(fileName));
    strcpy(name,fileName);
    const char delim[2]="/";
    token=strtok(name,delim);
    memset(file->path,0,1024);
    do
    {
        lInode=getDirEntry(file, dirInode,token,&fileType);
        if (lInode<0)
            return lInode;
        read_inode(file->sess,lInode,&file->inode);
        if (fileType==1)        //Regular file - we've found the file we want
        {
            file->name=mallocF(strlen(token)+1);
            strcpy(file->name,token);
            file->inodeNumber=lInode;
            if (file->inode.i_size>sessions[file->sess].blockSize) //If the file is larger than 1 block then
                file->lastBlockFreeSize=file->inode.i_size % sessions[file->sess].blockSize;
            else
                file->lastBlockFreeSize=sessions[file->sess].blockSize-file->inode.i_size;
            break;
        }
        else if (fileType==7)   //Symbolic link - the name of the file we want is in the i_block array
        {
            //Copy the path we constructed to the symlinkPath
            strcpy(file->symlinkPath,file->path);
            strcpy(file->symlinkPath,"/");
            strcpy(file->symlinkPath,token);
            
            //Clear the path
            memset(file->path,0,1024);
            //Reset strtok
            do
            {
                token=strtok(NULL,delim);
            } while (token!=NULL);
            
            //Copy the hard path to the search path
            strcpy(name,(char*)&file->inode.i_block);
            
            //Start searching again at the beginning of the new path
            token=strtok(name,delim);
            
            //Start searching at the root directory
            dirInode=sessions[file->sess].rootDirInode;
        }
        else
        {
            strcat(file->path,"/");
            strcat(file->path,token);
            dirInode=file->inode;
            token=strtok(NULL,delim);
        }
    }
    while (token!=NULL);
    
    setBlockList(file);

    

    
    //printf("File: %s, size=%u, path=%s\n",file->name,file->inode.i_size,file->path);
    freeF(name);
}


int ext2Open (int sess, char* fileName)
{
    int file=0;
    for (int cnt=1;cnt<FILE_HANDLE_MAX_COUNT;cnt++)
        if (fileHandles[cnt].filePtr==0)
        {
            fileHandles[cnt].filePtr=cnt;
            fileHandles[cnt].sess=sess;
            fileHandles[cnt].offset=0;
            file=cnt;
            break;
        }
    if (!file)
        return ERROR_NO_FREE_FILE_HANDLE;
    getFile(&fileHandles[file], fileName);
    fileHandles[file].base=fileHandles[file].inode.i_block[0]*sessions[sess].blockSize;
    return file;
}

int ext2Close(int FILE)
{
    if (invalidFileHandle(FILE))
        return ERROR_INVALID_FILE_HANDLE;

    if (fileHandles[FILE].filePtr==FILE)
    {
            fileHandles[FILE].filePtr=0;
            fileHandles[FILE].sess=0;
            fileHandles[FILE].offset=0;
    }
    
    return 0;
}


/*int updateInode(int FILE, sFile* file)
{
    sessions[fileHandles[FILE].sess].writeFunction(fileHandles[FILE].base+fileHandles[FILE].offset,
            (char*)&file->inode,
            sectorsForByteCount(sessions[fileHandles[FILE].sess].superBlock.s_inode_size));
}
*/

int ext2Write (int FILE, char* buffer, int bytes)
{
    int newBytes=0;
    
    if (invalidFileHandle(FILE))
        return ERROR_INVALID_FILE_HANDLE;
    
    //Update the file size if the write will take us past the current end of the file
    if (fileHandles[FILE].offset+bytes>fileHandles[FILE].inode.i_size)
    {
        newBytes=fileHandles[FILE].inode.i_size-fileHandles[FILE].offset+bytes;
        fileHandles[FILE].inode.i_size+=newBytes;
    }
    
    //If there's enough room in the last assigned block for this file then just write to that block
    if (fileHandles[FILE].lastBlockFreeSize>newBytes)
        sessions[fileHandles[FILE].sess].writeFunction(sessions[fileHandles[FILE].sess].device,buffer, bytes,1);
    
//    updateInode(FILE,&fileHandles[FILE]);
}

int ext2Seek (int FILE, int pos, int whence)
{
    if (invalidFileHandle(FILE))
        return ERROR_INVALID_FILE_HANDLE;

    //NOTE: Seeking past the end of the file doesn't extend the file, however executing a write afterwards will
    switch (whence)
    {
        case SEEK_DATA: //In the simplest implementation, a file system can support the operations by making SEEK_DATA always return 
                        //offset (i.e., even if the location referred to by offset is a hole, it can be considered to consist of data that is a sequence of zeros). 
            break;
        case SEEK_SET:
            fileHandles[FILE].offset=pos;
            break;
        case SEEK_END:
            fileHandles[FILE].offset=fileHandles[FILE].inode.i_size+pos;
            break;
        case SEEK_HOLE: //In the simplest implementation, a file system can support the operations by making SEEK_HOLE always return 
                        //the offset of the end of the file
            fileHandles[FILE].offset=fileHandles[FILE].inode.i_size;
            break;
        case SEEK_CUR:
            fileHandles[FILE].offset+=pos;
            break;
            
    }
    //sessions[fileHandles[FILE].sess].seekFunction(sessions[fileHandles[FILE].sess].device,pos, whence);
    return fileHandles[FILE].offset;
    
}
