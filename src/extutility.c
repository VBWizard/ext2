#include "ext2utility.h"
#include <limits.h>

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

int arrayMaxValue(unsigned int* array, size_t arraySize)
{
    unsigned int maxValue=0;
    
    for (int cnt=0;cnt<arraySize;cnt++)
    {
        if (array[cnt]>maxValue)
            maxValue=array[cnt];
    }
    
    return maxValue;
}

/*
 * find the position of the first 0 in a 32-bit array
 */
unsigned short __attribute((noinline)) find_first_zero(unsigned int bit_array)
{
    unsigned pos = 0;

    __asm__("bsf eax,ebx\n\t"
        "jne 1f\n\t"
        "mov eax,0xffff\n"
        "1:"
        :"=a" (pos)
        :"b" (bit_array));

    return (unsigned short) pos;
}

//Find the first zero in an array
unsigned int bitmapFFZ(unsigned int* array, int size)
{
    int index=0,a=0;
    for (int cnt=0;cnt<size;cnt++)
    {

        index=find_first_zero(~(array[cnt]));

        if (index!=0xFFFF)
            return cnt*(sizeof(unsigned int)*8)+(index);
    }
    
    
    return -1;
}

int bitmapSet(unsigned int* array, int index)
{

    int arrayIndex=index/(sizeof(int)*8);
    int value=1<<(index%(sizeof(int)*8));
    
    array[arrayIndex] |= value;
    
}
int bitmapUnSet(unsigned int* array, int index)
{

    int arrayIndex=index/sizeof(int)*8;
    int value=UINT_MAX;
    value+=(index%sizeof(int)*8);
    
    array[arrayIndex] |= value;
    
}
