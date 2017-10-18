#include "utility.h"

int sectorsForByteCount(int bytes)
{
    if (bytes<DISK_SECTOR_SIZE)
        return 1;
    else
        return bytes/DISK_SECTOR_SIZE;
}

bool invalidFileHandle(int FILE)
{
    if (FILE>FILE_HANDLE_MAX_COUNT || FILE<0 || fileHandles[FILE].filePtr==0)
        return ERROR_INVALID_FILE_HANDLE;

    return 0;
}
