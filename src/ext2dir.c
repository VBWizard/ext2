#include "ext2dir.h"
#include "ext2inode.h"
#include "utility.h"

int getDir(sFile *file, struct ext2_inode parent, char* dirBuffer, int bufferSize)
{
   int sizeToRead;
   
   if (parent.i_size>bufferSize)
       sizeToRead=bufferSize;
   else
       sizeToRead=parent.i_size;
   
    sessions[file->sess].readFunction( (parent.i_block[0]*sessions[file->sess].blockSize)/DISK_SECTOR_SIZE,
            dirBuffer,
            sectorsForByteCount(sizeToRead));
    
    //Clear out the unused space in the buffer
/*    if (sizeToRead<bufferSize)
        memset(dirBuffer+sizeToRead,
                0,
                bufferSize-sizeToRead);
*/
    return sizeToRead;
}

int getDirEntry(sFile *file, struct ext2_inode parent, char* path, int* fileType)
{
    struct ext2_dir_entry* entry;
    unsigned int size=0;
    char file_name[EXT2_NAME_LEN+1];
    struct ext2_inode dirInode=sessions[file->sess].rootDirInode;
    
    char* buffer=mallocF(sessions[file->sess].rootDirInode.i_size);
    sessions[file->sess].readFunction( (parent.i_block[0]*sessions[file->sess].blockSize)/DISK_SECTOR_SIZE,
            buffer,
            sectorsForByteCount(sessions[file->sess].rootDirInode.i_size));

    entry = (struct ext2_dir_entry *)buffer;

    while (size<parent.i_size)
    {
        memcpy(file_name, entry->name, entry->name_len);
        file_name[entry->name_len] = 0;              /* append null char to the file name */
        printf("%10u %s - %u\n", entry->inode, file_name, entry->file_type);
        if (!strcmp(file_name,path))
            break;
        entry = (void*) entry + entry->rec_len;      /* move to the next entry */
        size += entry->rec_len;
        file_name[0]=0;
    }
    if (file_name[0]!=0)
    {
        *fileType=entry->file_type;
        return entry->inode;
    }
    else
        return ERROR_INODE_NOT_FOUND;
    
    freeF(buffer);
}
