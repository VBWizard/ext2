/* 
 * File:   utility.h
 * Author: yogi
 *
 * Created on October 16, 2017, 6:34 AM
 */

#ifndef UTILITY_H
#define	UTILITY_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "ext2.h"
#include "ext2file.h"
    
    int sectorsForByteCount(int bytes);
    bool invalidFileHandle(int FILE);
    unsigned int bitmapFFZ(unsigned int* array, int startIndex, int size);
    int arrayMaxValue(unsigned int* array, size_t arraySize);
    int bitmapSet(unsigned int* array, int index);
    int bitmapUnSet(unsigned int* array, int index);
    
#ifdef	__cplusplus
}
#endif

#endif	/* UTILITY_H */

