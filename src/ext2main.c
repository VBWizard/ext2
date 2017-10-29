#include "ext2.h"
#include <fcntl.h>
#include <linux/fs.h>
#include "ext2inode.h"
#include "ext2dir.h"
#include "ext2blockgroup.h"
#include "ext2utility.h"
#include "ext2read.h"
#include "ext2session.h"
#include "ext2block.h"

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
    
    setFileBlockList(file);

    

    
    //printf("File: %s, size=%u, path=%s\n",file->name,file->inode.i_size,file->path);
    freeF(name);
    return 0;
}


int ext2open (int sess, char* fileName)
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
    int lResult=getFile(&fileHandles[file], fileName);
    if (lResult)
    {
        fileHandles[file].filePtr=0;
        fileHandles[file].sess=0;
        fileHandles[file].offset=0;
        file=lResult;
    }
    else
        fileHandles[file].base=fileHandles[file].inode.i_block[0]*sessions[sess].blockSize;
    return file;
}

int ext2close(int FILE)
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

int ext2seek (int FILE, int pos, int whence)
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
